#pragma region includes
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Wire.h>
#include <Hash.h>
#include <FS.h>
using fs::FS;
#include <SPIFFS.h>
#include "NTPupdate.h"
#include "FONT16.h"
#include "FONT16_CHS.h"
#include "FONT28.h"
#include "FONT28_CHS.h"
#include "FONT48DEC.h"
#include "FREQFONT.h"
#include "globals.h"
#include "gui.h"
#include "comms.h"
#include "rds.h"
#include "touch.h"
#include "nonvolatile.h"
#include "utils.h"
#include "system_console.h"
#pragma endregion

Console console(&tft);
RTC_DATA_ATTR bool gpio_chip = false;
RTC_DATA_ATTR bool tef_found = false;

template<typename... Args>
void panic(Args... args) {
  radio.power(1);
  tft.fillScreen(TFT_RED);
  console.reset();

  (console.print(args), ...);
  while (true);
}

#pragma region helpers
inline void Round30K(unsigned int freq) {
  if (freq % FREQ_OIRT_STEP_30K == 1) frequency_OIRT = (freq + 1);
  else if (freq % FREQ_OIRT_STEP_30K == 0) frequency_OIRT = (freq - 1);
}

void Round50K(unsigned int freq) {
  if (freq % 10 < 3) frequency = (freq - freq % 10);
  else if (freq % 10 > 2 && freq % 10 < 8) frequency = (freq - (freq % 10 - 5));
  else if (freq % 10 > 7) frequency = (freq - (freq % 10) + 10);
}

inline void Round100K(unsigned int freq) {
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

inline void Touch_IRQ_Handler() {
  touch_detect = true;
}

inline void deepSleep() {
  MuteScreen(1);
  StoreFrequency();
  radio.power(1);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)ROTARY_PIN_A, LOW);
  esp_deep_sleep_start();
}

inline bool IsStationEmpty() {
  return presets[memorypos].band == BAND_FM && presets[memorypos].frequency == EE_PRESETS_FREQUENCY;
}

void ShowFreq(int mode);
void doLog() {
  if (!autologged && RDSstatus && radio.rds.correctPI != 0) {
    if (autolog) {
      switch (addRowToCSV()) {
        case 0: ShowFreq(2); break;
        case 1: ShowFreq(3); break;
        case 2: ShowFreq(4); break;
      }

      delay(200);
      ShowFreq(0);
    }
    autologged = true;
  }
}

inline void EdgeBeeper() {
  if(!edgebeep) return;
  radio.tone(50, -5, 2000);
}

const char* textUI(uint16_t number) {
  if (number >= language_entrynumber) return "Overflow";
  else return (const char*)pgm_read_ptr(&(myLanguage[language][number]));
}

void doBWtuneUp() {
  rotary = 0;
  if (band < BAND_GAP) {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), false);
    BWsettemp++;
    if (BWsettemp > 20) BWsettemp = 1;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
  } else {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), false);
    BWsettemp++;
    if (BWsettemp > 4 && BWsettemp < 20) BWsettemp = 20; else if (BWsettemp > 20) BWsettemp = 1;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), true);
  }
}

void doBWtuneDown() {
  rotary = 0;
  if (band < BAND_GAP) {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), false);
    BWsettemp--;
    if (BWsettemp > 20 || BWsettemp == 0) BWsettemp = 20;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsFM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp || (BWsettemp == 17 && BWset == 0) || (BWsettemp == 18 && !iMSset) || (BWsettemp == 19 && !EQset) ? true : false), true);
  } else {
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), false);
    BWsettemp--;
    if (BWsettemp > 4 && BWsettemp < 20) BWsettemp = 4; else if (BWsettemp == 0) BWsettemp = 20;
    drawButton((BWsettemp == 20 ? "OK" : BWButtonLabelsAM[BWsettemp - 1]), BWsettemp - 1, (BWset == BWsettemp ? true : false), true);
  }
}

void ShowStereoStatus() {
  if (StereoToggle) {
    if (band < BAND_GAP) Stereostatus = radio.getStereoStatus(); else Stereostatus = false;
    if (Stereostatus != Stereostatusold) {
      if (Stereostatus && !screenmute) tft.drawBitmap(32, 5, Stereo, 32, 22, StereoColor);
      else if (!screenmute) tft.drawBitmap(32, 5, Stereo, 32, 22, GreyoutColor);
      Stereostatusold = Stereostatus;
    }
  }
}

void ShowRSSI() {
  if (wifi) rssi = WiFi.RSSI(); else rssi = 0;
  if (rssiold != rssi) {
    rssiold = rssi;
    if (!wifi && batterydetect) tft.drawBitmap(282, 3, WiFi4, 30, 25, BackgroundColor);
    else if (rssi == 0) tft.drawBitmap(282, 3, WiFi4, 30, 25, GreyoutColor);
    else if (rssi > -50 && rssi < 0) tft.drawBitmap(282, 3, WiFi4, 30, 25, WifiColorHigh);
    else if (rssi > -60) {
      tft.drawBitmap(282, 3, WiFi4, 30, 25, GreyoutColor);
      tft.drawBitmap(282, 3, WiFi3, 30, 25, WifiColorHigh);
    } else if (rssi > -70) {
      tft.drawBitmap(282, 3, WiFi4, 30, 25, GreyoutColor);
      tft.drawBitmap(282, 3, WiFi2, 30, 25, WifiColorLow);
    } else if (rssi < -70) {
      tft.drawBitmap(282, 3, WiFi4, 30, 25, GreyoutColor);
      tft.drawBitmap(282, 3, WiFi1, 30, 25, WifiColorLow);
    }
  }
}

void ShowBW() {
  if (!BWtune && millis() >= bwupdatetimer + TIMER_BW_TIMER) bwupdatetimer = millis();
  else if(!BWtune) return;

  if (BW != BWOld || BWreset) {
    if(BW == 0) panic("BW is 0");
    else if(BW > 311) panic("BW larger than 311");

    if(BWset == 0) tftReplace(ARIGHT, String(BWOld, DEC), String(BW, DEC), 203, 4, BWAutoColor, BWAutoColorSmooth, BackgroundColor, 28); else tftReplace(ARIGHT, String (BWOld, DEC), String(BW, DEC), 203, 4, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 28);
    BWOld = BW;
    BWreset = false;
    if (wifi) {
      Udp.beginPacket(remoteip, 9030);
      Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";bandwidth=" + String(BW * 1000));
      Udp.endPacket();
    }
  }
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

inline void updateCodetect() {
  if (band > BAND_GAP) {
    if (WAM) tftPrint(ALEFT, "CO", 50, 61, PrimaryColor, PrimaryColorSmooth, 16);
    else tftPrint(ALEFT, "CO", 50, 61, BackgroundColor, BackgroundColor, 16);
  }
}

void SetTunerPatch() {
  if(FORBIDDEN_TUNER(TEF)) {
    radio.init(102);
    uint16_t device, hw, sw;
    radio.getIdentification(&device, &hw, &sw);
    TEF = highByte(hw) * 100 + highByte(sw);
    tft.fillScreen(BackgroundColor);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));

    if(FORBIDDEN_TUNER(TEF)) {
      tftPrint(ACENTER, textUI(32), 150, 78, ActiveColor, ActiveColorSmooth, 28);
      for (;;);
    }
    EEPROM.writeByte(EE_BYTE_TEF, TEF);
    EEPROM.commit();
    Tuner_Reset();
    esp_restart();
  }
}

void ShowSignalLevel();
void ShowOffset();
void ShowBattery();
void GetData() {
  if (!afscreen && !rdsstatscreen) ShowSignalLevel();
  if (!BWtune && !menu && !rdsstatscreen) showPS();

  if (band < BAND_GAP && !BWtune && !menu) {
    if (advancedRDS && !afscreen && !rdsstatscreen && !screenmute) ShowAdvancedRDS();
    if (!advancedRDS && !afscreen && rdsstatscreen && !screenmute) ShowRDSStatistics();
    if (afscreen && !screenmute) ShowAFEON();
    if (!afscreen && !rdsstatscreen) {
      if (!screenmute) ShowErrors();
      showPTY();
      showRadioText();
      if (millis() >= tuningtimer + 200) doAF();
    }
    showPI();
  }

  ShowStereoStatus();

  if (!screenmute) {
    showCT();
    ShowRSSI();
    ShowOffset();
    ShowBW();
    updateCodetect();
    if (millis() >= tuningtimer + 200 && !wifi) ShowBattery();
  }
}

