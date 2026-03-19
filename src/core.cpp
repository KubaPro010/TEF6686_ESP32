#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Wire.h>
#include <Hash.h>
#include <FS.h>
using fs::FS;
#include <SPIFFS.h>
#include "NTPupdate.h"
#include "globals.h"
#include "gui.h"
#include "comms.h"
#include "rds.h"
#include "nonvolatile.h"
#include "utils.h"
#include "system_console.h"
#include "core.h"
#include "main.h"
#include "logbook.h"
#include "graphics.h"

Console console(&tft);
RTC_DATA_ATTR bool gpio_chip = false;
RTC_DATA_ATTR bool tef_found = false;

#pragma region helpers
bool isSignalQualityGood(int usn, int wam, int ostatus, int threshold_multiplier, int ostatus_threshold) {
  return (usn < fmscansens * threshold_multiplier) && (wam < SCAN_SIGNAL_THRESHOLD_WAM) &&
         (ostatus < ostatus_threshold) && (ostatus > -ostatus_threshold);
}

void Round30K(unsigned int freq) {
  if (freq % FREQ_OIRT_STEP_30K == 1) frequency_OIRT = (freq + 1);
  else if (freq % FREQ_OIRT_STEP_30K == 0) frequency_OIRT = (freq - 1);
}

void Round50K(unsigned int freq) {
  if (freq % 10 < 3) frequency = (freq - freq % 10);
  else if (freq % 10 > 2 && freq % 10 < 8) frequency = (freq - (freq % 10 - 5));
  else if (freq % 10 > 7) frequency = (freq - (freq % 10) + 10);
}

void Round100K(unsigned int freq) {
  if (freq % 10 < 5) frequency = (freq - freq % 10);
  else frequency = (freq - (freq % 10) + 10);
}

void Round200K(unsigned int freq) {
  frequency = (freq / 10) * 10;

  if (freq % 10 >= 5) frequency += 10;
  if ((frequency / 100) % 2 == 0) frequency += 10;
  if (frequency > 10800) frequency = 10790;
  if (frequency % 20 == 0 && (frequency / 10) % 2 == 0) frequency -= 10;
}

void Round5K(unsigned int freqAM) {
  if (freqAM % 10 < 3) frequency_AM = (freqAM - freqAM % 10);
  else if (freqAM % 10 > 2 && freqAM % 10 < 8) frequency_AM = (freqAM - (freqAM % 10 - 5));
  else if (freqAM % 10 > 7) frequency_AM = (freqAM - (freqAM % 10) + 10);
}

void deepSleep() {
  MuteScreen(true);
  StoreFrequency();
  radio.power(true);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)ROTARY_PIN_A, LOW);
  esp_deep_sleep_start();
}

inline __attribute__((always_inline)) void EdgeBeeper() {
  if(!edgebeep) return;
  radio.tone(50, -6, 2000);
}

const char* textUI(uint16_t number) {
  if (number >= language_entrynumber) return "[OVERFLOW]";
  else return (const char*)pgm_read_ptr(&(Languages[language][number]));
}

void updateSWMIBand() {
  switch (SWMIBandPos) {
    case SW_MI_BAND_11M:
    case SW_MI_BAND_13M:
    case SW_MI_BAND_15M:
    case SW_MI_BAND_16M:
    case SW_MI_BAND_19M:
    case SW_MI_BAND_22M:
    case SW_MI_BAND_25M:
    case SW_MI_BAND_31M:
    case SW_MI_BAND_41M:
    case SW_MI_BAND_49M:
    case SW_MI_BAND_60M:
    case SW_MI_BAND_75M:
    case SW_MI_BAND_90M:
    case SW_MI_BAND_120M:
    case SW_MI_BAND_160M:
      FrequencySprite.drawString(SWMIBandstring + " ", 0, 0, 6);
      beepresetstart = true;
      if (beepresetstop) {
        EdgeBeeper();
        beepresetstop = false;
      }
      break;

    case SW_MI_BAND_GAP:
      beepresetstop = true;
      if (beepresetstart) {
        EdgeBeeper();
        beepresetstart = false;
      }
      break;
  }
}

void WakeToSleep(bool yes) {
  if (yes) {
    screensavertriggered = true;
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(true);
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
        analogWrite(CONTRASTPIN, map(ContrastSet / 100, 0, 100, 0, 255));
        break;
      case LCD_BRIGHTNESS_A_QUARTER:
        analogWrite(CONTRASTPIN, map(ContrastSet / 4, 0, 100, 0, 255));
        break;
      case LCD_BRIGHTNESS_HALF:
        analogWrite(CONTRASTPIN, map(ContrastSet / 2, 0, 100, 0, 255));
        break;
    }
  } else {
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(false);
        screensavertriggered = false;
        screensavertimer = millis();
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
      case LCD_BRIGHTNESS_A_QUARTER:
      case LCD_BRIGHTNESS_HALF:
        MuteScreen(false);
        screensavertriggered = false;
        screensavertimer = millis();
        break;
    }
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
  }
}

void CheckBandForbiddenFM() {
  switch (band) {
    case BAND_FM:
      if (bandFM == FM_BAND_OIRT) bandforbidden = 1; else bandforbidden = 0;
      break;
    case BAND_OIRT:
      if (bandFM == FM_BAND_FM) bandforbidden = 1; else bandforbidden = 0;
      break;
  }
}

void CheckBandForbiddenAM() {
  switch (band) {
    case BAND_LW:
      if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW || bandAM == AM_BAND_SW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW || bandAM == AM_BAND_SW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
    case BAND_SW:
      if (bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW || bandAM == AM_BAND_MW) bandforbidden = 1;
      else bandforbidden = 0;
      break;
  }
}

void FMjumptoAM() {
  if (bandAM == AM_BAND_ALL || bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_LW) {
    band = BAND_LW;
    if (stepsize > 3) stepsize = 3;
  } else if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_MW) band = BAND_MW;
  else if (bandAM == AM_BAND_SW) band = BAND_SW;
}

void AMjumptoFM() {
  if (bandFM != FM_BAND_NONE) {
    if (bandFM == FM_BAND_FM) band = BAND_FM; else band = BAND_OIRT;
  } else FMjumptoAM();
}

void ToggleBand(byte nowBand) {
  switch (nowBand) {
    case BAND_LW:
      if (bandAM == AM_BAND_LW_MW || bandAM == AM_BAND_ALL) band = BAND_MW;
      else if (bandAM == AM_BAND_LW_SW) band = BAND_SW;
      else if (bandAM == AM_BAND_LW || bandAM == AM_BAND_NONE) AMjumptoFM();
      break;
    case BAND_MW:
      if (bandAM == AM_BAND_MW_SW || bandAM == AM_BAND_ALL) {
        band = BAND_SW;
      } else if (bandAM == AM_BAND_LW_MW) {
        if (bandFM != FM_BAND_NONE) {
          if (bandFM == FM_BAND_FM) band = BAND_FM; else band = BAND_OIRT;
        } else band = BAND_LW;
      } else if (bandAM == AM_BAND_MW || bandAM == AM_BAND_NONE) AMjumptoFM();
      break;
    case BAND_SW:
      if (bandFM != FM_BAND_NONE) {
        if (bandFM == FM_BAND_FM) band = BAND_FM;
        else band = BAND_OIRT;
      } else {
        if (bandAM == AM_BAND_LW_SW || bandAM == AM_BAND_ALL) band = BAND_LW;
        else if (bandAM == AM_BAND_MW_SW) band = BAND_MW;
        else if (bandAM == AM_BAND_SW || bandAM == AM_BAND_NONE) AMjumptoFM();
      }
      break;
    case BAND_OIRT:
      if(bandFM == FM_BAND_ALL || bandFM == FM_BAND_FM) band = BAND_FM;
      else if(bandFM == FM_BAND_OIRT && bandAM != AM_BAND_NONE) FMjumptoAM();
      break;
    case BAND_FM:
      if (bandAM != AM_BAND_NONE) FMjumptoAM();
      else if (bandFM == FM_BAND_OIRT || bandFM == FM_BAND_ALL) band = BAND_OIRT;
      break;
  }
}

void doBandSelectionFM() {
  if (band > BAND_GAP) return;

  switch (bandFM) {
    case FM_BAND_ALL:
      break;
    case FM_BAND_OIRT:
      if (band == BAND_FM) {
        band = BAND_OIRT;
        if (frequency_OIRT > FREQ_FM_OIRT_END) {
          frequency_OIRT = FREQ_FM_OIRT_START;
          EdgeBeeper();
        } else if (frequency_OIRT < FREQ_FM_OIRT_START) {
          frequency_OIRT = FREQ_FM_OIRT_END;
          EdgeBeeper();
        } SelectBand();
      }
      break;
    case FM_BAND_FM:
      if (band == BAND_OIRT) {
        band = BAND_FM;
        if (frequency > HighEdgeSet * 10) {
          frequency = LowEdgeSet * 10;
          EdgeBeeper();
        } else if (frequency < LowEdgeSet * 10) {
          frequency = HighEdgeSet * 10;
          EdgeBeeper();
        } SelectBand();
      }
      break;
    case FM_BAND_NONE:
      ToggleBand(band);
      SelectBand();
      break;
  }
}

