#include "globals.h"
#include "core.h"
#include "main.h"
#include "gui.h"
#include "nonvolatile.h"
#include "FONT16.h"
#include "FONT16_CHS.h"
#include "FONT28.h"
#include "FONT28_CHS.h"
#include "FONT48DEC.h"
#include "FREQFONT.h"

void Touch_IRQ_Handler() {
  touch_detect = true;
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

void SetTunerPatch() {
  if(FORBIDDEN_TUNER(TEF)) {
    radio.init(102);
    uint16_t hw, sw;
    radio.getIdentification(&hw, &sw);
    TEF = highByte(hw) * 100 + highByte(sw);
    tft.fillScreen(BackgroundColor);
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));

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

void setup_periph() {
  Wire.setClock(400000);
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
  device = radio.getIdentification(&hw, &sw);
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
  if(fmsi) radio.setFMSI(2);

  if(rx_rtc_avail) {
    bool reset = init_rtc();
    if(reset) console.print("RX8010SJ was reset, no time");
    else {
      rtcset = true;
      console.print("RX8010SJ is used as a time source");
    }
  } else console.print("RX8010SJ is not available at address " + String(RX8010SJ_ADDRESS, HEX));

  if(analogRead(BATTERY_PIN) < BATTERY_DETECT_THRESHOLD) batterydetect = false;
  else console.print("Battery detected");
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  gpio_set_drive_capability((gpio_num_t)5, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)16, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)17, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)18, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)19, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)21, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)22, GPIO_DRIVE_CAP_0);
  gpio_set_drive_capability((gpio_num_t)23, GPIO_DRIVE_CAP_0);

  analogWriteFrequency(5000);

  EEPROM.begin(EE_TOTAL_CNT);

  setupmode = true;
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(100000);

  loadData();

  if (spispeed == 0) setAutoSpeedSPI();
  else tft.setSPISpeed(spispeed);

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

  if (USBmode) Serial.updateBaudRate(19200); else Serial.updateBaudRate(115200);

  if(iMSset && EQset) iMSEQ = 2;
  else if(!iMSset && EQset) iMSEQ = 3;
  else if(iMSset && !EQset) iMSEQ = 4;
  else iMSEQ = 1;

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

  tft.loadFont(FONT16_CHS, 0);
  tft.loadFont(FONT48, 1);
  tft.loadFont(FONT16, 2);
  tft.loadFont(FONT28, 3);
  tft.loadFont(FONT28_CHS, 4);

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

  SPIFFS.begin();

  FrequencySprite.createSprite(200, 50);
  FrequencySprite.setTextDatum(TR_DATUM);
  FrequencySprite.loadFont(FREQFONT0, 0);
  FrequencySprite.loadFont(FREQFONT1, 1);
  FrequencySprite.loadFont(FREQFONT2, 2);
  FrequencySprite.loadFont(FREQFONT3, 3);
  FrequencySprite.loadFont(FREQFONT4, 4);
  FrequencySprite.loadFont(FREQFONT5, 5);

  GeneralTextSprite.createSprite(308, 28);
  GeneralTextSprite.copyFontFromTFT(2, 2);

  PSSprite.createSprite(150, 32);
  PSSprite.copyFontFromTFT(2, 0);
  PSSprite.copyFontFromTFT(0, 1);
  PSSprite.copyFontFromTFT(3, 2);
  PSSprite.copyFontFromTFT(4, 3);

  SquelchSprite.createSprite(27, 19);

  FullLineSprite.createSprite(308, 19);

  OneBigLineSprite.createSprite(270, 30);

  SignalSprite.createSprite(80, 48);
  SignalSprite.copyFontFromTFT(4, 0);
  SignalSprite.setTextColor(PrimaryColor, PrimaryColorSmooth, false);

  UpdateFonts();
  tft.fillScreen(BackgroundColor);

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    if (rotarymode == 0) rotarymode = 1; else rotarymode = 0;
    EEPROM.writeByte(EE_BYTE_ROTARYMODE, rotarymode);
    EEPROM.commit();
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
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
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
    Infoboxprint(textUI(3));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(MODEBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == HIGH && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
    Infoboxprint(textUI(4));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(BANDBUTTON) == LOW) delay(50);
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == LOW && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
    DefaultSettings();
    Infoboxprint(textUI(63));
    tftPrint(ACENTER, textUI(2), 155, 130, ActiveColor, ActiveColorSmooth, 28);
    while (digitalRead(ROTARY_BUTTON) == LOW && digitalRead(BWBUTTON) == LOW) delay(50);
    esp_restart();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == LOW && digitalRead(BANDBUTTON) == HIGH) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
    Infoboxprint(textUI(278));
    tftPrint(ACENTER, textUI(279), 155, 100, ActiveColor, ActiveColorSmooth, 28);
    tft.calibrateTouch(TouchCalData, PrimaryColor, BackgroundColor, 30);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH1, TouchCalData[0]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH2, TouchCalData[1]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH3, TouchCalData[2]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH4, TouchCalData[3]);
    EEPROM.writeUInt(EE_UINT16_CALTOUCH5, TouchCalData[4]);
    EEPROM.commit();
  }

  if (digitalRead(BWBUTTON) == LOW && digitalRead(ROTARY_BUTTON) == HIGH && digitalRead(MODEBUTTON) == HIGH && digitalRead(BANDBUTTON) == LOW) {
    analogWrite(CONTRASTPIN, map(ContrastSet, 0, 100, 0, 255));
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

  tft.pushImage((tft.width() - 163) / 2, (tft.height() - 84) / 2, 163, 84, (uint16_t*)radiologo, TFT_BLACK);
  tft.drawBitmap((tft.width() - 59) / 2, 24, TEFLogo, 59, 23, ActiveColor);

  for (int x = 0; x <= ContrastSet; x++) {
    analogWrite(CONTRASTPIN, map(x, 0, 100, 0, 255));
    delay(5);
  }

  console.print("Firmware " + String(VERSION));
  console.print("Compiled at " + String(__TIME__) + " " + String(__DATE__));

  if(!tef_found) {
    console.print(textUI(6));
    while (true) delay(100);
  }

  TEF = EEPROM.readByte(EE_BYTE_TEF);
  if(FORBIDDEN_TUNER(TEF)) SetTunerPatch();

  if(esp_reset_reason() != ESP_RST_DEEPSLEEP) later_setup_periph();

  if(!SPIFFS.exists("/logbook.csv")) {
    handleCreateNewLogbook();
    console.print("Creating a new logbook.");
  }

  if (wifi) {
    console.print("Trying WiFi");
    tryWiFi();
    delay(1750);
  } else {
    Server.end();
    Udp.stop();
  }

  while(digitalRead(ROTARY_BUTTON) == LOW) delay(100);

  console.print("Init done.");

  LowLevelInit = true;

  BuildDisplay();
  SelectBand();
  if (tunemode == TUNE_MEM) DoMemoryPosTune();

  eccDisplay.setPostDrawCallback([&](TFT_eSprite* sprite) {
    sprite->fillRect(275, 0, 10, 19, BackgroundColor);
    sprite->drawLine(284, 0, 284, 19, FrameColor);
  });
  rtDisplay.setPostDrawCallback([&](TFT_eSprite* sprite) {
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