void WakeToSleep(bool yes) {
  if (yes) {
    screensavertriggered = true;
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(1);
        StoreFrequency();
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
        analogWrite(CONTRASTPIN, map(ContrastSet / 100, 0, 100, 15, 255));
        break;
      case LCD_BRIGHTNESS_A_QUARTER:
        analogWrite(CONTRASTPIN, map(ContrastSet / 4, 0, 100, 15, 255));
        break;
      case LCD_BRIGHTNESS_HALF:
        analogWrite(CONTRASTPIN, map(ContrastSet / 2, 0, 100, 15, 255));
        break;
    }
  } else {
    switch (poweroptions) {
      case LCD_OFF:
        MuteScreen(0);
        screensavertriggered = false;
        screensavertimer = millis();
        break;
      case LCD_BRIGHTNESS_1_PERCENT:
      case LCD_BRIGHTNESS_A_QUARTER:
      case LCD_BRIGHTNESS_HALF:
        MuteScreen(0);
        screensavertriggered = false;
        screensavertimer = millis();
        break;
    }
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
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
        }
        SelectBand();
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
        }
        SelectBand();
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
    case AM_BAND_ALL:
      break;
    case AM_BAND_LW_MW:
      if (band == BAND_SW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_LW_SW:
      if (band == BAND_MW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW_SW:
      if (band == BAND_LW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_LW:
      if (band != BAND_LW) {
        band = BAND_LW;
        SelectBand();
      }
      break;
    case AM_BAND_MW:
      if (band != BAND_MW) {
        band = BAND_MW;
        SelectBand();
      }
      break;
    case AM_BAND_SW:
      if (band != BAND_SW) {
        band = BAND_SW;
        SelectBand();
      }
      break;
    case AM_BAND_NONE:
      ToggleBand(band);
      SelectBand();
      break;
  }
}

void BANDBUTTONPress() {
  if (seek) radio.setUnMute();
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
  while (digitalRead(BANDBUTTON) == LOW) delay(50);
  delay(75);
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
      unsigned long counter = millis();
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
  while (digitalRead(BWBUTTON) == LOW) delay(50);
  delay(75);
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

void ShowStepSize() {
  if (!advancedRDS) {
    tft.fillRect(191, 38, 15, 4, GreyoutColor);
    tft.fillRect(222, 38, 15, 4, GreyoutColor);
    if (band < BAND_GAP) tft.fillRect(113, 38, 15, 4, GreyoutColor); else if (band != BAND_LW && band != BAND_MW) tft.fillRect(129, 38, 15, 4, GreyoutColor);
    if (band < BAND_GAP) tft.fillRect(144, 38, 15, 4, GreyoutColor); else tft.fillRect(159, 38, 15, 4, GreyoutColor);
    if (stepsize == 1) tft.fillRect(222, 38, 15, 4, InsignificantColor);
    if (stepsize == 2) tft.fillRect(191, 38, 15, 4, InsignificantColor);
    if (stepsize == 3) {
      if (band < BAND_GAP) tft.fillRect(144, 38, 15, 4, InsignificantColor); else tft.fillRect(159, 38, 15, 4, InsignificantColor);
    }
    if (stepsize == 4) {
      if (band < BAND_GAP) tft.fillRect(113, 38, 15, 4, InsignificantColor); else tft.fillRect(129, 38, 15, 4, InsignificantColor);
    }
  }
}

bool IsFrequencyUsed(unsigned int freq) {
  bool result = false;
  for (byte x = scanstart; x <= scanstop; x++) {
    if ((presets[x].band == BAND_FM || presets[x].band == BAND_OIRT) && presets[x].frequency == freq) {
      result = true;
      break;
    }
  }
  return result;
}

void ShowRDSLogo(bool RDSstatus) {
  if (!screenmute) {
    if (RDSstatus != RDSstatusold) {
      if (RDSstatus) tft.drawBitmap(68, 5, RDSLogo, 35, 22, RDSColor);
      else tft.drawBitmap(68, 5, RDSLogo, 35, 22, GreyoutColor);
    }
    RDSstatusold = RDSstatus;
  }
}

void showAutoSquelch(bool mode) {
  if (mode) tft.drawBitmap(223, 147, AutoSQ, 18, 18, PrimaryColor);
  else tft.drawBitmap(223, 147, AutoSQ, 18, 18, BackgroundColor);
}

void updateBW() {
  if (BWset == 0) {
    if (!BWtune && !screenmute && !advancedRDS && !afscreen && !rdsstatscreen) {
      tft.fillRoundRect(248, 36, 69, 18, 2, SecondaryColor);
      tftPrint(ACENTER, "AUTO BW", 282, 38, BackgroundColor, SecondaryColor, 16);
    }
    radio.setFMABandw();
  } else {
    if (!BWtune && !screenmute && !advancedRDS && !afscreen && !rdsstatscreen) {
      tft.fillRoundRect(248, 36, 69, 18, 2, GreyoutColor);
      tftPrint(ACENTER, "AUTO BW", 282, 38, BackgroundColor, GreyoutColor, 16);
    }
  }
}

void updateiMS() {
  if (band < BAND_GAP) {
    if (iMSset == 0) {
      if (!screenmute && !advancedRDS && !afscreen && !rdsstatscreen && !BWtune) {
        tft.fillRoundRect(249, 57, 30, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "iMS", 265, 59, BackgroundColor, SecondaryColor, 16);
      }
      radio.setiMS(1);
    } else {
      if (!screenmute && !advancedRDS && !afscreen && !rdsstatscreen && !BWtune) {
        tft.fillRoundRect(249, 57, 30, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "iMS", 265, 59, BackgroundColor, GreyoutColor, 16);
      }
      radio.setiMS(0);
    }
  }
}

void updateEQ() {
  if (band < BAND_GAP) {
    if (EQset == 0) {
      if (!screenmute && !advancedRDS && !afscreen && !rdsstatscreen && !BWtune) {
        tft.fillRoundRect(287, 57, 30, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "EQ", 301, 59, BackgroundColor, SecondaryColor, 16);
      }
      radio.setEQ(1);
    } else {
      if (!screenmute && !advancedRDS && !afscreen && !rdsstatscreen && !BWtune) {
        tft.fillRoundRect(287, 57, 30, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "EQ", 301, 59, BackgroundColor, GreyoutColor, 16);
      }
      radio.setEQ(0);
    }
  }
}

void DataPrint(String string) {
  if (XDRGTKUSB) Serial.print(string);
  if (XDRGTKTCP) RemoteClient.print(string);
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
    }
    if (iMSEQ == 3) {
      iMSset = 1;
      EQset = 0;
      updateiMS();
      updateEQ();
      iMSEQ = 4;
    }
    if (iMSEQ == 2) {
      iMSset = 0;
      EQset = 1;
      updateiMS();
      updateEQ();
      iMSEQ = 3;
    }
    if (iMSEQ == 1) {
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

void TuneFreq(int temp) {
  int newfreq = temp;

  if (band == BAND_FM) {
    while (newfreq < (LowEdgeSet * 10)) newfreq *= 10;
    if (newfreq > (HighEdgeSet * 10)) {
      EdgeBeeper();
      return;
    }
    if (newfreq == frequency) return;
    frequency = newfreq;
    radio.SetFreq(frequency);

  } else if (band == BAND_OIRT) {
    while (newfreq < (LowEdgeOIRTSet * 10)) newfreq *= 10;
    if (newfreq > HighEdgeOIRTSet) {
      EdgeBeeper();
      return;
    }
    if (newfreq == frequency_OIRT) return;
    frequency_OIRT = newfreq;
    radio.SetFreq(frequency_OIRT);

  } else if (band == BAND_LW) {
    while (newfreq < LWLowEdgeSet) newfreq *= 10;
    if (newfreq > LWHighEdgeSet) {
      EdgeBeeper();
      return;
    }
    if (newfreq == frequency_LW) return;
    frequency_AM = newfreq;
    frequency_LW = newfreq;
    radio.SetFreqAM(frequency_AM);

  } else if (band == BAND_MW) {
    while (newfreq < MWLowEdgeSet) newfreq *= 10;
    if (newfreq > MWHighEdgeSet) {
      EdgeBeeper();
      return;
    }
    if (newfreq == frequency_MW) return;
    frequency_AM = newfreq;
    frequency_MW = newfreq;
    radio.SetFreqAM(frequency_AM);

  } else if (band == BAND_SW) {
    while (newfreq < SWLowEdgeSet) newfreq *= 10;
    if (newfreq > SWHighEdgeSet) {
      EdgeBeeper();
      return;
    }
    if (newfreq == frequency_SW) return;
    frequency_AM = newfreq;
    frequency_SW = newfreq;
    radio.SetFreqAM(frequency_AM);
  }

  aftest = true;
  aftimer = millis();

  radio.clearRDS();
  if (RDSSPYUSB) Serial.print("G:\r\nRESET-------\r\n\r\n");
  if (RDSSPYTCP) RemoteClient.print("G:\r\nRESET-------\r\n\r\n");
}

void ShowNum(int val) {
  FrequencySprite.setTextDatum(TR_DATUM);

  FrequencySprite.fillSprite(BackgroundColor);
  FrequencySprite.setTextColor(SecondaryColor, SecondaryColorSmooth, false);
  FrequencySprite.drawString(String(val) + " ", 218, -6, freqfont);
  FrequencySprite.pushSprite(46, 46);
}

byte numval[16] = {2, 3, 127, 5, 6, 0, 9, 13, 8, 7, 4, 1, 0, 0, 0, 0};

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
      if ((temp & 0x01) == 0) {
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
    }
    DoMemoryPosTune();
  } else {
    tunemode = TUNE_MAN;
    if (band != presets[memorypos].band) {
      band = presets[memorypos].band;
      SelectBand();
    }
    TuneUp();
    ShowFreq(0);
  }
  if (scanmute) {
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
  }
  screensavertimer = millis();
}

void NumpadProcess(int num) {
  if (scandxmode) {
    if (num == 127) {
      ShowFreq(5);
      ShowFreq(0);
    }
  } else {
    if (num == 127) {
      freq_in = 0;
      menuoption = ITEM1;
      menupage = DXMODE;
      menuitem = 0;
      if (spispeed == 7) tft.setSPISpeed(40);
      submenu = true;
      menu = true;
      if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
      BuildMenu();
    } else if (num == 13) {
      if (freq_in != 0) {
        TuneFreq(freq_in);
        if (XDRGTKUSB || XDRGTKTCP) {
          if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
        }
        if (!memorystore) {
          if (!memtune) radio.clearRDS();
          memtune = false;
          ShowFreq(0);
          store = true;
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
    case 950: tft.setSPISpeed(19); break;
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
    default: tft.setSPISpeed(30); break;
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
  Serial.end();
  if (wifi) remoteip = IPAddress (WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], subnetclient);
  if (USBmode) Serial.begin(19200); else Serial.begin(115200);

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
      tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
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
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
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
    }
    setupmode = false;
  } else if (setting && !screenmute) {
    screenmute = true;
    analogWrite(CONTRASTPIN, 0);
    tft.writecommand(0x10);
  }
}
#pragma endregion

void setup_periph() {
  Wire.setClock(400000);
  Serial.begin(115200);
  Serial.println();
  byte error, address;
  for (address = 1; address < 108; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      if(address == RX8010SJ_ADDRESS) {
        Serial.print(" RTC");
        rx_rtc_avail = true;
      } else if(address == TEF668X_ADDRESS) {
        Serial.print(" TEF");
        tef_found = true;
      } else if(address == XL9555_ADDRESS) {
        Serial.print(" GPIO");
        gpio_chip = true;
      }
      Serial.println(" !");
    } else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  Serial.flush();
  Serial.end();

  Wire.setClock(100000);
}

void later_setup_periph() {
  if(gpio_chip) {
    console.print("XL9555 found, setting up");
    // Configures the GPIO chip for input in every pin
    Wire.beginTransmission(XL9555_ADDRESS);
    Wire.write(0x06);
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.endTransmission();
  } else console.print("XL9555 found not found on address " + String(XL9555_ADDRESS, HEX) + ". Numpad will not work");

  if(tef_found) {
    // The tuner being missing here, would cause a infinite loop with no exit and no error, as it resets and polls the chip if it reset, if no response then we try again, and now, you see?
    radio.init(TEF);
  }

  uint16_t device, hw, sw;
  radio.getIdentification(&device, &hw, &sw);
  if (TEF != (highByte(hw) * 100 + highByte(sw))) SetTunerPatch();

  if (lowByte(device) == 14) {
    fmsi = false;
    chipmodel = 0;
    console.print("Detected a TEF6686 Lithio");
  } else if (lowByte(device) == 1) {
    chipmodel = 1;
    console.print("Detected a TEF6687 Lithio FMSI");
  } else if (lowByte(device) == 9) {
    chipmodel = 2;
    fmsi = false;
    console.print("Detected a TEF6688 Lithio DR");
  } else if (lowByte(device) == 3) {
    chipmodel = 3;
    console.print("Detected a TEF6689 Lithio FMSI DR");
  } else panic("Unknown TEF device");
  console.print("Chip Patch: v" + String(TEF));

  if(VolSet != 0) radio.setVolume(VolSet);
  radio.setOffset(LevelOffset);
  radio.setAMOffset(AMLevelOffset);
  radio.setAMCoChannel(amcodect, amcodectcount);
  radio.setAMAttenuation(amgain);
  radio.setStereoLevel(StereoLevel);
  radio.setHighCutLevel(HighCutLevel);
  radio.setHighCutOffset(HighCutOffset);
  radio.clearRDS();
  radio.setMute();
  if (!StereoToggle) radio.setMono(true);
  radio.setSoftmuteFM(softmutefm);
  radio.setSoftmuteAM(softmuteam);
  radio.setAMNoiseBlanker(amnb);
  radio.setFMNoiseBlanker(fmnb);
  if(audiomode != 0) radio.setAudio(audiomode);
  if(fmdeemphasis != DEEMPHASIS_50) radio.setDeemphasis(fmdeemphasis);
  if(fmagc != 92) radio.setAGC(fmagc);
  if(amagc != 100) radio.setAMAGC(amagc);
  if (fmsi) radio.setFMSI(2);

  if(rx_rtc_avail) {
    bool reset = init_rtc();
    if(reset) console.print("RX8010SJ was reset, no time");
    else {
      rtcset = true;
      console.print("RX8010SJ is used as a time source");
    }
  } else console.print("RX8010SJ is not available at address " + String(RX8010SJ_ADDRESS, HEX));

  if(analogRead(BATTERY_PIN) < 200) batterydetect = false;
  else console.print("Battery detected.");
}

void read_encoder();
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  gpio_set_drive_capability((gpio_num_t) 5, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 16, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 17, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 18, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 19, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 21, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 22, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t) 23, GPIO_DRIVE_CAP_0);
  EEPROM.begin(EE_TOTAL_CNT);

  setupmode = true;

  Wire.begin();
  Wire.setClock(100000);

  loadData();

  if (spispeed == SPI_SPEED_DEFAULT) tft.setSPISpeed(SPI_FREQUENCY / 1000000);
  else if (spispeed == 7) setAutoSpeedSPI();
  else tft.setSPISpeed(spispeed * 10);

  if(esp_reset_reason() != ESP_RST_DEEPSLEEP) setup_periph();

  LWLowEdgeSet = FREQ_LW_LOW_EDGE_MIN;
  LWHighEdgeSet = FREQ_LW_HIGH_EDGE_MAX;
  MWLowEdgeSet = mwstepsize == false ? FREQ_MW_LOW_EDGE_MIN_9K : FREQ_MW_LOW_EDGE_MIN_10K;
  MWHighEdgeSet = mwstepsize == false ? FREQ_MW_HIGH_EDGE_MAX_9K : FREQ_MW_HIGH_EDGE_MAX_10K;
  SWLowEdgeSet = FREQ_SW_LOW_EDGE_MIN;
  SWHighEdgeSet = FREQ_SW_HIGH_EDGE_MAX;
  LowEdgeOIRTSet = FREQ_FM_OIRT_START;
  HighEdgeOIRTSet = FREQ_FM_OIRT_END;

  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].band = EEPROM.readByte(i + EE_PRESETS_BAND_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].frequency = EEPROM.readUInt((i * 4) + EE_PRESETS_FREQUENCY_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].bw = EEPROM.readByte(i + EE_PRESET_BW_START);
  for (int i = 0; i < EE_PRESETS_CNT; i++) presets[i].ms = EEPROM.readByte(i + EE_PRESET_MS_START);

  for (int i = 0; i < EE_PRESETS_CNT; i++) {
    for (int y = 0; y < 9; y++) presets[i].RDSPS[y] = EEPROM.readByte((i * 9) + y + EE_PRESETS_RDSPS_START);
    for (int y = 0; y < 5; y++) presets[i].RDSPI[y] = EEPROM.readByte((i * 5) + y + EE_PRESETS_RDSPI_START);
  }

  if (USBmode) Serial.begin(19200); else Serial.begin(115200);

  if (iMSset && EQset) iMSEQ = 2;
  if (!iMSset && EQset) iMSEQ = 3;
  if (iMSset && !EQset) iMSEQ = 4;
  if (!iMSset && !EQset) iMSEQ = 1;

  switch (band) {
    case BAND_LW:
      frequency_LW = frequency_AM;
      if (stepsize > 3) stepsize = 3;
      break;
    case BAND_MW:
      frequency_MW = frequency_AM;
      if (stepsize > 3) stepsize = 3;
      break;
    case BAND_SW: frequency_SW = frequency_AM; break;
    case BAND_FM:
      if (frequency % 10 != 0) {
        if (fmdefaultstepsize == 1) Round100K(frequency); else Round50K(frequency);
      }
      break;
    case BAND_OIRT:
      if (frequency % FREQ_OIRT_STEP_30K != 2) Round30K(frequency_OIRT);
      break;
  }

  tft.init();

  tft.loadFont(FONT48, 2);

  doTheme();

  if (displayflip == 0) tft.setRotation(3);
  else tft.setRotation(1);

  tft.invertDisplay(!invertdisplay);

  pinMode(BANDBUTTON, INPUT);
  pinMode(MODEBUTTON, INPUT);
  pinMode(BWBUTTON, INPUT);
  pinMode(ROTARY_BUTTON, INPUT);
  pinMode(ROTARY_PIN_A, INPUT);
  pinMode(ROTARY_PIN_B, INPUT);
  pinMode(TOUCHIRQ, INPUT);
  pinMode(EXT_IRQ, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(TOUCHIRQ), Touch_IRQ_Handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), read_encoder, CHANGE);

  tft.setSwapBytes(true);

  SPIFFS.begin();

  FrequencySprite.createSprite(200, 50);
  FrequencySprite.setTextDatum(TR_DATUM);
  FrequencySprite.setSwapBytes(true);
  FrequencySprite.loadFont(FREQFONT0, 0);
  FrequencySprite.loadFont(FREQFONT1, 1);
  FrequencySprite.loadFont(FREQFONT2, 2);
  FrequencySprite.loadFont(FREQFONT3, 3);
  FrequencySprite.loadFont(FREQFONT4, 4);
  FrequencySprite.loadFont(FREQFONT5, 5);

  GeneralTextSprite.createSprite(308, 28);
  GeneralTextSprite.setSwapBytes(true);
  GeneralTextSprite.loadFont(FONT48, 2);

  PSSprite.createSprite(150, 32);
  PSSprite.setSwapBytes(true);
  PSSprite.loadFont(FONT16, 0);
  PSSprite.loadFont(FONT16_CHS, 1);
  PSSprite.loadFont(FONT28, 2);
  PSSprite.loadFont(FONT28_CHS, 3);

  SquelchSprite.createSprite(27, 19);
  SquelchSprite.setSwapBytes(true);

  FullLineSprite.createSprite(308, 19);
  FullLineSprite.setSwapBytes(true);

  OneBigLineSprite.createSprite(270, 30);
  OneBigLineSprite.setSwapBytes(true);

  SignalSprite.createSprite(80, 48);
  SignalSprite.loadFont(FONT28, 0);
  SignalSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
  SignalSprite.setSwapBytes(true);

  UpdateFonts();
  tft.fillScreen(BackgroundColor);

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    if (rotarymode == 0) rotarymode = 1; else rotarymode = 0;
    EEPROM.writeByte(EE_BYTE_ROTARYMODE, rotarymode);
    EEPROM.commit();
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    Infoboxprint(textUI(1));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(BWBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == LOW && digitalRead(BANDBUTTON) == HIGH) {
    if (displayflip == 0) {
      displayflip = 1;
      tft.setRotation(1);
    } else {
      displayflip = 0;
      tft.setRotation(3);
    }
    EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, displayflip);
    EEPROM.commit();
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    Infoboxprint(textUI(3));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(MODEBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    Infoboxprint(textUI(4));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(BANDBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == LOW && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    DefaultSettings();
    Infoboxprint(textUI(63));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(ROTARY_BUTTON) == LOW && digitalRead(BWBUTTON) == LOW) delay(50);
    esp_restart();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == LOW && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    Infoboxprint(textUI(279));
    tftPrint(ACENTER, textUI(280), 155, 100, ActiveColor, ActiveColorSmooth, 28);
    tft.calibrateTouch(TouchCalData, PrimaryColor, BackgroundColor, 30);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH1, TouchCalData[0]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH2, TouchCalData[1]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH3, TouchCalData[2]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH4, TouchCalData[3]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH5, TouchCalData[4]);
    EEPROM.commit();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 15, 255));
    Infoboxprint(textUI(66));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    invertdisplay = !invertdisplay;
    tft.invertDisplay(!invertdisplay);
    while (digitalRead(BWBUTTON) == LOW && digitalRead(BANDBUTTON) == LOW) delay(50);
    EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, invertdisplay);
    EEPROM.commit();
  }

  tft.setTouch(TouchCalData);

  tft.fillScreen(TFT_BLACK);

  tft.pushImage((tft.width() - 163) / 2, (tft.height() - 84) / 2, 163, 84, radiologo, TFT_BLACK);
  tft.drawBitmap((tft.width() - 59) / 2, 24, TEFLogo, 59, 23, ActiveColor);

  for (int x = 0; x <= ContrastSet; x++) {
    analogWrite(CONTRASTPIN, map(x, 0, 100, 15, 255));
    delay(8);
  }

  console.print("Firmware " + String(VERSION));

  if(!tef_found) {
    console.print(textUI(6));
    while (true) delay(1);
  }


  TEF = EEPROM.readByte(EE_BYTE_TEF);
  if(FORBIDDEN_TUNER(TEF)) SetTunerPatch();

  if(esp_reset_reason() != ESP_RST_DEEPSLEEP) later_setup_periph();

  if(!SPIFFS.exists("/logbook.csv")) {
    handleCreateNewLogbook();
    console.print("Creating a new logbook.");
  }

  if (wifi) {
    console.print("Trying WiFi, setting clock to 160 MHz");
    tryWiFi();
    delay(1750);
  } else {
    Server.end();
    Udp.stop();
  }

  while(digitalRead(ROTARY_BUTTON) == LOW) delay(75);

  console.print("Init done.");

  LowLevelInit = true;

  BuildDisplay();
  SelectBand();
  if (tunemode == TUNE_MEM) DoMemoryPosTune();

  eccDisplay.setPostDrawCallback([&](TFT_eSprite* sprite, bool scrolling) {
    sprite->fillRect(275, 0, 10, 19, BackgroundColor);
    sprite->drawLine(284, 0, 284, 19, FrameColor);
  });
  rtDisplay.setPostDrawCallback([&](TFT_eSprite* sprite, bool scrolling) {
    sprite->fillRect(275, 0, 10, 19, BackgroundColor);
    sprite->drawLine(284, 0, 284, 19, FrameColor);
    if(radio.rds.hasRT) {
      sprite->fillCircle(278, 3, 2, radio.rds.rtAB ? GreyoutColor : InsignificantColor);
      sprite->fillCircle(278, 14, 2, radio.rds.rtAB ? InsignificantColor : GreyoutColor);
    }
  });

  setupmode = false;

  if (!usesquelch) radio.setUnMute();

  screensavertimer = millis();
  tottimer = millis();
  console.reset();
}