void doBandSelectionAM() {
  if (band < BAND_GAP) return;

  switch (bandAM) {
    case AM_BAND_ALL: break;
    case AM_BAND_LW_MW:
      if (band == BAND_SW) {
        band = BAND_LW;
        SelectBand();
      } break;
    case AM_BAND_LW_SW:
      if (band == BAND_MW) {
        band = BAND_LW;
        SelectBand();
      } break;
    case AM_BAND_MW_SW:
      if (band == BAND_LW) {
        band = BAND_MW;
        SelectBand();
      } break;
    case AM_BAND_LW:
      if (band != BAND_LW) {
        band = BAND_LW;
        SelectBand();
      } break;
    case AM_BAND_MW:
      if (band != BAND_MW) {
        band = BAND_MW;
        SelectBand();
      } break;
    case AM_BAND_SW:
      if (band != BAND_SW) {
        band = BAND_SW;
        SelectBand();
      } break;
    case AM_BAND_NONE:
      ToggleBand(band);
      SelectBand();
      break;
  }
}

void BANDBUTTONPress() {
  if(seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    ShowFreq(5);
    ShowFreq(0);
  } else {
    if (memorystore) {
      EEPROM.writeByte(memorypos + EE_PRESETS_BAND_START, BAND_FM);
      EEPROM.writeUInt((memorypos * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
      EEPROM.commit();
      presets[memorypos].band = BAND_FM;
      presets[memorypos].frequency = EE_PRESETS_FREQUENCY;
      memorystore = false;
      ShowTuneMode();
      if (memoryposstatus == MEM_DARK || memoryposstatus == MEM_EXIST) {
        memoryposstatus = MEM_NORMAL;
        ShowMemoryPos();
      }
    } else {
      if (!usesquelch) radio.setUnMute();
      unsigned long counterold = millis();
      unsigned long counter = millis();
      if (!BWtune && !menu) {
        while (digitalRead(BANDBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold < 1000) {
          if (afscreen || rdsstatscreen) {
            leave = true;
            BuildAdvancedRDS();
            freq_in = 0;
          } else if (advancedRDS) {
            leave = true;
            BuildDisplay();
            freq_in = 0;
            SelectBand();
            screensavertimer = millis();
          } else doBandToggle();
        } else {
          if (band < BAND_GAP) {
            if (advancedRDS && !seek) {
              BuildAFScreen();
              freq_in = 0;
            } else {
              BuildAdvancedRDS();
              freq_in = 0;
            }
          } else WakeToSleep(true);
          while (digitalRead(BANDBUTTON) == LOW && counter - counterold <= 2500) counter = millis();
          if (counter - counterold > 2499) {
            switch (longbandpress) {
              case STANDBY:
                deepSleep();
                break;
              case SCREENOFF:
                screensavertriggered = true;
                MuteScreen(1);
                break;
            }
          }
        }
      }
    }
  }
  while (digitalRead(BANDBUTTON) == LOW) delay(DELAY_BUTTON_DEBOUNCE_MS);
  delay(DELAY_BUTTON_DEBOUNCE_EXTRA_MS);
}

void LimitAMFrequency() {
  switch (band) {
    case BAND_LW:
      frequency_AM = frequency_LW;
      if (frequency_AM > LWHighEdgeSet || frequency_AM < LWLowEdgeSet) frequency_AM = LWLowEdgeSet;
      break;
    case BAND_MW:
      frequency_AM = frequency_MW;
      if (frequency_AM > MWHighEdgeSet || frequency_AM < MWLowEdgeSet) frequency_AM = MWLowEdgeSet;
      break;
    case BAND_SW:
      frequency_AM = frequency_SW;
      if (frequency_AM > SWHighEdgeSet || frequency_AM < SWLowEdgeSet) frequency_AM = SWLowEdgeSet;
      break;
  }
}

void BWButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (afscreen || rdsstatscreen) BuildRDSStatScreen();
  else {
    if (scandxmode) {
      unsigned long counterold = millis();
      unsigned long counter = counterold;
      while (digitalRead(BWBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

      if (counter - counterold < 1000) {
        ShowFreq(5);
        ShowFreq(0);
      } else cancelDXScan();
    } else {
      if (!usesquelch) radio.setUnMute();
      if (!BWtune && !menu) {
        if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        unsigned long counterold = millis();
        unsigned long counter = millis();
        while (digitalRead(BWBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold < 1000) {
          BuildBWSelector();
          freq_in = 0;
          BWtune = true;
          BWtemp = BWset;
        } else {
          if (band == BAND_FM || band == BAND_OIRT) doStereoToggle();
          else {
            BuildBWSelector();
            freq_in = 0;
            BWtune = true;
          }
        }
      }
    }
  }
  while (digitalRead(BWBUTTON) == LOW) delay(DELAY_BUTTON_DEBOUNCE_MS);
  delay(DELAY_BUTTON_DEBOUNCE_EXTRA_MS);
}

void doStereoToggle() {
  if (StereoToggle) {
    if (!screenmute) {
      tft.drawBitmap(32, 5, Stereo, 32, 22, BackgroundColor);
      tft.drawBitmap(38, 5, Mono, 22, 22, SecondaryColor);
    }
    radio.setMono(true);
    StereoToggle = false;
  } else {
    if (!screenmute) {
      tft.drawBitmap(38, 5, Mono, 22, 22, BackgroundColor);
      tft.drawBitmap(32, 5, Stereo, 32, 22, GreyoutColor);
    }
    radio.setMono(false);
    Stereostatusold = false;
    StereoToggle = true;
  }
  radio.setAudio(audiomode);
  EEPROM.writeByte(EE_BYTE_AUDIOMODE, audiomode);
  EEPROM.writeByte(EE_BYTE_STEREO, StereoToggle);
  EEPROM.commit();
}

bool IsFrequencyUsed(unsigned int freq) {
  for (byte x = scanstart; x <= scanstop; x++) {
    if ((presets[x].band == BAND_FM || presets[x].band == BAND_OIRT) && presets[x].frequency == freq) return true;
  } return false;
}

void DataPrint(String string) {
  if(XDRGTKUSB) Serial.print(string);
  if(XDRGTKTCP) RemoteClient.print(string);
}

void toggleiMSEQ() {
  if (band < BAND_GAP) {
    if (iMSEQ == 0) iMSEQ = 1;

    if (iMSEQ == 4) {
      iMSset = 0;
      EQset = 0;
      updateiMS();
      updateEQ();
      iMSEQ = 0;
    } else if (iMSEQ == 3) {
      iMSset = 1;
      EQset = 0;
      updateiMS();
      updateEQ();
      iMSEQ = 4;
    } else if (iMSEQ == 2) {
      iMSset = 0;
      EQset = 1;
      updateiMS();
      updateEQ();
      iMSEQ = 3;
    } else if (iMSEQ == 1) {
      iMSset = 1;
      EQset = 1;
      updateiMS();
      updateEQ();
      iMSEQ = 2;
    }
    EEPROM.writeByte(EE_BYTE_IMSSET, iMSset);
    EEPROM.writeByte(EE_BYTE_EQSET, EQset);
    EEPROM.commit();
    if (XDRGTKUSB || XDRGTKTCP) DataPrint("G" + String(!EQset) + String(!iMSset) + "\n");
  }
}

bool TuneFreq(int temp) {
  int newfreq = temp;

  if (band == BAND_FM) {
    while (newfreq < (LowEdgeSet * 10)) newfreq *= 10;
    if (newfreq > (HighEdgeSet * 10)) {
      EdgeBeeper();
      return false;
    }
    if (newfreq == frequency) return true;
    frequency = newfreq;
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    while (temp < LowEdgeOIRTSet) temp = temp * 10;
    if (newfreq > HighEdgeOIRTSet) {
      EdgeBeeper();
      return false;
    }
    if (newfreq == frequency_OIRT) return true;
    frequency_OIRT = newfreq;
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    while (newfreq < LWLowEdgeSet) newfreq *= 10;
    if (newfreq > LWHighEdgeSet) {
      EdgeBeeper();
      return false;
    }
    if (newfreq == frequency_LW) return true;
    frequency_AM = newfreq;
    frequency_LW = newfreq;
    radio.SetFreqAM(frequency_AM);
  } else if (band == BAND_MW) {
    while (newfreq < MWLowEdgeSet) newfreq *= 10;
    if (newfreq > MWHighEdgeSet) {
      EdgeBeeper();
      return false;
    }
    if (newfreq == frequency_MW) return true;
    frequency_AM = newfreq;
    frequency_MW = newfreq;
    radio.SetFreqAM(frequency_AM);
  } else if (band == BAND_SW) {
    while (newfreq < SWLowEdgeSet) newfreq *= 10;
    if (newfreq > SWHighEdgeSet) {
      EdgeBeeper();
      return false;
    }
    if (newfreq == frequency_SW) return true;
    frequency_AM = newfreq;
    frequency_SW = newfreq;
    radio.SetFreqAM(frequency_AM);
  }

  aftest = true;
  aftimer = millis();

  radio.clearRDS();
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
  return true;
}

void ShowNum(int val) {
  FrequencySprite.setTextDatum(TR_DATUM);
  FrequencySprite.fillSprite(BackgroundColor);
  FrequencySprite.setTextColor(SecondaryColor, SecondaryColorSmooth, false);
  FrequencySprite.drawString(String(val) + " ", 218, -6, freqfont);
  FrequencySprite.pushSprite(46, 46);
}

constexpr byte numval[16] = {2, 3, 127, 5, 6, 0, 9, 13, 8, 7, 4, 1, 0, 0, 0, 0};
int GetNum() {
  if(!gpio_chip) return -1;
  // Get input port 0 and 1
  Wire.beginTransmission(XL9555_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(XL9555_ADDRESS, 2);

  // According to the schematic of the DP666, this is the table that the buttons are connected to:
  // IO0_0 = NUM2
  // IO0_1 = NUM3
  // IO0_2 = BACKSPACE (DX is printed on the case)
  // IO0_3 = NUM5
  // IO0_4 = NUM6
  // IO0_5 = NUM0
  // IO0_6 = NUM9
  // IO0_7 = ENTER
  // IO1_0 = NUM8
  // IO1_1 = NUM7
  // IO1_2 = NUM4
  // IO1_3 = NUM1
  // Rest is NC
  // According to the docs, register 0 contains pins IO0_x where x is equal to the bit from the right (x = 7, is MSB)

  int cnt = 0;
  uint16_t temp;
  unsigned int num;
  if(Wire.available() == 2) {
    keypadtimer = millis();

    temp = Wire.read() & 0xFF;
    temp |= (Wire.read() & 0xFF) << 8;
    for (int i = 0; i < 16; i++) {
      if ((temp & 1) == 0) {
        num = numval[i];
        cnt++;
      }
      temp >>= 1;
    }
    if (cnt == 1) return num;
  }

  return -1;
}

void StoreMemoryPos(uint8_t _pos) {
  EEPROM.writeByte(_pos + EE_PRESETS_BAND_START, band);
  EEPROM.writeByte(_pos + EE_PRESET_BW_START, BWset);
  EEPROM.writeByte(_pos + EE_PRESET_MS_START, StereoToggle);

  if (band == BAND_FM) EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency);
  else if (band == BAND_OIRT) EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_OIRT);
  else if (band == BAND_LW) EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_LW);
  else if (band == BAND_MW) EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_MW);
  else EEPROM.writeUInt((_pos * 4) + EE_PRESETS_FREQUENCY_START, frequency_SW);

  presets[_pos].band = band;
  presets[_pos].bw = BWset;
  presets[_pos].ms = StereoToggle;

  String stationName = radio.rds.stationName;
  char stationNameCharArray[10];
  char picodeArray[7];
  stationName.toCharArray(stationNameCharArray, sizeof(stationNameCharArray));
  memcpy(picodeArray, radio.rds.picode, sizeof(picodeArray));

  for (int y = 0; y < 9; y++) {
    presets[_pos].RDSPS[y] = (y < strlen(stationNameCharArray)) ? stationNameCharArray[y] : '\0';
    EEPROM.writeByte((_pos * 9) + y + EE_PRESETS_RDSPS_START, presets[_pos].RDSPS[y]);
  }

  for (int y = 0; y < 5; y++) {
    presets[_pos].RDSPI[y] = (y < sizeof(picodeArray)) ? picodeArray[y] : '\0';
    EEPROM.writeByte((_pos * 5) + y + EE_PRESETS_RDSPI_START, presets[_pos].RDSPI[y]);
  }

  EEPROM.commit();

  if (band == BAND_FM) presets[_pos].frequency = frequency;
  else if (band == BAND_OIRT) presets[_pos].frequency = frequency_OIRT;
  else if (band == BAND_LW) presets[_pos].frequency = frequency_LW;
  else if (band == BAND_MW) presets[_pos].frequency = frequency_MW;
  else presets[_pos].frequency = frequency_SW;
}

void startFMDXScan() {
  screensavertimer = millis();
  initdxscan = true;
  scanholdflag = false;
  autologged = false;

  if (menu) endMenu();
  if (afscreen || advancedRDS || rdsstatscreen) {
    BuildDisplay();
    freq_in = 0;
  }

  memoryposold = memorypos;
  if (memorypos > scanstop || memorypos < scanstart) memorypos = scanstart;
  scanmodeold = tunemode;

  if (scanmem) {
    tunemode = TUNE_MEM;
    if (band != presets[memorypos].band) {
      band = presets[memorypos].band;
      SelectBand();
    } DoMemoryPosTune();
  } else {
    tunemode = TUNE_MAN;
    if (band != presets[memorypos].band) {
      band = presets[memorypos].band;
      SelectBand();
    }
    TuneUp();
    ShowFreq(0);
  } if (scanmute) {
    radio.setMute();
    tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
    SQ = true;
    Squelchold = -2;
  }
  scantimer = millis();
  scandxmode = true;
  ShowTuneMode();
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("J1\n");
}

void doBandToggle() {
  if (tunemode != TUNE_MEM) {
    ToggleBand(band);
    radio.clearRDS();
    StoreFrequency();
    SelectBand();
    if (XDRGTKUSB || XDRGTKTCP) {
      if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n");
      else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n");
      else DataPrint("M1\nT" + String(frequency_AM) + "\n");
    }
  } else {
    scanmodeold = tunemode;
    startFMDXScan();
    return;
  } screensavertimer = millis();
}

void NumpadProcess(int num) {
  if (scandxmode && num == 127) {
    ShowFreq(5);
    ShowFreq(0);
  } else if (tunemode == TUNE_MEM) {
    if (num == 127) {
      freq_in = 0;
      menuoption = ITEM1;
      menupage = DXMODE;
      menuitem = 0;
      submenu = true;
      menu = true;
      if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
      BuildMenu();
    } else if (num == 13) {
      if (freq_in > 0 && freq_in <= EE_PRESETS_CNT) {
        byte oldmemorypos = memorypos;
        memorypos = freq_in - 1;
        if (IsStationEmpty()) {
          // Flash red and revert
          FrequencySprite.setTextDatum(TR_DATUM);
          FrequencySprite.fillSprite(BackgroundColor);
          FrequencySprite.setTextColor(SignificantColor, SignificantColorSmooth, false);
          FrequencySprite.drawString(String(freq_in) + " ", 218, -6, freqfont);
          FrequencySprite.pushSprite(46, 46);
          delay(505);
          memorypos = oldmemorypos;
          ShowFreq(0);
        } else DoMemoryPosTune();
        ShowMemoryPos();
      } else ShowFreq(0);
      freq_in = 0;
    } else {
      if (freq_in / 100 == 0) {
        int temp = freq_in * 10 + num;
        if (temp <= EE_PRESETS_CNT) freq_in = temp;
      } ShowNum(freq_in);
    }
  } else {
    if (num == 127) {
      freq_in = 0;
      menuoption = ITEM1;
      menupage = DXMODE;
      menuitem = 0;
      submenu = true;
      menu = true;
      if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
      BuildMenu();
    } else if (num == 13) {
      if (freq_in != 0) {
        if (TuneFreq(freq_in)) {
          if (XDRGTKUSB || XDRGTKTCP) {
            if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
          }
          if (!memorystore) {
            if (!memtune) radio.clearRDS();
            memtune = false;
            ShowFreq(0);
            store = true;
          }
        } else {
          ShowNum(freq_in);
          FrequencySprite.setTextDatum(TR_DATUM);
          FrequencySprite.fillSprite(BackgroundColor);
          FrequencySprite.setTextColor(SignificantColor, SignificantColorSmooth, false);
          FrequencySprite.drawString(String(freq_in) + " ", 218, -6, freqfont);
          FrequencySprite.pushSprite(46, 46);
          delay(1000);
          ShowFreq(0);

        }
      } else ShowFreq(0);
      freq_in = 0;
    } else {
      if (freq_in / 10000 == 0) freq_in = freq_in * 10 + num;
      ShowNum(freq_in);
    }
  }
}

void setAutoSpeedSPI() {
  switch (frequency / 10) {
    case 875 ... 877: tft.setSPISpeed(28); break;
    case 878 ... 881: tft.setSPISpeed(24); break;
    case 882 ... 892: tft.setSPISpeed(42); break;
    case 893 ... 899: tft.setSPISpeed(31); break;
    case 900 ... 904: tft.setSPISpeed(12); break;
    case 905 ... 906: tft.setSPISpeed(16); break;
    case 907 ... 910: tft.setSPISpeed(11); break;
    case 911 ... 916: tft.setSPISpeed(15); break;
    case 917 ... 921: tft.setSPISpeed(24); break;
    case 922 ... 928: tft.setSPISpeed(13); break;
    case 929: tft.setSPISpeed(11); break;
    case 930 ... 932: tft.setSPISpeed(13); break;
    case 933 ... 939: tft.setSPISpeed(18); break;
    case 940 ... 941: tft.setSPISpeed(12); break;
    case 942: tft.setSPISpeed(17); break;
    case 943 ... 949: tft.setSPISpeed(15); break;
    case 950: tft.setSPISpeed(20); break;
    case 951: tft.setSPISpeed(15); break;
    case 952 ... 960: tft.setSPISpeed(22); break;
    case 961 ... 965: tft.setSPISpeed(15); break;
    case 966 ... 973: tft.setSPISpeed(22); break;
    case 974 ... 979: tft.setSPISpeed(17); break;
    case 980 ... 982: tft.setSPISpeed(20); break;
    case 983 ... 987: tft.setSPISpeed(18); break;
    case 988 ... 993: tft.setSPISpeed(11); break;
    case 994 ... 996: tft.setSPISpeed(18); break;
    case 997 ... 1005: tft.setSPISpeed(11); break;
    case 1006: tft.setSPISpeed(13); break;
    case 1007 ... 1011: tft.setSPISpeed(11); break;
    case 1012 ... 1016: tft.setSPISpeed(18); break;
    case 1017 ... 1026: tft.setSPISpeed(13); break;
    case 1027 ... 1035: tft.setSPISpeed(23); break;
    case 1036 ... 1038: tft.setSPISpeed(15); break;
    case 1039 ... 1042: tft.setSPISpeed(12); break;
    case 1043 ... 1047: tft.setSPISpeed(23); break;
    case 1048 ... 1050: tft.setSPISpeed(28); break;
    case 1051 ... 1062: tft.setSPISpeed(15); break;
    case 1063 ... 1068: tft.setSPISpeed(18); break;
    case 1069 ... 1074: tft.setSPISpeed(14); break;
    case 1075: tft.setSPISpeed(17); break;
    case 1076 ... 1080: tft.setSPISpeed(15); break;
    default: tft.setSPISpeed(35); break;
  }
}

void endMenu() {
  if(rds_settings_changed) {
    radio.clearRDS();
    RDSstatus = false;
  }
  rds_settings_changed = menu = false;
  menuopen = false;
  LowLevelInit = true;
  submenu = false;
  menuoption = ITEM1;
  menupage = INDEX;
  menuitem = 0;
  saveData();
  if (af == 2) radio.rds.afreg = true; else radio.rds.afreg = false;
  if (wifi) remoteip = IPAddress(WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], subnetclient);

  Serial.flush();
  if (USBmode) Serial.updateBaudRate(19200); else Serial.updateBaudRate(115200);

  leave = true;
  if (language == LANGUAGE_CHS) PSSprite.setTextFont(3); else PSSprite.setTextFont(2);
  PSSprite.setTextDatum(TL_DATUM);
  BuildDisplay();
  SelectBand();
}

void cancelDXScan() {
  tunemode = scanmodeold;
  memorypos = memoryposold;
  scandxmode = false;
  if (scanmute) {
    radio.setUnMute();
    tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);

    if (!flashing) {
      tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
      tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor);
    }

    SQ = false;
    Squelchold = -2;
  }

  ShowTuneMode();
  ShowMemoryPos();
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("J0\n");
}