void ShowModLevel();
void doSquelch();
void loop() {
  if (wifi && !menu) {
    webserver.handleClient();

    if (millis() >= udplogtimer + 500) {
      sendUDPlog();
      udplogtimer = millis();
    }

    if (millis() >= NTPtimer + 1800000) {
      NTPupdate();
      NTPtimer = millis();
    }
  }

  if (hardwaremodel == PORTABLE_TOUCH_ILI9341 && touch_detect) {
    if (tft.getTouchRawZ() > 100) {
      uint16_t x, y;
      tft.getTouch(&x, &y);
      if (x > 0 || y > 0) {
        if (!firstTouchHandled) {
          doTouchEvent(x, y);
          firstTouchHandled = true;
          lastTouchTime = millis();
        } else if (touchrepeat && millis() - lastTouchTime >= 500) doTouchEvent(x, y);
      }
    } else {
      firstTouchHandled = false;
      touch_detect = false;
    }
  }

  Communication();

  // tottimer is like the time of the last interaction
  if (tot != 0 && millis() >= tottimer + (tot * 60000)) deepSleep();

  if (freq_in != 0 && millis() >= keypadtimer + 2000) {
    freq_in = 0;
    ShowFreq(0);
  }

  if (scandxmode) {
    unsigned long waitTime = (scanhold == 0) ? 500 : (scanhold * 1000);
    if (!scanholdflag) scanholdflag = (USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80) && (OStatus > -80);
    bool bypassMillisCheck = scanholdonsignal && !scanholdflag;
    bool shouldScan = bypassMillisCheck || (!bypassMillisCheck && (millis() >= scantimer + waitTime));

    if (shouldScan) {
      if (scanmute && scanholdonsignal) {
        radio.setMute();
        if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
        SQ = true;
      }
      scanholdflag = false;
      if (scanmem) {
        memorypos++;
        if (memorypos > scanstop) memorypos = scanstart;
        while (IsStationEmpty() || (presets[memorypos].band != BAND_FM && presets[memorypos].band != BAND_OIRT)) {
          memorypos++;
          if (memorypos > scanstop) {
            memorypos = scanstart;
            break;
          }
        }
        doLog();
        DoMemoryPosTune();
        radio.clearRDS();
        autologged = false;
        ShowMemoryPos();
      } else {
        doLog();
        TuneUp();
        autologged = false;
        ShowFreq(0);
        if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
      }
      scantimer = millis();
      initdxscan = false;
    } else {
      if (scanmute && scanholdonsignal) {
        radio.setUnMute();
        SQ = false;
        if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
      }
    }

    if (millis() >= flashingtimer + 500) {
      flashing = !flashing;
      if (flashing) {
        tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
      } else {
        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
      }
      flashingtimer = millis();
    }

    if (!scanholdflag) delay(75);
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);

    if (!initdxscan) {
      switch (scancancel) {
        case CORRECTPI:
          if (RDSstatus && radio.rds.correctPI != 0) cancelDXScan();
          break;
        case SIGNAL:
          if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80 && OStatus > -80) && (Squelch < SStatus || Squelch == 920)) cancelDXScan();
          break;
      }
    }
  }

  if (millis() >= tuningtimer + 200) rdsflagreset = false;

  if (millis() >= tuningtimer + 2000) {
    if (store) {
      StoreFrequency();
      store = false;
    }
  }

  if (!BWtune && !menu && !afscreen && !rdsstatscreen && !scandxmode) {
    if (af != 0 && dropout && millis() >= aftimer + 1000) {
      aftimer = millis();
      if (radio.af_counter == 0) {
        if (findMemoryAF && radio.rds.correctPI != 0 && tunemode == TUNE_MEM && (USN > 250 || WAM > 250)) {
          radio.setMute();
          tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
          SQ = true;
          if (!screenmute) {
            if (advancedRDS) {
              tft.drawRoundRect(10, 30, 300, 170, 2, ActiveColor);
              tft.fillRoundRect(12, 32, 296, 166, 2, BackgroundColor);
              tftPrint(ACENTER, textUI(31), 160, 100, ActiveColor, ActiveColorSmooth, 28);
            } else ShowFreq(1);
          }

          bool foundmemaf = false;
          for (int x = 8750; x <= 10800; x += 10) {
            if (rotary != 0 || digitalRead(BANDBUTTON) == LOW || digitalRead(MODEBUTTON) == LOW || digitalRead(BWBUTTON) == LOW || digitalRead(ROTARY_BUTTON) == LOW) break;
            radio.SetFreq(x);
            unsigned long millisold = millis();
            while (millis() - millisold < 187) {
              if (!screenmute && !advancedRDS) ShowModLevel();
            }
            if (radio.rds.correctPI == radio.getBlockA()) {
              frequency = x;
              foundmemaf = true;
              break;
            }
          }

          if (!foundmemaf) {
            frequency = freqold;
            radio.SetFreq(frequency);
          }

          if (!screenmute) {
            if (advancedRDS) {
              leave = true;
              BuildAdvancedRDS();
              freq_in = 0;
            } else ShowFreq(0);
          }

          radio.setUnMute();
          SQ = false;
          tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        }
        findMemoryAF = false;
      } else frequency = radio.TestAF();
      if (freqold != frequency) {
        ShowFreq(0);
        dropout = true;
        if (radio.afmethodB) {
          afmethodBold = true;
          radio.clearRDS();
        }
      }
      if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
      if (screenmute) {
        freqold = frequency;
        dropout = false;
      }
      store = true;
    }

    if (band == BAND_FM && af != 0 && radio.rds.correctPI != 0) {
      if ((aftest && millis() >= aftimer + 3000) || ((USN > 250 || WAM > 250) && millis() >= aftimer + 1000)) {
        aftimer = millis();
        aftest = false;
        frequency = radio.TestAF();
        if (freqold != frequency) {
          ShowFreq(0);
          dropout = true;
          if (radio.afmethodB) {
            afmethodBold = true;
            radio.clearRDS();
          }
          if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
          if (screenmute) {
            freqold = frequency;
            dropout = false;
          }
          store = true;
        }
      }

      if (band == BAND_FM && millis() >= aftimer + 10000) {
        aftimer = millis();
        if (USN > 150 || WAM > 150) {
          frequency = radio.TestAF();
          if (freqold != frequency) {
            ShowFreq(0);
            dropout = true;
            if (radio.afmethodB) {
              afmethodBold = true;
              radio.clearRDS();
            }
            if (XDRGTKUSB || XDRGTKTCP) DataPrint("T" + String((frequency + ConverterSet * 100) * 10) + "\n");
            store = true;
            if (screenmute) {
              freqold = frequency;
              dropout = false;
            }
          }
        }
      }
    }
  }

  if (seek) Seek(direction);

  if ((SStatus / 10 > LowLevelSet) && !LowLevelInit && !BWtune && !menu && band < BAND_GAP) {
    if (!screenmute && !advancedRDS && !rdsstatscreen && !afscreen) {
      if (showmodulation) {
        tftPrint(ALEFT, "10", 24, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ALEFT, "30", 54, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ALEFT, "50", 84, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ALEFT, "70", 114, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ALEFT, "100", 160, 144, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ACENTER, "A", 7, 128, ActiveColor, ActiveColorSmooth, 16);
        for (byte segments = 0; segments < 93; segments++) {
          if (segments > 54) {
            if (((segments - 53) % 10) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, BarSignificantColor);
          } else {
            if (((segments + 1) % 6) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, BarInsignificantColor);
          }
        }
      }
      if (radio.rds.region == 0) tftPrint(ALEFT, "PI", 212, 193, ActiveColor, ActiveColorSmooth, 16);
      else {
        tftPrint(ALEFT, "PI", 212, 184, ActiveColor, ActiveColorSmooth, 16);
        tftPrint(ALEFT, "ID", 212, 201, ActiveColor, ActiveColorSmooth, 16);
      }
      tftPrint(ALEFT, "PS", 3, 193, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(ALEFT, "RT", 3, 221, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(ALEFT, "PTY", 3, 163, ActiveColor, ActiveColorSmooth, 16);
      if (!showmodulation) tft.drawLine(16, 143, 189, 143, GreyoutColor); else tft.drawLine(16, 143, 189, 143, ActiveColor);
    }
    LowLevelInit = true;
  }

  if ((SStatus / 10 <= LowLevelSet) && band < BAND_GAP) {
    if (LowLevelInit && !BWtune && !menu) {
      if (!screenmute && !rdsstatscreen && !afscreen && !advancedRDS) {
        if(showmodulation) {
          tftPrint(ALEFT, "10", 24, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(ALEFT, "30", 54, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(ALEFT, "50", 84, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(ALEFT, "70", 114, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(ALEFT, "100", 160, 144, GreyoutColor, BackgroundColor, 16);
          tftPrint(ACENTER, "A", 7, 128, GreyoutColor, BackgroundColor, 16);
          tft.fillRect(16, 133, 174, 6, GreyoutColor);
          for (byte segments = 0; segments < 93; segments++) {
            if (segments > 54) {
              if (((segments - 53) % 10) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, GreyoutColor);
            } else {
              if (((segments + 1) % 6) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, GreyoutColor);
            }
          }
        }
        if (radio.rds.region == 0) tftPrint(ALEFT, "PI", 212, 193, GreyoutColor, BackgroundColor, 16);
        else {
          tftPrint(ALEFT, "PI", 212, 184, GreyoutColor, BackgroundColor, 16);
          tftPrint(ALEFT, "ID", 212, 201, GreyoutColor, BackgroundColor, 16);
        }
        tftPrint(ALEFT, "PS", 3, 193, GreyoutColor, BackgroundColor, 16);
        tftPrint(ALEFT, "RT", 3, 221, GreyoutColor, BackgroundColor, 16);
        tftPrint(ALEFT, "PTY", 3, 163, GreyoutColor, BackgroundColor, 16);
        tft.drawLine(16, 143, 189, 143, GreyoutColor);
        tft.drawBitmap(68, 5, RDSLogo, 35, 22, GreyoutColor);
      }
      LowLevelInit = false;
    }

    if (!BWtune && !menu && (screenmute || radio.rds.correctPI != 0)) readRds();
    if (millis() >= lowsignaltimer + 300) {
      lowsignaltimer = millis();
      if (af || (!screenmute || (screenmute && (XDRGTKTCP || XDRGTKUSB)))) {
        if (band < BAND_GAP) radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
        else radio.getStatusAM(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
      }
      if (!BWtune && !menu) {
        doSquelch();
        GetData();
      }
    }

  } else {
    if (af || (!screenmute || (screenmute && (XDRGTKTCP || XDRGTKUSB)))) {
      if (band < BAND_GAP) radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
      else radio.getStatusAM(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
    }
    if (!BWtune && !menu) {
      doSquelch();
      if (millis() >= tuningtimer + 200) readRds();
      GetData();
      if (!screenmute && !rdsstatscreen && !afscreen && !advancedRDS) ShowModLevel();
    }
  }

  if (rotary == -1) {
    tottimer = millis();
    if (screensavertriggered) {
      if (!touchrotating) {
        rotary = 0;
        WakeToSleep(REVERSE);
      } else {
        if (BWtune) doBWtuneUp(); else KeyUp();
      }
    } else {
      if (BWtune) doBWtuneUp(); else KeyUp();
      if (rotaryaccelerate && rotarycounter > 2 && !BWtune && !menu) {
        for (int i = 0; i < rotarycounteraccelerator; i++) KeyUp();
        rotarycounter = 0;
      }
      if (screensaverset > 0 && !BWtune && !menu && !screensavertriggered) screensavertimer = millis();
    }
  }

  if (rotary == 1) {
    tottimer = millis();
    if (screensavertriggered) {
      if (!touchrotating) {
        rotary = 0;
        WakeToSleep(REVERSE);
      } else {
        if (BWtune) doBWtuneDown(); else KeyDown();
      }
    } else {
      if (BWtune) doBWtuneDown(); else KeyDown();
      if (rotaryaccelerate && rotarycounter > 2 && !BWtune && !menu) {
        for (int i = 0; i < rotarycounteraccelerator; i++) KeyDown();
        rotarycounter = 0;
      }
      if (screensaverset > 0 && !BWtune && !menu && !screensavertriggered) screensavertimer = millis();
    }
  }

  if (digitalRead(BANDBUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(BANDBUTTON) == LOW);
    } else BANDBUTTONPress();
  }

  if (digitalRead(ROTARY_BUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(ROTARY_BUTTON) == LOW);
    } else if (!afscreen && !rdsstatscreen) ButtonPress();
  }

  if (digitalRead(MODEBUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(MODEBUTTON) == LOW);
    } else if(!screenmute) ModeButtonPress();
  }

  if (digitalRead(BWBUTTON) == LOW && !BWtune) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(REVERSE);
      while (digitalRead(BWBUTTON) == LOW);
    } else if(!screenmute) BWButtonPress();
  }

  if (digitalRead(EXT_IRQ) == LOW) {
    if (screensavertriggered) WakeToSleep(REVERSE);
    int num = GetNum();
    if (!screenmute && !BWtune && !menu && !advancedRDS && !rdsstatscreen && !afscreen && num != -1) NumpadProcess(num);
  }

  if (screensaverset > 0 && !screensavertriggered && !BWtune && !menu && millis() >= screensavertimer + 1000 * screensaverOptions[screensaverset]) WakeToSleep(true);
}

void DivdeSWMIBand() {
  if (frequency_AM >= FREQ_SW_160M_START && frequency_AM <= FREQ_SW_160M_END) {
    SWMIBandPos = SW_MI_BAND_160M;
    frequency_MIBand_160M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_120M_START && frequency_AM <= FREQ_SW_120M_END) {
    SWMIBandPos = SW_MI_BAND_120M;
    frequency_MIBand_120M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_90M_START && frequency_AM <= FREQ_SW_90M_END) {
    SWMIBandPos = SW_MI_BAND_90M;
    frequency_MIBand_90M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_75M_START && frequency_AM <= FREQ_SW_75M_END) {
    SWMIBandPos = SW_MI_BAND_75M;
    frequency_MIBand_75M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_60M_START && frequency_AM <= FREQ_SW_60M_END) {
    SWMIBandPos = SW_MI_BAND_60M;
    frequency_MIBand_60M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_49M_START && frequency_AM <= FREQ_SW_49M_END) {
    SWMIBandPos = SW_MI_BAND_49M;
    frequency_MIBand_49M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_41M_START && frequency_AM <= FREQ_SW_41M_END) {
    SWMIBandPos = SW_MI_BAND_41M;
    frequency_MIBand_41M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_31M_START && frequency_AM <= FREQ_SW_31M_END) {
    SWMIBandPos = SW_MI_BAND_31M;
    frequency_MIBand_31M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_25M_START && frequency_AM <= FREQ_SW_25M_END) {
    SWMIBandPos = SW_MI_BAND_25M;
    frequency_MIBand_25M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_22M_START && frequency_AM <= FREQ_SW_22M_END) {
    SWMIBandPos = SW_MI_BAND_22M;
    frequency_MIBand_22M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_19M_START && frequency_AM <= FREQ_SW_19M_END) {
    SWMIBandPos = SW_MI_BAND_19M;
    frequency_MIBand_19M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_16M_START && frequency_AM <= FREQ_SW_16M_END) {
    SWMIBandPos = SW_MI_BAND_16M;
    frequency_MIBand_16M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_15M_START && frequency_AM <= FREQ_SW_15M_END) {
    SWMIBandPos = SW_MI_BAND_15M;
    frequency_MIBand_15M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_13M_START && frequency_AM <= FREQ_SW_13M_END) {
    SWMIBandPos = SW_MI_BAND_13M;
    frequency_MIBand_13M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else if (frequency_AM >= FREQ_SW_11M_START && frequency_AM <= FREQ_SW_11M_END) {
    SWMIBandPos = SW_MI_BAND_11M;
    frequency_MIBand_11M = frequency_AM;
    SWMIBandstring = (String) "" + SWMIBandPos + " m";
  } else {
    if (SWMIBandPos > SW_MI_BAND_GAP) {
      SWMIBandPosold = SWMIBandPos;
      SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
    }
    SWMIBandPos = SW_MI_BAND_GAP;
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
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_160M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_160M == 0 ? FREQ_SW_160M_START : frequency_MIBand_160M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_13M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_13M == 0 ? FREQ_SW_13M_END : frequency_MIBand_13M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_13M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_11M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_11M == 0 ? FREQ_SW_11M_START : frequency_MIBand_11M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_15M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
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
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_19M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_19M == 0 ? FREQ_SW_19M_START : frequency_MIBand_19M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_25M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_25M == 0 ? FREQ_SW_25M_END : frequency_MIBand_25M;
      }
      radio.SetFreqAM(frequency_AM);
      frequency_SW = frequency_AM;
      break;
    case SW_MI_BAND_25M:
      if (frequencyup) {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_22M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
        frequency_AM = frequency_MIBand_22M == 0 ? FREQ_SW_22M_START : frequency_MIBand_22M;
      } else {
        SWMIBandstringold = (String) "" + SWMIBandPosold + " m";
        SWMIBandPos = SW_MI_BAND_31M;
        SWMIBandPosold = SWMIBandPos;
        SWMIBandstring = (String) "" + SWMIBandPos + " m";
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
      if (radio.rds.region == 0) tftPrint(ALEFT, "PI", 212, 193, GreyoutColor, BackgroundColor, 16);
      else {
        tftPrint(ALEFT, "PI", 212, 184, GreyoutColor, BackgroundColor, 16);
        tftPrint(ALEFT, "ID", 212, 201, GreyoutColor, BackgroundColor, 16);
      }

      tftPrint(ALEFT, "PS", 3, 193, GreyoutColor, BackgroundColor, 16);
      tftPrint(ALEFT, "RT", 3, 221, GreyoutColor, BackgroundColor, 16);
      tftPrint(ALEFT, "PTY", 3, 163, GreyoutColor, BackgroundColor, 16);
      tft.drawBitmap(68, 5, RDSLogo, 35, 22, GreyoutColor);
      tft.fillRoundRect(249, 57, 30, 18, 2, GreyoutColor);
      tftPrint(ACENTER, "iMS", 265, 59, BackgroundColor, GreyoutColor, 16);
      tft.fillRoundRect(287, 57, 30, 18, 2, GreyoutColor);
      tftPrint(ACENTER, "EQ", 301, 59, BackgroundColor, GreyoutColor, 16);
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
    if (radio.rds.region == 0) tftPrint(ALEFT, "PI", 212, 193, ActiveColor, ActiveColorSmooth, 16);
    else {
      tftPrint(ALEFT, "PI", 212, 184, ActiveColor, ActiveColorSmooth, 16);
      tftPrint(ALEFT, "ID", 212, 201, ActiveColor, ActiveColorSmooth, 16);
    }
    tftPrint(ALEFT, "PS", 3, 193, ActiveColor, ActiveColorSmooth, 16);
    tftPrint(ALEFT, "RT", 3, 221, ActiveColor, ActiveColorSmooth, 16);
    tftPrint(ALEFT, "PTY", 3, 163, ActiveColor, ActiveColorSmooth, 16);

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
        if (!screenmute) {
          tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        }
        memorystore = false;
        unsigned long counterold = millis();
        unsigned long counter = millis();
        while (digitalRead(MODEBUTTON) == LOW && counter - counterold <= 1000) counter = millis();

        if (counter - counterold <= 1000) {
          doTuneMode();
        } else {
          if (!BWtune && !menu) {
            menuoption = ITEM1;
            menupage = INDEX;
            menuitem = 0;
            if (spispeed == 7) tft.setSPISpeed(40);
            if (language == LANGUAGE_CHS) PSSprite.setTextFont(1); else PSSprite.setTextFont(0);
            BuildMenu();
            freq_in = 0;
            menu = true;
          }
        }
      } else {
        if (menuopen && !submenu) {
          ButtonPress();
        } else if (!submenu) {
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
                delay(2000);
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
  delay(75);
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
  }

  StoreFrequency();

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
        if (!memorystore) {
          memorystore = true;
          if (!IsStationEmpty()) memoryposstatus = MEM_EXIST;
          else memoryposstatus = MEM_NORMAL;
          ShowMemoryPos();

          ShowTuneMode();
        } else {
          memorystore = false;
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
        unsigned long counter = millis();
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
            delay(200);
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
                tftPrint(ACENTER, "AUTO", 22, 60, BackgroundColor, BackgroundColor, 16);
                tftPrint(ACENTER, "BAND", 22, 60, BackgroundColor, BackgroundColor, 16);
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
          if (!iMSset && !EQset) iMSEQ = 0;
          if (iMSset && EQset) iMSEQ = 2;
          if (!iMSset && EQset) iMSEQ = 3;
          if (iMSset && !EQset) iMSEQ = 4;
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
              else ToggleSWMIBand(REVERSE);
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
              if (displayflip) ToggleSWMIBand(REVERSE);
              else ToggleSWMIBand(true);
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
    if (advancedRDS) tftPrint(ALEFT, String(memorypos + 1), 215, 36, BackgroundColor, BackgroundColor, 16);
    else tftPrint(ALEFT, String(memorypos + 1), 50, 32, BackgroundColor, BackgroundColor, 16);
  }
}

void DoMemoryPosTune() {
  if (spispeed == 7) tft.setSPISpeed(50);
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
          FrequencySprite.setTextColor(ActiveColor, ActiveColorSmooth, false);
          FrequencySprite.setTextDatum(TC_DATUM);
        }

        switch (mode) {
          case 0:
            FrequencySprite.drawString(String(freq / 100) + "." + (freq % 100 < 10 ? "0" : "") + String(freq % 100) + " ", 218, -6, freqfont);
            freqold = freq;
            break;
          case 1: Infoboxprint(textUI(31)); break;
          case 2: Infoboxprint(textUI(287)); break;
          case 3: Infoboxprint(textUI(288)); break;
          case 4: Infoboxprint(textUI(292)); break;
          case 5: Infoboxprint(textUI(281)); break;
        }

        FrequencySprite.pushSprite(46, 46);
        if (mode == 5) delay(1000);
      }
    }
  }

  if (spispeed == 7) setAutoSpeedSPI();
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

void ShowSignalLevel() {
  SAvg = (((SAvg * 9) + 5) / 10) + SStatus;
  SAvg2 = (((SAvg2 * 9) + 5) / 10) + CN;
  SAvg4 = (((SAvg4 * 9) + 5) / 10) + WAM;
  SAvg5 = (((SAvg5 * 9) + 5) / 10) + USN;

  SStatus = SAvg / 10;
  CN = SAvg2 / 10;
  MP = SAvg4 / 10;
  US = SAvg5 / 10;

  if (!screenmute) {
    if (millis() >= SNRupdatetimer + TIMER_SNR_TIMER) {
      SNRupdatetimer = millis();
      if (!advancedRDS) {
        if (CN > (CNold + 1) || CN < (CNold - 1)) {
          if (CNold == 0) tftPrint(ARIGHT, "--", 234, 165, BackgroundColor, BackgroundColor, 16); else tftPrint(ARIGHT, String(CNold), 234, 165, BackgroundColor, BackgroundColor, 16);
          if (tuned) {
            if (CN == 0) tftPrint(ARIGHT, "--", 234, 165, PrimaryColor, PrimaryColorSmooth, 16); else tftPrint(ARIGHT, String(CN), 234, 165, PrimaryColor, PrimaryColorSmooth, 16);
            CNold = CN;
          } else {
            tftPrint(ARIGHT, "--", 234, 165, PrimaryColor, PrimaryColorSmooth, 16);
            CNold = 0;
          }
        }
        byte MPprint = constrain(map(MP, 0, 1000, 0, 99), 0, 99);

        if (MPprint != MPold) {
          tftReplace(ARIGHT, String(MPold), (band < BAND_GAP ? String(MPprint) : "--"), 299, 165, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
          MPold = MPprint;
        }

        byte USprint = constrain(map(US, 0, 1000, 0, 99), 0, 99);

        if (USprint != USold) {
          tftReplace(ARIGHT, String(USold), (band < BAND_GAP ? String(USprint) : "--"), 299, 147, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
          USold = USprint;
        }
      }
    }

    int SStatusprint = 0;
    if (unit == 0) SStatusprint = SStatus;
    if (unit == 1) SStatusprint = ((SStatus * 100) + 10875) / 100;
    if (unit == 2) SStatusprint = round((float(SStatus) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0);

    static int DisplayedSignalSegments = 0;

    if (SStatusprint > (SStatusold + 3) || SStatusprint < (SStatusold - 3)) {
      if (advancedRDS) tftReplace(ARIGHT, String(SStatusold / 10) + "." + String(abs(SStatusold % 10)), String(SStatusprint / 10) + "." + String(abs(SStatusprint % 10)), 273, 51, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
      else {
        if (SStatusold / 10 != SStatusprint / 10) tftReplace(ARIGHT, String(SStatusold / 10), String(SStatusprint / 10), 280, 104, FreqColor, FreqColorSmooth, BackgroundColor, 48);
        tftReplace(ALEFT, "." + String(abs(SStatusold % 10)), "." + String(abs(SStatusprint % 10)), 285, 102, FreqColor, FreqColorSmooth, BackgroundColor, 28);

        // Calculate segments for signal meter
        if (band < BAND_GAP) DisplayedSignalSegments = constrain(map(SStatus / 10, 0, 70, 0, 100), 0, 87);
        else DisplayedSignalSegments = constrain((SStatus + 200) / 10, 0, 87);

        HSV hsv1 = RGB565toHSV(BarInsignificantColor);
        HSV hsv2 = RGB565toHSV(BarSignificantColor);

        int gradientStart = (87 * 25) / 100;
        int gradientEnd = (87 * 60) / 100;

        // Draw solid color for first 25%
        for (int i = 0; i < min(DisplayedSignalSegments, gradientStart); i++) tft.fillRect(16 + 2 * i, 105, 2, 6, BarInsignificantColor);

        if (DisplayedSignalSegments > gradientStart) {
          for (int i = gradientStart; i < min(DisplayedSignalSegments, gradientEnd); i++) {
            float h = map(i, gradientStart, gradientEnd, hsv1.h, hsv2.h);
            float s = map(i, gradientStart, gradientEnd, hsv1.s * 100, hsv2.s * 100) / 100.0;
            float v = map(i, gradientStart, gradientEnd, hsv1.v * 100, hsv2.v * 100) / 100.0;
            tft.fillRect(16 + 2 * i, 105, 2, 6, HSVtoRGB565(h, s, v));
          }
        }

        if (DisplayedSignalSegments > gradientEnd) {
          for (int i = gradientEnd; i < DisplayedSignalSegments; i++) tft.fillRect(16 + 2 * i, 105, 2, 6, BarSignificantColor);
        }

        int greyStart = 16 + 2 * DisplayedSignalSegments;
        int greyWidth = 2 * (87 - DisplayedSignalSegments);
        tft.fillRect(greyStart, 105, greyWidth, 6, GreyoutColor);
      }

      SStatusold = SStatusprint;
    }
  }

  if (!seek) {
    if (tuned && CN > 15 && SStatus > 300) {
      if (!setextendbw) {
        setextendbw = true;
        radio.extendBW(true);
      }
    } else {
      if (setextendbw) {
        setextendbw = false;
        radio.extendBW(false);
      }
    }
  }

  if (wifi && millis() >= udptimer + 2000) {
    udptimer = millis();
    Udp.beginPacket(remoteip, 9030);
    Udp.print("from=TEF_tuner_" + String(stationlistid, DEC) + ";RcvLevel=" + String(SStatus / 10));
    Udp.endPacket();
  }
}

void ShowOffset() {
  if (OStatus != OStatusold) {
    if (millis() >= offsetupdatetimer + TIMER_OFFSET_TIMER) offsetupdatetimer = millis();
    else return;

    int baseX = 13; // Left boundary
    int baseY = 2; // Top boundary
    int width = 4; // Max width
    int height = 26; // Max height

    int centerX = baseX + width / 2;    // Center dot X
    int centerY = baseY + height / 2;   // Center dot Y

    // **Move arrows 3px further from the dot**
    int arrowGap = 3; // Space between dot and arrows
    int arrowWidth = 9; // Arrow stretch size
    int leftArrowBaseX = centerX - arrowWidth - arrowGap;  // Shift left arrow further left
    int rightArrowBaseX = centerX + arrowWidth + arrowGap; // Shift right arrow further right

    int arrowBaseYTop = baseY + 1; // Adjusted top Y position
    int arrowBaseYBottom = baseY + height - 1;

    if (band < BAND_GAP) {
      if (OStatus < -250) {
        // Left arrow active (▶)
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        // Center dot
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        // Right arrow dimmed (◀)
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, SignificantColor);
        tuned = false;
      } else if (USN < 250 && WAM < 250 && OStatus > -250 && OStatus < 250 && !SQ) {
        // Both arrows dimmed
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        // Center dot active
        tft.fillCircle(centerX, centerY, 3, InsignificantColor);
        tuned = true;
      } else if (OStatus > 250) {
        // Right arrow active (◀)
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        // Center dot dimmed
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        // Left arrow dimmed (▶)
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, SignificantColor);
        tuned = false;
      } else {
        // Everything dimmed
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        tuned = false;
      }
    } else {
      if (OStatus < -2) {
        // Left arrow active (▶)
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        // Right arrow dimmed (◀)
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, SignificantColor);
        tuned = false;
      } else if (OStatus >= -2 && OStatus <= 2 && !SQ) {
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillCircle(centerX, centerY, 3, InsignificantColor);
        tuned = true;
      } else if (OStatus > 2) {
        // Right arrow active (◀)
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        // Left arrow dimmed (▶)
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, SignificantColor);
        tuned = false;
      } else {
        tft.fillTriangle(leftArrowBaseX, arrowBaseYTop, centerX - arrowGap, centerY, leftArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillTriangle(rightArrowBaseX, arrowBaseYTop, centerX + arrowGap, centerY, rightArrowBaseX, arrowBaseYBottom, GreyoutColor);
        tft.fillCircle(centerX, centerY, 3, GreyoutColor);
        tuned = false;
      }
    }
    OStatusold = OStatus;
  }
}

void ShowModLevel() {
  if (showmodulation) {
    int segments;
    MStatus = (MStatus > 120) ? 120 : MStatus;

    if (seek || SQ) MStatus = 0;

    segments = constrain(map(MStatus, 0, 120, 0, 86), 0, 86);

    if (segments < DisplayedSegments && (millis() - ModulationpreviousMillis >= 20)) {
      DisplayedSegments = max(DisplayedSegments - 3, segments);
      ModulationpreviousMillis = millis();
    } else if (segments > DisplayedSegments) DisplayedSegments = segments;

    DisplayedSegments = constrain(DisplayedSegments, 0, 86);

    if (DisplayedSegments > peakholdold) {
      peakholdold = DisplayedSegments;
      peakholdmillis = millis();
    }

    if (millis() - peakholdmillis >= 1000) {
      if (millis() - ModulationpeakPreviousMillis >= 20) {
        peakholdold = max(peakholdold - 3, DisplayedSegments);
        ModulationpeakPreviousMillis = millis();
      }
    }

    peakholdold = constrain(peakholdold, 0, 86);

    HSV hsv1 = RGB565toHSV(ModBarInsignificantColor);
    HSV hsv2 = RGB565toHSV(ModBarSignificantColor);

    int gradientStart = (86 * 25) / 100;
    int gradientEnd = (86 * 60) / 100;

    for (int i = 0; i < min(DisplayedSegments, gradientStart); i++) tft.fillRect(16 + 2 * i, 133, 2, 6, ModBarInsignificantColor);

    if (DisplayedSegments > gradientStart) {
      for (int i = gradientStart; i < min(DisplayedSegments, gradientEnd); i++) {
        float h = map(i, gradientStart, gradientEnd, hsv1.h, hsv2.h);
        float s = map(i, gradientStart, gradientEnd, hsv1.s * 100, hsv2.s * 100) / 100.0;
        float v = map(i, gradientStart, gradientEnd, hsv1.v * 100, hsv2.v * 100) / 100.0;
        tft.fillRect(16 + 2 * i, 133, 2, 6, HSVtoRGB565(h, s, v));
      }
    }

    if (DisplayedSegments > gradientEnd) {
      for (int i = gradientEnd; i < DisplayedSegments; i++) tft.fillRect(16 + 2 * i, 133, 2, 6, ModBarSignificantColor);
    }

    int greyStart = 16 + 2 * DisplayedSegments;
    int greyWidth = 2 * (87 - DisplayedSegments);
    tft.fillRect(greyStart, 133, greyWidth, 6, GreyoutColor);

    int peakHoldPosition = 16 + 2 * peakholdold;
    tft.fillRect(peakHoldPosition, 133, 2, 6, (MStatus > 80) ? ModBarSignificantColor : PrimaryColor);

    if (millis() - peakholdmillis >= 1000 && (peakholdold <= DisplayedSegments || peakholdold >= 86)) tft.fillRect(peakHoldPosition, 133, 2, 6, GreyoutColor);
  }
}

void doSquelch() {
  if (!XDRGTKUSB && !XDRGTKTCP && usesquelch && !autosquelch) Squelch = map(analogRead(PIN_POT), 0, 4095, -100, 920);
  if (Squelch < - 800) Squelch = -100;
  if (Squelch > 900) Squelch = 920;

  if (unit == 0) SquelchShow = Squelch / 10;
  if (unit == 1) SquelchShow = ((Squelch * 100) + 10875) / 1000;
  if (unit == 2) SquelchShow = round(((float)Squelch / 10.0 - 10.0 * log10(75) - 90.0) * 10.0) / 10;
  if (Squelch > 920) Squelch = 920;

  if (autosquelch) {
    if (band < BAND_GAP) {
      if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 100 && OStatus > -100) && (!scandxmode || (scandxmode && !scanmute))) {
        if (SQ || BWreset) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
          autosquelchtimer = millis();
          SQ = false;
        }
      } else {
        if ((!SQ || BWreset) && (millis() >= autosquelchtimer + 1000)) {
          radio.setMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
          autosquelchtimer = millis();
          SQ = true;
        }
      }
    } else {
      if ((USN < amscansens * 30) && (OStatus < 2 && OStatus > -2) && (!scandxmode || (scandxmode && !scanmute))) {
        if (!seek) radio.setUnMute();
        if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
        SQ = false;
      } else {
        radio.setMute();
        if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
        SQ = true;
      }
    }
  } else {
    if (!XDRGTKUSB && !XDRGTKTCP && usesquelch && (!scandxmode || (scandxmode && !scanmute))) {
      if (!screenmute && usesquelch && !advancedRDS && !afscreen && !rdsstatscreen) {
        if (!BWtune && !menu && (Squelch > Squelchold + 2 || Squelch < Squelchold - 2)) {
          SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
          SquelchSprite.fillSprite(BackgroundColor);
          if (Squelch == -100) SquelchSprite.drawString("--", 0, 0);
          else if (Squelch == 920) SquelchSprite.drawString("ST", 0, 0);
          else SquelchSprite.drawString(String(SquelchShow), 0, 0);
          SquelchSprite.pushSprite(223, 147);
          Squelchold = Squelch;
        }
      }
    }

    if ((XDRGTKUSB || XDRGTKTCP) && (!scandxmode || (scandxmode && !scanmute))) {
      if (!XDRMute) {
        if (Squelch != -1) {
          if (Squelch < SStatus || Squelch == -100 || Squelch == 0) {
            if (!seek) radio.setUnMute();
            if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
            SQ = false;
          } else {
            radio.setMute();
            if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
            SQ = true;
          }
        } else {
          if (Stereostatus) {
            radio.setUnMute();
            if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
            SQ = false;
          } else {
            radio.setMute();
            if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
            SQ = true;
          }
        }
        if (!screenmute && usesquelch && !advancedRDS && !afscreen && !rdsstatscreen) {
          if (Squelch != Squelchold) {
            SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);
            SquelchSprite.fillSprite(BackgroundColor);

            if (Squelch == -1) SquelchSprite.drawString("ST", 0, 0);
            else if (Squelch == 0) SquelchSprite.drawString("--", 0, 0);
            else SquelchSprite.drawString(String(SquelchShow), 0, 0);
            if (Squelch != Squelchold) SquelchSprite.pushSprite(223, 147);
            Squelchold = Squelch;
          }
        }
      }
    } else if (usesquelch && (!scandxmode || (scandxmode && !scanmute))) {
      if (Squelch != 920) {
        if (Squelch < SStatus || Squelch == -100 || Squelch == 0) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
          SQ = false;
        } else {
          radio.setMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
          SQ = true;
        }
      } else {
        if (Stereostatus) {
          if (!seek) radio.setUnMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
          SQ = false;
        } else {
          radio.setMute();
          if (!screenmute && !seek) tft.drawBitmap(249, 4, Speaker, 28, 24, PrimaryColor);
          SQ = true;
        }
      }
    }
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

void ShowTuneMode() {
  if (!screenmute) {
    switch (tunemode) {
      case TUNE_MAN:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint(ACENTER, "AUTO", 22, 60, GreyoutColor, GreyoutColor, 16);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);
        } else {
          tftPrint(ACENTER, "BAND", 22, 60, GreyoutColor, GreyoutColor, 16);
          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(ACENTER, "AUTO", 22, 60, BackgroundColor, GreyoutColor, 16);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "MAN", 22, 38, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;

      case TUNE_AUTO:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "AUTO", 22, 60, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;

      case TUNE_MEM:
        if (band == BAND_SW && nowToggleSWMIBand) {
          tftPrint(ACENTER, "AUTO", 22, 60, GreyoutColor, GreyoutColor, 16);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);
        } else {
          tftPrint(ACENTER, "BAND", 22, 60, BackgroundColor, GreyoutColor, 16);

          tft.fillRoundRect(2, 58, 40, 18, 2, GreyoutColor);
          tftPrint(ACENTER, "AUTO", 22, 60, BackgroundColor, GreyoutColor, 16);
        }

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        if (memorystore) {
          tft.fillRoundRect(2, 80, 40, 18, 2, SignificantColor);
          tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, SignificantColor, 16);
        } else {
          tft.fillRoundRect(2, 80, 40, 18, 2, SecondaryColor);
          tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor, 16);
        }
        break;

      case TUNE_MI_BAND:
        tft.fillRoundRect(2, 58, 40, 18, 2, SecondaryColor);
        tftPrint(ACENTER, "BAND", 22, 60, BackgroundColor, SecondaryColor, 16);

        tft.fillRoundRect(2, 36, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MAN", 22, 38, BackgroundColor, GreyoutColor, 16);

        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor, 16);
        break;
    }
  }
}