void MuteScreen(bool setting) {
  if (!setting && screenmute) {
    screenmute = false;
    setupmode = true;
    leave = true;
    tft.writecommand(0x11);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
    if (band < BAND_GAP) {
      if (afscreen) {
        BuildAFScreen();
        freq_in = 0;
      } else if (advancedRDS) {
        BuildAdvancedRDS();
        freq_in = 0;
      } else if (rdsstatscreen) {
        BuildRDSStatScreen();
        freq_in = 0;
      } else {
        BuildDisplay();
        freq_in = 0;
        SelectBand();
      }
    } else {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
    } setupmode = false;
  } else if (setting && !screenmute) {
    screenmute = true;
    analogWrite(CONTRASTPIN, 0);
    tft.writecommand(0x10);
  }
}
#pragma endregion

void DivdeSWMIBand() {
  if (frequency_AM >= FREQ_SW_160M_START && frequency_AM <= FREQ_SW_160M_END) {
    SWMIBandPos = SW_MI_BAND_160M;
    frequency_MIBand_160M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_120M_START && frequency_AM <= FREQ_SW_120M_END) {
    SWMIBandPos = SW_MI_BAND_120M;
    frequency_MIBand_120M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_90M_START && frequency_AM <= FREQ_SW_90M_END) {
    SWMIBandPos = SW_MI_BAND_90M;
    frequency_MIBand_90M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_75M_START && frequency_AM <= FREQ_SW_75M_END) {
    SWMIBandPos = SW_MI_BAND_75M;
    frequency_MIBand_75M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_60M_START && frequency_AM <= FREQ_SW_60M_END) {
    SWMIBandPos = SW_MI_BAND_60M;
    frequency_MIBand_60M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_49M_START && frequency_AM <= FREQ_SW_49M_END) {
    SWMIBandPos = SW_MI_BAND_49M;
    frequency_MIBand_49M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_41M_START && frequency_AM <= FREQ_SW_41M_END) {
    SWMIBandPos = SW_MI_BAND_41M;
    frequency_MIBand_41M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_31M_START && frequency_AM <= FREQ_SW_31M_END) {
    SWMIBandPos = SW_MI_BAND_31M;
    frequency_MIBand_31M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_25M_START && frequency_AM <= FREQ_SW_25M_END) {
    SWMIBandPos = SW_MI_BAND_25M;
    frequency_MIBand_25M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_22M_START && frequency_AM <= FREQ_SW_22M_END) {
    SWMIBandPos = SW_MI_BAND_22M;
    frequency_MIBand_22M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_19M_START && frequency_AM <= FREQ_SW_19M_END) {
    SWMIBandPos = SW_MI_BAND_19M;
    frequency_MIBand_19M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_16M_START && frequency_AM <= FREQ_SW_16M_END) {
    SWMIBandPos = SW_MI_BAND_16M;
    frequency_MIBand_16M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_15M_START && frequency_AM <= FREQ_SW_15M_END) {
    SWMIBandPos = SW_MI_BAND_15M;
    frequency_MIBand_15M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_13M_START && frequency_AM <= FREQ_SW_13M_END) {
    SWMIBandPos = SW_MI_BAND_13M;
    frequency_MIBand_13M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_11M_START && frequency_AM <= FREQ_SW_11M_END) {
    SWMIBandPos = SW_MI_BAND_11M;
    frequency_MIBand_11M = frequency_AM;
    SWMIBandstring = (String)"" + SWMIBandPos + " m";
  } else {
    if (SWMIBandPos > SW_MI_BAND_GAP) {
      SWMIBandPosold = SWMIBandPos;
      SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
    } SWMIBandPos = SW_MI_BAND_GAP;
  }
}