void ShowBattery() {
  if (millis() >= batupdatetimer + TIMER_BAT_TIMER) batupdatetimer = millis();
  else return;

  float v = analogReadMilliVolts(BATTERY_PIN) * 0.002; // 0.002 converts to volts plus corrects the /2 voltage divider
  byte battery = map(constrain(v, BATTERY_LOW_VALUE, BATTERY_FULL_VALUE), BATTERY_LOW_VALUE, BATTERY_FULL_VALUE, 0, BAT_LEVEL_STAGE);
  byte batteryprobe = map(constrain(v, BATTERY_LOW_VALUE, BATTERY_FULL_VALUE), BATTERY_LOW_VALUE, BATTERY_FULL_VALUE, 0, 20);
  if (batteryold != batteryprobe) {
    if (batterydetect) {
      if (battery == 0) {
        tft.drawRoundRect(277, 6, 37, 20, 2, BarSignificantColor);
        tft.fillRoundRect(313, 13, 4, 6, 2, BarSignificantColor);
      } else {
        tft.drawRoundRect(277, 6, 37, 20, 2, ActiveColor);
        tft.fillRoundRect(313, 13, 4, 6, 2, ActiveColor);
      }

      if (batteryoptions != BATTERY_VALUE && batteryoptions != BATTERY_PERCENT && battery != 0) {
        if(v > BATTERY_FULL_VALUE) tft.fillRoundRect(279, 8, 32, 16, 2, ActiveColor);
        else tft.fillRoundRect(279, 8, battery * 8, 16, 2, SecondaryColor);
      } else tft.fillRoundRect(279, 8, 32, 16, 2, BackgroundColor);
    }
    batteryold = batteryprobe;
    batteryVold = 0;
    vPerold = 0;

    if (batterydetect) {
      float vPer = constrain((v - BATTERY_LOW_VALUE) / (BATTERY_FULL_VALUE - BATTERY_LOW_VALUE), 0.0, 1.0) * 100.0;
      if (abs(v - batteryVold) > 0.05 && batteryoptions == BATTERY_VALUE) {
        tftPrint(ALEFT, String(v, 2) + "V", 279, 9, BatteryValueColor, BatteryValueColorSmooth, 16);
        batteryVold = v;
      } else if (int(vPer) != int(vPerold) && batteryoptions == BATTERY_PERCENT && abs(vPer - vPerold) > 0.5) {
        tftPrint(ALEFT, String(vPer, 0) + "%", 279, 9, BatteryValueColor, BatteryValueColorSmooth, 16);
        vPerold = vPer;
      }
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
  if (stepsize == 1) temp = 1;
  else if (stepsize == 2) temp = 10;
  else if (stepsize == 3) temp = 100;
  else if (stepsize == 4) temp = 1000;
  if (rotaryaccelerate && rotarycounter > 2) temp *= 2;

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
  if (stepsize == 1) temp = 1;
  else if (stepsize == 2) temp = 10;
  else if (stepsize == 3) temp = 100;
  else if (stepsize == 4) temp = 1000;
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
    if (band == BAND_FM) DataPrint("M0\nT" + String(frequency * 10) + "\n"); else if (band == BAND_OIRT) DataPrint("M0\nT" + String(frequency_OIRT * 10) + "\n"); else DataPrint("M1\nT" + String(frequency_AM) + "\n");
  }

  if (band < BAND_GAP) {
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
    if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80 && OStatus > -80) && (!usesquelch || (Squelch < SStatus || Squelch == 920))) {
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
    radio.getStatusAM(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
    if ((USN < amscansens * 30) && (OStatus < 2 && OStatus > -2) && (!usesquelch || (Squelch < SStatus || Squelch == 920))) {
      seek = false;
      radio.setUnMute();
      if (!screenmute) tft.drawBitmap(249, 4, Speaker, 28, 24, GreyoutColor);
      store = true;
    } else seek = true;
  }
}

void read_encoder() {
  if (!digitalRead(ROTARY_PIN_A) || !digitalRead(ROTARY_PIN_B)) {
    uint32_t dt = millis() - rotarytimer;
    if (dt >= 45) {
      rotarycounteraccelerator = 6;
      rotarycounter = 0;
    } else if (dt >= 30) {
      rotarycounteraccelerator = 4;
      rotarycounter = 0;
    } else if (dt >= 15) {
      rotarycounteraccelerator = 2;
      rotarycounter = 0;
    }
  }

  static uint8_t old_AB = 3;
  static int8_t encval = 0;
  static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

  old_AB <<= 2;
  if (digitalRead(ROTARY_PIN_A)) old_AB |= 0x02;
  if (digitalRead(ROTARY_PIN_B)) old_AB |= 0x01;
  encval += enc_states[old_AB & 0xf];
  if (!(255 - old_AB)) encval = 0;

  if (encval > 3) {
    if (rotarymode) rotary = -1; else rotary = 1;
    rotarycounter++;
    rotarytimer = millis();
    encval = 0;
  } else if (encval < -3) {
    if (rotarymode) rotary = 1; else rotary = -1;
    rotarycounter++;
    rotarytimer = millis();
    encval = 0;
  }
}

void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize) {
  uint8_t selectedFont = 0;
  if (fontsize == 28) selectedFont = 1;
  else if (fontsize == 48) selectedFont = 2;

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  tft.setTextColor(background, background, false);
  tft.drawString(textold, x, y, selectedFont);

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.setTextColor(color, smoothcolor, false);
  tft.drawString(modifiedText, x, y, selectedFont);
}

void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize) {
  uint8_t selectedFont = 0;
  if (fontsize == 28) selectedFont = 1;
  else if (fontsize == 48) selectedFont = 2;

  tft.setTextColor(color, smoothcolor, (fontsize == 52 ? true : false));

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.drawString(modifiedText, x, y, selectedFont);
}

void UpdateFonts() {
  if (language == LANGUAGE_CHS) {
    if (menu) PSSprite.setTextFont(1); else PSSprite.setTextFont(3);
    OneBigLineSprite.loadFont(FONT28_CHS, 0);
    FullLineSprite.loadFont(FONT16_CHS, 0);
    GeneralTextSprite.loadFont(FONT16_CHS, 0);
    GeneralTextSprite.loadFont(FONT28_CHS, 1);
    FrequencySprite.loadFont(FONT16_CHS, 6);
    tft.loadFont(FONT16_CHS, 0);
    tft.loadFont(FONT28_CHS, 1);
    SquelchSprite.loadFont(FONT16, 0);
  } else {
    if (menu) PSSprite.setTextFont(0); else PSSprite.setTextFont(2);
    OneBigLineSprite.loadFont(FONT28, 0);
    FullLineSprite.loadFont(FONT16, 0);
    GeneralTextSprite.loadFont(FONT16, 0);
    GeneralTextSprite.loadFont(FONT28, 1);
    FrequencySprite.loadFont(FONT16, 6);
    tft.loadFont(FONT16, 0);
    tft.loadFont(FONT28, 1);
    SquelchSprite.loadFont(FONT16_CHS, 0);
  }
}

uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi) {
  uint8_t error = 0;
  uint8_t counter = 0;
  uint8_t rangestart = startmem;
  uint16_t _current = frequency;
  uint16_t totalIterations = (stopfreq - startfreq) + 1;
  uint16_t currentIteration = 0;
  uint8_t percent = 0;
  uint8_t percentold = 0;
  bool stopScanning = false;
  bool dostore = false;

  radio.setMute();
  radio.power(0);
  band = BAND_FM;
  tunemode = TUNE_MAN;

  tft.drawRect(59, 109, 202, 8, FrameColor);
  tft.fillRect(60, 110, 200, 6, GreyoutColor);
  tftPrint(ARIGHT, textUI(269), 120, 155, ActiveColor, ActiveColorSmooth, 16);

  for (frequency = startfreq * 10; frequency <= stopfreq * 10; frequency += 10) {
    if (stopScanning) break;

    currentIteration++;
    percent = (currentIteration * 100) / totalIterations;

    radio.SetFreq(frequency);
    radio.clearRDS();
    delay(50);
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);
    if ((USN < fmscansens * 30) && (WAM < 230) && (OStatus < 80 && OStatus > -80)) {
      for (byte y = 0; y < 20; y++) {
        delay(50);
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