void ToggleSWMIBand(bool frequencyup) {
  switch (SWMIBandPosold) {
    case SW_MI_BAND_GAP:
      if (frequencyup) {
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        frequency_AM = FREQ_SW_160M_START;
      } else {
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        frequency_AM = FREQ_SW_11M_END;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_11M:
      if (frequencyup) {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_160M == 0 ? FREQ_SW_160M_START : frequency_MIBand_160M;
      } else {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_13M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_13M == 0 ? FREQ_SW_13M_END : frequency_MIBand_13M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_13M:
      if (frequencyup) {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_11M == 0 ? FREQ_SW_11M_START : frequency_MIBand_11M;
      } else {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_15M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_15M == 0 ? FREQ_SW_15M_END : frequency_MIBand_15M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_15M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_13M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_13M == 0 ? FREQ_SW_13M_START : frequency_MIBand_13M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_16M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_16M == 0 ? FREQ_SW_16M_END : frequency_MIBand_16M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_16M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_15M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_15M == 0 ? FREQ_SW_15M_START : frequency_MIBand_15M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_19M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_19M == 0 ? FREQ_SW_19M_END : frequency_MIBand_19M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_19M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_16M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_16M == 0 ? FREQ_SW_16M_START : frequency_MIBand_16M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_22M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_22M == 0 ? FREQ_SW_22M_END : frequency_MIBand_22M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_22M:
      if (frequencyup) {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_19M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_19M == 0 ? FREQ_SW_19M_START : frequency_MIBand_19M;
      } else {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_25M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_25M == 0 ? FREQ_SW_25M_END : frequency_MIBand_25M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_25M:
      if (frequencyup) {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_22M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_22M == 0 ? FREQ_SW_22M_START : frequency_MIBand_22M;
      } else {
        SWMIBandstringold = (String)"" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_31M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String)"" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_31M == 0 ? FREQ_SW_31M_END : frequency_MIBand_31M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_31M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_25M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_25M == 0 ? FREQ_SW_25M_START : frequency_MIBand_25M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_41M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_41M == 0 ? FREQ_SW_41M_END : frequency_MIBand_41M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_41M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_31M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_31M == 0 ? FREQ_SW_31M_START : frequency_MIBand_31M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_49M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_49M == 0 ? FREQ_SW_49M_END : frequency_MIBand_49M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_49M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_41M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_41M == 0 ? FREQ_SW_41M_START : frequency_MIBand_41M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_60M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_60M == 0 ? FREQ_SW_60M_END : frequency_MIBand_60M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_60M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_49M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_49M == 0 ? FREQ_SW_49M_START : frequency_MIBand_49M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_75M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_75M == 0 ? FREQ_SW_75M_END : frequency_MIBand_75M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_75M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_60M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_60M == 0 ? FREQ_SW_60M_START : frequency_MIBand_60M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_90M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_90M == 0 ? FREQ_SW_90M_END : frequency_MIBand_90M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_90M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_75M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_75M == 0 ? FREQ_SW_75M_START : frequency_MIBand_75M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_120M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_120M == 0 ? FREQ_SW_120M_END : frequency_MIBand_120M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_120M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_90M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_90M == 0 ? FREQ_SW_90M_START : frequency_MIBand_90M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_160M == 0 ? FREQ_SW_160M_END : frequency_MIBand_160M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_160M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_120M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_120M == 0 ? FREQ_SW_120M_START : frequency_MIBand_120M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_11M == 0 ? FREQ_SW_49M_END : frequency_MIBand_11M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
  }
}

void SelectBand() {
  if (afscreen || advancedRDS || rdsstatscreen) {
    BuildDisplay();
    freq_in = 0;
  }

  if (band > BAND_GAP) {
    if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
    if (tunemode == TUNE_MI_BAND && band != BAND_SW) tunemode = TUNE_MAN;
    BWreset = true;
    BWset = BWsetAM;
    switch (band) {
      case BAND_LW: freqold = frequency_LW; frequency_AM = frequency_LW; break;
      case BAND_MW: freqold = frequency_MW; frequency_AM = frequency_MW; break;
      case BAND_SW: freqold = frequency_SW; frequency_AM = frequency_SW; break;
    }
    LimitAMFrequency();
    if (!externaltune && tunemode != TUNE_MEM) CheckBandForbiddenAM();
    radio.SetFreqAM(frequency_AM);
    radio.setAMAttenuation(amgain);
    radio.setAMCoChannel(amcodect, amcodectcount);
    doBW();
    if (!screenmute) {
      if (radio.rds.region == 0) tftPrint16(ALEFT, "PI", 212, 193, GreyoutColor, BackgroundColor);
      else {
        tftPrint16(ALEFT, "PI", 212, 184, GreyoutColor, BackgroundColor);
        tftPrint16(ALEFT, "ID", 212, 201, GreyoutColor, BackgroundColor);
      }

      tftPrint16(ALEFT, "PS", 3, 193, GreyoutColor, BackgroundColor);
      tftPrint16(ALEFT, "RT", 3, 221, GreyoutColor, BackgroundColor);
      tftPrint16(ALEFT, "PTY", 3, 163, GreyoutColor, BackgroundColor);
      tft.drawBitmap(68, 5, RDSLogo, 35, 22, GreyoutColor);
      tft.fillRoundRect(249, 57, 30, 18, 2, GreyoutColor);
      tftPrint16(ACENTER, "iMS", 265, 59, BackgroundColor, GreyoutColor);
      tft.fillRoundRect(287, 57, 30, 18, 2, GreyoutColor);
      tftPrint16(ACENTER, "EQ", 301, 59, BackgroundColor, GreyoutColor);
      tftReplace(ALEFT, "MHz", "kHz", 258, 76, ActiveColor, ActiveColorSmooth, BackgroundColor, 28);
    }
  } else {
    if (tunemode == TUNE_MI_BAND) tunemode = TUNE_MAN;

    if (!leave) {
      if (band == BAND_FM) radio.SetFreq(frequency);
      if (band == BAND_OIRT) radio.SetFreq(frequency_OIRT);
    }

    BWreset = true;
    BWset = BWsetFM;
    freqold = frequency_AM;
    if (!externaltune && tunemode != TUNE_MEM) CheckBandForbiddenFM();
    doBW();
    if (radio.rds.region == 0) tftPrint16(ALEFT, "PI", 212, 193, ActiveColor, ActiveColorSmooth);
    else {
      tftPrint16(ALEFT, "PI", 212, 184, ActiveColor, ActiveColorSmooth);
      tftPrint16(ALEFT, "ID", 212, 201, ActiveColor, ActiveColorSmooth);
    }
    tftPrint16(ALEFT, "PS", 3, 193, ActiveColor, ActiveColorSmooth);
    tftPrint16(ALEFT, "RT", 3, 221, ActiveColor, ActiveColorSmooth);
    tftPrint16(ALEFT, "PTY", 3, 163, ActiveColor, ActiveColorSmooth);

    tftReplace(ALEFT, "kHz", "MHz", 258, 76, ActiveColor, ActiveColorSmooth, BackgroundColor, 28);
  }

  if (!leave) radio.clearRDS();
  ShowFreq(0);

  if (!screenmute) {
    tft.fillRect(113, 38, 124, 4, BackgroundColor);
    ShowErrors();
    showPTY();
    showRadioText();
    showPI();
    updateiMS();
    updateEQ();
    ShowTuneMode();
    ShowStepSize();

    tftPrint(ALEFT, textUI(99), 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(ALEFT, textUI(100), 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(ALEFT, textUI(101), 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(ALEFT, textUI(102), 70, 32, BackgroundColor, BackgroundColor, 16);
    tftPrint(ALEFT, textUI(103), 70, 32, BackgroundColor, BackgroundColor, 16);

    switch (band) {
      case BAND_LW: tftPrint(ALEFT, textUI(99), 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_MW: tftPrint(ALEFT, textUI(100), 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_SW: tftPrint(ALEFT, textUI(101), 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_FM: tftPrint(ALEFT, textUI(102), 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
      case BAND_OIRT: tftPrint(ALEFT, textUI(103), 70, 32, (bandforbidden ? GreyoutColor : PrimaryColor), (bandforbidden ? BackgroundColor : PrimaryColorSmooth), 16); break;
    }
  }
  leave = false;
}

void doTuneMode() {
  switch (tunemode) {
    case TUNE_MAN:
      if (band == BAND_SW) {
        if (showSWMIBand && nowToggleSWMIBand) tunemode = TUNE_MI_BAND;
        else tunemode = TUNE_AUTO;
      } else tunemode = TUNE_AUTO;
      break;
    case TUNE_MI_BAND:
    case TUNE_AUTO:
      tunemode = TUNE_MEM;
      break;
    case TUNE_MEM:
      if (!bandforbidden) tunemode = TUNE_MAN;
      break;
  }
  ShowTuneMode();
  ShowMemoryPos();
  EEPROM.writeByte(EE_BYTE_TUNEMODE, tunemode);
  EEPROM.commit();
}

void RoundStep() {
  if (band == BAND_FM) {
    unsigned int freq = frequency;
    switch (fmdefaultstepsize) {
      case 0: Round50K(freq); break;
      case 1: Round100K(freq); break;
      case 2: Round200K(freq); break;
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    Round30K(frequency_OIRT);
    radio.SetFreq(frequency_OIRT);
  } else {
    if (band == BAND_LW) {
      unsigned int freq = frequency_AM / (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_AM = freq * (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_LW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    } else if (band == BAND_MW) {
      unsigned int freq = frequency_AM / (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_AM = freq * (mwstepsize == false ? FREQ_MW_STEP_9K : FREQ_MW_STEP_10K);
      frequency_MW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    } else if (band == BAND_SW) {
      Round5K(frequency_AM);
      frequency_SW = frequency_AM;
      radio.SetFreqAM(frequency_AM);
    }
  } StoreFrequency();
  while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
}

void ButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    ShowFreq(5);
    ShowFreq(0);
  } else {
    if (!usesquelch) radio.setUnMute();
    if (advancedRDS) {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
    }
    if (!BWtune && !menu) {
      if (tunemode == TUNE_MEM) {
        memorystore = !memorystore;
        if (!memorystore) {
          if (!IsStationEmpty()) memoryposstatus = MEM_EXIST;
          else memoryposstatus = MEM_NORMAL;
          ShowMemoryPos();
          ShowTuneMode();
        } else {
          StoreMemoryPos(memorypos);
          ShowTuneMode();
          if (memoryposstatus == MEM_DARK || memoryposstatus == MEM_EXIST) {
            memoryposstatus = MEM_NORMAL;
            ShowMemoryPos();
          }
        }
      } else {
        if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        unsigned long counterold = millis();
        unsigned long counter = counterold;
        while (digitalRead(ROTARY_BUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold < 1000) {
          if (tunemode == TUNE_MAN) {
            stepsize++;
            if((band == BAND_SW || band < BAND_GAP) && stepsize > 4) stepsize = 0;
            else if(stepsize > 3) stepsize = 0;

            if (!screenmute) ShowStepSize();

            EEPROM.writeByte(EE_BYTE_STEPSIZE, stepsize);
            EEPROM.commit();
            if (stepsize == 0) {
              RoundStep();
              ShowFreq(0);
            }
          }
        } else {
          if (band < BAND_GAP) {
            switch (addRowToCSV()) {
              case 0: ShowFreq(2); break;
              case 1: ShowFreq(3); break;
              case 2: ShowFreq(4); break;
            }
            delay(DELAY_UI_UPDATE_MS);
            EdgeBeeper();
            while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
            ShowFreq(0);
          } else {
            if (band == BAND_SW && tunemode != TUNE_MEM) {
              nowToggleSWMIBand = !nowToggleSWMIBand;
              tunemode = TUNE_MAN;
              EEPROM.writeByte(EE_BYTE_BANDAUTOSW, nowToggleSWMIBand);
              EEPROM.commit();
              if (!screenmute) {
                tftPrint16(ACENTER, "AUTO", 22, 60, BackgroundColor, BackgroundColor);
                tftPrint16(ACENTER, "BAND", 22, 60, BackgroundColor, BackgroundColor);
              }
              doTuneMode();
              ShowTuneMode();
            }
          }
        }
      }
      screensavertimer = millis();
    } else {
      if (menu) DoMenu();
      if (BWtune) {
        if (BWsettemp == 18 || BWsettemp == 19) {
          if (BWsettemp == 18) iMSset = !iMSset;
          if (BWsettemp == 19) EQset = !EQset;
          if(!iMSset && !EQset) iMSEQ = 0;
          else if(iMSset && EQset) iMSEQ = 2;
          else if(!iMSset && EQset) iMSEQ = 3;
          else iMSEQ = 4;
          EEPROM.writeByte(EE_BYTE_IMSSET, iMSset);
          EEPROM.writeByte(EE_BYTE_EQSET, EQset);
          EEPROM.commit();
          updateiMS();
          updateEQ();
          if (XDRGTKUSB || XDRGTKTCP) DataPrint("G" + String(!EQset) + String(!iMSset) + "\n");
          showBWSelector();
          if (band < BAND_GAP) drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          else drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
        } else if (BWsettemp == 20) {
          BuildDisplay();
          freq_in = 0;
          SelectBand();
        } else {
          doBW();
          showBWSelector();
          if (band < BAND_GAP) drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
          else drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
        }
      }
    }
  }
  while (digitalRead(ROTARY_BUTTON) == LOW) delay(50);
}

void ModeButtonPress() {
  if (seek) radio.setUnMute();
  seek = false;
  if (scandxmode) {
    ShowFreq(5);
    ShowFreq(0);
  } else {
    if (!usesquelch) radio.setUnMute();
    if (advancedRDS) {
      BuildDisplay();
      freq_in = 0;
      SelectBand();
      screensavertimer = millis();
    } else if (afscreen) {
      if (afpagenr == 1) afpagenr = 2; else if (afpagenr == 2 && afpage) afpagenr = 3; else afpagenr = 1;
      BuildAFScreen();
      freq_in = 0;
    } else if (rdsstatscreen) {
      BuildAFScreen();
      freq_in = 0;
    } else {
      if (!BWtune && !menu) {
        if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        memorystore = false;
        unsigned long counterold = millis();
        unsigned long counter = counterold;
        while (digitalRead(MODEBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold <= 1000) doTuneMode();
        else {
          if (!BWtune && !menu) {
            menuoption = ITEM1;
            menupage = INDEX;
            menuitem = 0;
            if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
            BuildMenu();
            freq_in = 0;
            menu = true;
          }
        }
      } else {
        if (menuopen && !submenu) ButtonPress();
        else if (!submenu) {
          OStatusold = 1000;
          Stereostatusold = false;
          SStatusold = 2000;
          BWOld = 0;
          if (frequency % 10 != 0) {
            if (fmdefaultstepsize == 1) Round100K(frequency); else Round50K(frequency);
            EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, frequency);
          }
          CheckBandForbiddenFM();
          CheckBandForbiddenAM();
          if (!usesquelch) radio.setUnMute();
          MWLowEdgeSet = mwstepsize == false ? FREQ_MW_LOW_EDGE_MIN_9K : FREQ_MW_LOW_EDGE_MIN_10K;
          MWHighEdgeSet = mwstepsize == false ? FREQ_MW_HIGH_EDGE_MAX_9K : FREQ_MW_HIGH_EDGE_MAX_10K;
          doBandSelectionFM();
          doBandSelectionAM();
          screensavertimer = millis();
          endMenu();
        } else {
          if (submenu) {
            if (menuopen) {
              if (menupage == CONNECTIVITY && menuoption == ITEM2 && wifi) {
                tryWiFi();
                delay(1750);
              }
              if (menupage == DISPLAYSETTINGS && menuoption == ITEM5) doTheme();
              menuopen = false;
            }
            submenu = false;
            menuoption = ITEM1;
            menupage = INDEX;
            menuitem = 0;
            if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
            BuildMenu();
            freq_in = 0;
          }
        }
      }
    }
  }
  while (digitalRead(MODEBUTTON) == LOW) delay(50);
  delay(50);
}

void KeyUp() {
  rotary = 0;
  if (scandxmode) {
    ShowFreq(5);
    ShowFreq(0);
  } else {
    if (!afscreen && !rdsstatscreen) {
      if (!BWtune && !menu) {
        switch (tunemode) {
          case TUNE_MAN:
            TuneUp();
            break;
          case TUNE_AUTO:
            direction = true;
            seek = true;
            seekinit = true;
            Seek(direction);
            break;
          case TUNE_MEM:
            memorypos++;
            if (memorypos > EE_PRESETS_CNT - 1) memorypos = 0;
            if (!memorystore) {
              while (IsStationEmpty()) {
                memorypos++;
                if (memorypos > EE_PRESETS_CNT - 1) {
                  memorypos = 0;
                  break;
                }
              }
            }
            if (!memorystore) DoMemoryPosTune();
            else {
              if (!IsStationEmpty()) memoryposstatus = MEM_EXIST; else memoryposstatus = MEM_DARK;
            }
            ShowMemoryPos();
            EEPROM.writeByte(EE_BYTE_MEMORYPOS, memorypos);
            EEPROM.commit();
            break;
          case TUNE_MI_BAND:
            if (showSWMIBand) {
              if (displayflip) ToggleSWMIBand(true);
              else ToggleSWMIBand(false);
            }
            break;
        }
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS();
          memtune = false;
          ShowFreq(0);
          store = true;
        }
      } else MenuUpDown(true);
    }
  }
}

void KeyDown() {
  rotary = 0;
  if (scandxmode) {
    ShowFreq(5);
    ShowFreq(0);
  } else {
    if (!afscreen && !rdsstatscreen) {
      if (!BWtune && !menu) {
        switch (tunemode) {
          case TUNE_MAN:
            TuneDown();
            break;
          case TUNE_AUTO:
            direction = false;
            seek = true;
            seekinit = true;
            Seek(direction);
            break;
          case TUNE_MEM:
            memorypos--;
            if (memorypos > EE_PRESETS_CNT - 1) memorypos = EE_PRESETS_CNT - 1;
            if (!memorystore) {
              while (IsStationEmpty()) {
                memorypos--;
                if (memorypos > EE_PRESETS_CNT - 1) {
                  memorypos = EE_PRESETS_CNT - 1;
                  break;
                }
              }
            }
            if (!memorystore) DoMemoryPosTune();
            else {
              if (!IsStationEmpty()) memoryposstatus = MEM_EXIST; else memoryposstatus = MEM_DARK;
            }
            ShowMemoryPos();
            EEPROM.writeByte(EE_BYTE_MEMORYPOS, memorypos);
            EEPROM.commit();
            break;
          case TUNE_MI_BAND:
            if (showSWMIBand) {
              if (displayflip) ToggleSWMIBand(false);
              else ToggleSWMIBand(true);
            } break;
        }
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS();
          memtune = false;
          ShowFreq(0);
          store = true;
        }
      } else MenuUpDown(false);
    }
  }
}

void ShowMemoryPos() {
  if (tunemode == TUNE_MEM) {
    int memposcolor = 0;
    int memposcolorsmooth = 0;

    if (!memorystore) {
      if (IsStationEmpty()) memoryposstatus = MEM_DARK; else memoryposstatus = MEM_NORMAL;
    }

    switch (memoryposstatus) {
      case MEM_DARK:
        memposcolor = InsignificantColor;
        memposcolorsmooth = InsignificantColorSmooth;
        break;
      case MEM_NORMAL:
        memposcolor = PrimaryColor;
        memposcolorsmooth = PrimaryColorSmooth;
        break;
      case MEM_EXIST:
        memposcolor = SignificantColor;
        memposcolorsmooth = SignificantColorSmooth;
        break;
    }
    if (advancedRDS) tftReplace(ALEFT, String(memoryposold + 1), String(memorypos + 1), 215, 36, SecondaryColor, SecondaryColorSmooth, BackgroundColor, 16);
    else tftReplace(ALEFT, String(memoryposold + 1), String(memorypos + 1), 50, 32, memposcolor, memposcolorsmooth, BackgroundColor, 16);
    memoryposold = memorypos;
  } else {
    if (advancedRDS) tftPrint16(ALEFT, String(memorypos + 1), 215, 36, BackgroundColor, BackgroundColor);
    else tftPrint16(ALEFT, String(memorypos + 1), 50, 32, BackgroundColor, BackgroundColor);
  }
}

void DoMemoryPosTune() {
  radio.clearRDS();

  if (IsStationEmpty()) {
    memoryposstatus = MEM_DARK;
    return;
  } else memoryposstatus = MEM_NORMAL;

  if (band != presets[memorypos].band) {
    band = presets[memorypos].band;
    SelectBand();
  } else band = presets[memorypos].band;

  switch (band) {
    case BAND_FM:
      frequency = presets[memorypos].frequency;
      radio.SetFreq(frequency);
      break;
    case BAND_OIRT:
      frequency_OIRT = presets[memorypos].frequency;
      radio.SetFreq(frequency_OIRT);
      break;
    case BAND_LW:
      frequency_LW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_LW);
      break;
    case BAND_MW:
      frequency_MW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_MW);
      break;
    case BAND_SW:
      frequency_SW = presets[memorypos].frequency;
      radio.SetFreqAM(frequency_SW);
      break;
  }

  if (band == BAND_FM || band == BAND_OIRT) {
    StereoToggle = presets[memorypos].ms;
    if (!StereoToggle) {
      Stereostatusold = false;
      tft.drawBitmap(32, 5, Stereo, 32, 22, BackgroundColor);
      tft.drawBitmap(38, 5, Mono, 22, 22, SecondaryColor);
    } else {
      Stereostatusold = false;
      tft.drawBitmap(38, 5, Mono, 22, 22, BackgroundColor);
      tft.drawBitmap(32, 5, Stereo, 32, 22, GreyoutColor);
    }
  }

  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
  if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");

  String stationName = "";
  if (presets[memorypos].RDSPS[0] != '\0') {
    for (byte i = 0; i < 9; i++) stationName += presets[memorypos].RDSPS[i];
  }

  if (presets[memorypos].RDSPI[0] != '\0') {
    for (byte i = 0; i < 4; i++) radio.rds.picode[i] = presets[memorypos].RDSPI[i];
    radio.rds.picode[4] = ' ';
    radio.rds.picode[5] = ' ';
    radio.rds.picode[6] = '\0';
    radio.rds.correctPI = (uint16_t)strtol(radio.rds.picode, NULL, 16);
  } else {
    for (byte i = 0; i < 6; i++) radio.rds.picode[i] = '\0';
  }

  radio.rds.stationName = stationName;

  BWset = presets[memorypos].bw;
  doBW();
  memtune = true;
  memreset = true;
  rdsflagreset = false;
  findMemoryAF = true;
  ShowFreq(0);
}

void ShowFreq(int mode) {
  if (!setupmode) {
    if (band != BAND_FM && band != BAND_OIRT) {
      if (freqold < 2000 && frequency_AM >= 2000 && stepsize == 0) if (frequency_AM != 27000 && freqold != 144) radio.SetFreqAM(2000);
      if (freqold >= 2000 && frequency_AM < 2000 && stepsize == 0) if (frequency_AM != 144 && freqold != 27000) radio.SetFreqAM(1998);
    }
  }

  if (band > BAND_GAP) {
    switch (band) {
      case BAND_LW: frequency_AM = frequency_LW; break;
      case BAND_MW: frequency_AM = frequency_MW; break;
      case BAND_SW: frequency_AM = frequency_SW; break;
    }

    if (!screenmute) {
      FrequencySprite.fillSprite(BackgroundColor);
      FrequencySprite.setTextDatum(TR_DATUM);
      FrequencySprite.setTextColor(FreqColor, FreqColorSmooth, false);
      FrequencySprite.drawString(String(frequency_AM) + " ", 218, -6, freqfont);
      FrequencySprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
      FrequencySprite.setTextDatum(TL_DATUM);

      if (band == BAND_SW && showSWMIBand) {
        DivdeSWMIBand();
        updateSWMIBand();
      }

      FrequencySprite.pushSprite(46, 46);
    }

    freqold = frequency_AM;
  } else {
    unsigned int freq = 0;
    if (band == BAND_FM) freq = frequency + ConverterSet * 100;
    if (band == BAND_OIRT) freq = frequency_OIRT;

    if (!screenmute) {
      if (advancedRDS) {
        for (int i = 0; i < 33; i++) tft.fillCircle((6 * i) + 10, 133, 2, GreyoutColor);
        tftReplace(ARIGHT, String(freqold / 100) + "." + (freqold % 100 < 10 ? "0" : "") + String(freqold % 100), String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100), 273, 35, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
        freqold = freq;
      } else if (afscreen) {
        tftReplace(ARIGHT, String(freqold / 100) + "." + (freqold % 100 < 10 ? "0" : "") + String(freqold % 100) + " MHz", String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100), 290, 201, BWAutoColor, BWAutoColorSmooth, BackgroundColor, 16);
        freqold = freq;
      } else {
        FrequencySprite.fillSprite(BackgroundColor);

        if (mode == 0) {
          FrequencySprite.setTextDatum(TR_DATUM);
          FrequencySprite.setTextColor(FreqColor, FreqColorSmooth, false);
        } else {
          FrequencySprite.setTextDatum(TC_DATUM);
          FrequencySprite.setTextColor(ActiveColor, ActiveColorSmooth, false);
        }

        switch (mode) {
          case 0:
            FrequencySprite.drawString(String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100) + " ", 218, -6, freqfont);
            freqold = freq;
            break;
          case 1: Infoboxprint(textUI(31)); break;
          case 2: Infoboxprint(textUI(286)); break;
          case 3: Infoboxprint(textUI(287)); break;
          case 4: Infoboxprint(textUI(291)); break;
          case 5: Infoboxprint(textUI(280)); break;
        }

        FrequencySprite.pushSprite(46, 46);
        if (mode == 5) delay(1000);
      }
    }
  }

  rdsreset = true;
  afmethodBold = false;
  aid_counterold = 0;
  dropout = false;

  if (wifi) {
    Udp.beginPacket(remoteip, 9030);
    if (band == BAND_FM) Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency) + "0000");
    else if (band == BAND_OIRT) Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency_OIRT) + "0000");
    else Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";freq=" + String(frequency_AM) + "000");
    Udp.endPacket();
  }
  tuningtimer = millis();

  if (!rdsflagreset && !screenmute && !afscreen && !rdsstatscreen) {
    ShowRDSLogo(false);
    if (!advancedRDS) {
      FullLineSprite.fillSprite(BackgroundColor);
      FullLineSprite.drawLine(283, 0, 283, 19, FrameColor);
      FullLineSprite.pushSprite(36, 220);
      tft.fillCircle(314, 223, 2, GreyoutColor);
      tft.fillCircle(314, 234, 2, GreyoutColor);
    } else {
      GeneralTextSprite.fillSprite(TFT_TRANSPARENT);
      GeneralTextSprite.fillRect(0, 0, 165, 19, BackgroundColor);
      GeneralTextSprite.pushSprite(36, 220, TFT_TRANSPARENT);
      tft.fillCircle(314, 223, 2, GreyoutColor);
      tft.fillCircle(314, 234, 2, GreyoutColor);
    }
    rdsflagreset = true;
  }
}

void doBW() {
  if (BWtune && !bwtouchtune) BWset = BWsettemp;

  if (band < BAND_GAP) {
    if (BWset > 16) BWset = 0;

    switch (BWset) {
      case 1: radio.setFMBandw(56); break;
      case 2: radio.setFMBandw(64); break;
      case 3: radio.setFMBandw(72); break;
      case 4: radio.setFMBandw(84); break;
      case 5: radio.setFMBandw(97); break;
      case 6: radio.setFMBandw(114); break;
      case 7: radio.setFMBandw(133); break;
      case 8: radio.setFMBandw(151); break;
      case 9: radio.setFMBandw(168); break;
      case 10: radio.setFMBandw(184); break;
      case 11: radio.setFMBandw(200); break;
      case 12: radio.setFMBandw(217); break;
      case 13: radio.setFMBandw(236); break;
      case 14: radio.setFMBandw(254); break;
      case 15: radio.setFMBandw(287); break;
      case 16: radio.setFMBandw(311); break;
    }
    BWsetFM = BWset;
    EEPROM.writeByte(EE_BYTE_BWSET_FM, BWsetFM);

    if ((XDRGTKUSB || XDRGTKTCP) && !XDRScan) {
      switch (BWset) {
        case 0: DataPrint("W0\n"); break;
        case 1: DataPrint("W56000\n"); break;
        case 2: DataPrint("W64000\n"); break;
        case 3: DataPrint("W72000\n"); break;
        case 4: DataPrint("W84000\n"); break;
        case 5: DataPrint("W97000\n"); break;
        case 6: DataPrint("W114000\n"); break;
        case 7: DataPrint("W133000\n"); break;
        case 8: DataPrint("W151000\n"); break;
        case 9: DataPrint("W168000\n"); break;
        case 10: DataPrint("W184000\n"); break;
        case 11: DataPrint("W200000\n"); break;
        case 12: DataPrint("W217000\n"); break;
        case 13: DataPrint("W236000\n"); break;
        case 14: DataPrint("W254000\n"); break;
        case 15: DataPrint("W287000\n"); break;
        case 16: DataPrint("W311000\n"); break;
      }
    }
  } else {
    if (BWset > 4) BWset = 1;

    switch (BWset) {
      case 1: radio.setAMBandw(3); break;
      case 2: radio.setAMBandw(4); break;
      case 3: radio.setAMBandw(6); break;
      case 4: radio.setAMBandw(8); break;
    }
    BWsetAM = BWset;
    EEPROM.writeByte(EE_BYTE_BWSET_AM, BWsetAM);
  }
  updateBW();
  BWreset = true;
  EEPROM.commit();
}

void ShowTuneMode() {
  if (!screenmute) {
    switch (tunemode) {
      case TUNE_MAN:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint16(ACENTER, "AUTO", 22, 60, GreyoutColor, GreyoutColor);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint16(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor);
        } else {
          tftPrint16(ACENTER, "BAND", 22, 60, GreyoutColor, GreyoutColor);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint16(ACENTER, "AUTO", 22, 60, BackgroundColor, GreyoutColor);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, SecondaryColor);
        tftPrint16(ACENTER, "MAN", 22, 38, BackgroundColor, SecondaryColor);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor);
        break;

      case TUNE_AUTO:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint16(ACENTER, "AUTO", 22, 60, BackgroundColor, SecondaryColor);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor);
        break;

      case TUNE_MEM:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint16(ACENTER, "AUTO", 22, 60, GreyoutColor, GreyoutColor);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint16(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor);
        } else {
          tftPrint16(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint16(ACENTER, "AUTO", 22, 60, BackgroundColor, GreyoutColor);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor);

        if (memorystore) {
          tft.fillRoundRect(2, 80, 40, 18, 2, SignificantColor);
          tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, SignificantColor);
        } else {
          tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
          tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor);
        }
        break;

      case TUNE_MI_BAND:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint16(ACENTER, "BAND", 22, 60, BackgroundColor, SecondaryColor);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor);
        break;
    }
  }
}

void TuneUp() {
  aftest = true;
  aftimer = millis();
  unsigned int temp = 0;
  if (stepsize == 0) {
    if (band > BAND_GAP) {
      if (frequency_AM <= MWHighEdgeSet && frequency_AM >= MWLowEdgeSet) {
        if (!mwstepsize) {
          temp = FREQ_MW_STEP_9K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
        } else if (mwstepsize) {
          temp = FREQ_MW_STEP_10K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_10K) * FREQ_MW_STEP_10K;
        }
      } else if (frequency_AM < MWLowEdgeSet) {
        temp = FREQ_MW_STEP_9K;
        frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
      } else if (frequency_AM <= SWHighEdgeSet && frequency_AM >= SWLowEdgeSet) {
        temp = FREQ_SW_STEP_5K;
        frequency_AM = (frequency_AM / FREQ_SW_STEP_5K) * FREQ_SW_STEP_5K;
      }
    } else {
      if (band == BAND_OIRT) temp = FREQ_OIRT_STEP_30K;
      else {
        switch (fmdefaultstepsize) {
          case 0: temp = FREQ_FM_STEP_50K; break;
          case 1: temp = FREQ_FM_STEP_100K; break;
          case 2: temp = FREQ_FM_STEP_200K; break;
        }
      }
    }
  }
  if(stepsize == 1) temp = 1;
  else if(stepsize == 2) temp = 10;
  else if(stepsize == 3) temp = 100;
  else if(stepsize == 4) temp = 1000;
  if(rotaryaccelerate && rotarycounter > 2) temp *= 2;

  if (band == BAND_FM) {
    frequency += temp;
    if (scandxmode) {
      while (IsFrequencyUsed(frequency) || presets[memorypos].band != BAND_FM) frequency += temp;
    }

    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8795) frequency = 8790;
    if (frequency >= (HighEdgeSet * 10) + 1) {
      frequency = LowEdgeSet * 10;
      if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8750) frequency = 8775;
      EdgeBeeper();
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    frequency_OIRT += temp;
    if (scandxmode) {
      while (IsFrequencyUsed(frequency_OIRT) || presets[memorypos].band != BAND_OIRT) frequency_OIRT += temp;
    }
    if (frequency_OIRT > HighEdgeOIRTSet) {
      frequency_OIRT = LowEdgeOIRTSet;
      EdgeBeeper();
    }
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    frequency_AM += temp;
    if (frequency_AM > LWHighEdgeSet) {
      frequency_AM = LWLowEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_LW = frequency_AM;
  } else if (band == BAND_MW) {
    frequency_AM += temp;
    if (frequency_AM > MWHighEdgeSet) {
      frequency_AM = MWLowEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_MW = frequency_AM;
  } else if (band == BAND_SW) {
    if (rotaryaccelerate && rotarycounter > 2) temp *= 2;
    frequency_AM += temp;
    if (frequency_AM > SWHighEdgeSet) {
      frequency_AM = SWLowEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_SW = frequency_AM;
  }
  radio.clearRDS();
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void TuneDown() {
  aftest = true;
  aftimer = millis();
  unsigned int temp = 0;
  if (stepsize == 0) {
    if (band > BAND_GAP) {
      if (frequency_AM < MWHighEdgeSet && frequency_AM > MWLowEdgeSet) {
        if (!mwstepsize) {
          temp = FREQ_MW_STEP_9K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
        } else if (mwstepsize) {
          temp = FREQ_MW_STEP_10K;
          frequency_AM = (frequency_AM / FREQ_MW_STEP_10K) * FREQ_MW_STEP_10K;
        }
      } else if (frequency_AM < MWLowEdgeSet) {
        temp = FREQ_MW_STEP_9K;
        frequency_AM = (frequency_AM / FREQ_MW_STEP_9K) * FREQ_MW_STEP_9K;
      } else {
        temp = FREQ_SW_STEP_5K;
        frequency_AM = (frequency_AM / FREQ_SW_STEP_5K) * FREQ_SW_STEP_5K;
      }
    } else {
      if (band == BAND_OIRT) temp = FREQ_OIRT_STEP_30K;
      else {
        switch (fmdefaultstepsize) {
          case 0: temp = FREQ_FM_STEP_50K; break;
          case 1: temp = FREQ_FM_STEP_100K; break;
          case 2: temp = FREQ_FM_STEP_200K; break;
        }
      }
    }
  }
  if(stepsize == 1) temp = 1;
  else if(stepsize == 2) temp = 10;
  else if(stepsize == 3) temp = 100;
  else if(stepsize == 4) temp = 1000;
  if (rotaryaccelerate && rotarycounter > 2) temp *= 2;

  if (band == BAND_FM) {
    frequency -= temp;
    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8770) frequency = 8775;
    if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 8755) frequency = 10790;

    if (frequency < LowEdgeSet * 10) {
      frequency = HighEdgeSet * 10;
      if (fmdefaultstepsize == 2 && stepsize == 0 && frequency == 10800) frequency = 10790;
      EdgeBeeper();
    }
    radio.SetFreq(frequency);
  } else if (band == BAND_OIRT) {
    frequency_OIRT -= temp;
    if (frequency_OIRT < LowEdgeOIRTSet) {
      frequency_OIRT = HighEdgeOIRTSet;
      EdgeBeeper();
    }
    radio.SetFreq(frequency_OIRT);
  } else if (band == BAND_LW) {
    frequency_AM -= temp;
    if (frequency_AM < LWLowEdgeSet) {
      frequency_AM = LWHighEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_LW = frequency_AM;
  } else if (band == BAND_MW) {
    frequency_AM -= temp;
    if (frequency_AM < MWLowEdgeSet) {
      frequency_AM = MWHighEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_MW = frequency_AM;
  } else if (band == BAND_SW) {
    if (rotaryaccelerate && rotarycounter > 2) temp *= 2;
    frequency_AM -= temp;
    if (frequency_AM < SWLowEdgeSet) {
      frequency_AM = SWHighEdgeSet;
      EdgeBeeper();
    }
    radio.SetFreqAM(frequency_AM);
    frequency_SW = frequency_AM;
  }
  radio.clearRDS();
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void Seek(bool mode) {
  radio.setMute();
  if (seekinit) {
    if (stepsize != 0) {
      stepsize = 0;
      RoundStep();
      ShowStepSize();
    }
    seekinit = false;
  }

  if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
  if (!mode) TuneDown(); else TuneUp();
  delay(50);

  ShowFreq(0);
  if (XDRGTKUSB || XDRGTKTCP) {
    if(band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if(band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
  }

  if (band < BAND_GAP) {
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus);
    if (isSignalQualityGood(USN, WAM, OStatus) && (!usesquelch || (Squelch < SStatus || Squelch == SQUELCH_MAX_VALUE))) {
      seek = false;
      radio.setUnMute();
      if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
      store = true;
    } else {
      seek = true;
      if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
      if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
    }
  } else {
    radio.getStatusAM(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus);
    if ((USN < amscansens * 30) && (OStatus < 2 && OStatus > -2) && (!usesquelch || (Squelch < SStatus || Squelch == 920))) {
      seek = false;
      radio.setUnMute();
      if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
      store = true;
    } else seek = true;
  }
}

uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi) {
  uint8_t error = 0, counter = 0, percent = 0, percentold = 0;
  uint8_t rangestart = startmem;
  uint16_t _current = frequency;
  uint16_t totalIterations = (stopfreq - startfreq) + 1;
  uint16_t currentIteration = 0;
  bool stopScanning = false;
  bool dostore = false;

  radio.setMute();
  radio.power(false);
  band = BAND_FM;
  tunemode = TUNE_MAN;

  tft.drawRect(59, 109, 202, 8, FrameColor);
  tft.fillRect(60, 110, 200, 6, GreyoutColor);
  tftPrint(ARIGHT, textUI(268), 120, 155, ActiveColor, ActiveColorSmooth, 16);

  for(frequency = startfreq * 10; frequency <= stopfreq * 10; frequency += 10) {
    if (stopScanning) break;

    currentIteration++;
    percent = (currentIteration * 100) / totalIterations;

    radio.SetFreq(frequency);
    radio.clearRDS();
    delay(DELAY_TUNE_MS);
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus);
    if (isSignalQualityGood(USN, WAM, OStatus)) {
      for (byte y = 0; y < 20; y++) {
        delay(DELAY_RDS_READ_MS);
        radio.readRDS(showrdserrors);
      }

      dostore = true;
      if (doublepi != 0) {
        for (byte x = (doublepi == 1 ? rangestart : 0); x <= (doublepi == 1 ? stopmem : EE_PRESETS_CNT - 1); x++) {
          if (presets[x].RDSPI[0] != '\0') {
            bool allMatch = true;

            for (byte i = 0; i < 4; i++) {
              if (presets[x].RDSPI[i] != radio.rds.picode[i]) {
                allMatch = false;
                break;
              }
            }

            if (allMatch) {
              dostore = false;
              break;
            }
          }
        }
      }

      if (((rdsonly && radio.rds.hasRDS) || !rdsonly) && dostore) {
        StoreMemoryPos(startmem);
        counter ++;
        startmem++;
        if (startmem > stopmem) {
          error = 1;
          stopScanning = true;
          break;
        }
        dostore = false;
      }
    }

    if (percent != percentold) {
      switch (CurrentTheme) {
        case 7:
          SignalSprite.pushImage(-107, -95, 292, 170, popupbackground_wo);
          SquelchSprite.pushImage(-177, -135, 292, 170, popupbackground_wo);
          break;
        default:
          SignalSprite.pushImage(-107, -95, 292, 170, popupbackground);
          SquelchSprite.pushImage(-177, -135, 292, 170, popupbackground);
          break;
      }

      SignalSprite.setTextColor(SecondaryColor, SecondaryColorSmooth, false);
      SignalSprite.setTextDatum(TC_DATUM);
      SignalSprite.drawString(String(percent) + "%", 40, 0);
      SignalSprite.pushSprite(120, 125);

      SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
      SquelchSprite.drawString(String(counter), 0, 0);
      SquelchSprite.pushSprite(200, 155);

      tft.fillRect(60, 110, 2 * percent, 6, BarInsignificantColor);

      percentold = percent;
    }
    if (digitalRead(MODEBUTTON) == LOW) {
      while (digitalRead(MODEBUTTON)) delay(50);
      error = 2;
      stopScanning = true;
      break;
    }
  }

  frequency = _current;
  radio.SetFreq(frequency);
  radio.clearRDS();
  radio.setUnMute();
  SQ = false;

  return error;
}
