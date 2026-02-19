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
#include "touch.h"
#include "rds.h"
#include "logbook.h"
#include "comms.h"
#include "graphics.h"

void Touch_IRQ_Handler() {
  touch_detect = true;
}

void read_encoder() {
  if(i2c_pc_control) return;
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

  analogWriteFrequency(6700); // six seven

  EEPROM.begin(EE_TOTAL_CNT);

  setupmode = true;
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(100000);

  loadData();

  pinMode(MODEBUTTON, INPUT);
  if(i2c_pc_control && digitalRead(MODEBUTTON) == HIGH) return;
  else i2c_pc_control = false;

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

void handleWiFi() {
  if (wifi && !menu) {
    webserver.handleClient();

    if (millis() >= udplogtimer + UDP_LOG_INTERVAL_MS) {
      sendUDPlog();
      udplogtimer = millis();
    }

    if (millis() >= NTPtimer + NTP_UPDATE_INTERVAL_MS) {
      NTPupdate();
      NTPtimer = millis();
    }
  }
}

void handleTouch() {
  if (hardwaremodel == PORTABLE_TOUCH_ILI9341 && touch_detect) {
    if (tft.getTouchRawZ() > TOUCH_RAW_Z_THRESHOLD) {
      uint16_t x, y;
      tft.getTouch(&x, &y);
      if (x > 0 || y > 0) {
        if (!firstTouchHandled) {
          doTouchEvent(x, y);
          firstTouchHandled = true;
          lastTouchTime = millis();
        } else if (touchrepeat && millis() - lastTouchTime >= DELAY_TOUCH_REPEAT_MS) doTouchEvent(x, y);
      }
    } else {
      firstTouchHandled = false;
      touch_detect = false;
    }
  }
}

void handleTimers() {
  // tottimer is like the time of the last interaction
  if (tot != 0 && millis() >= tottimer + (tot * TOT_MULTIPLIER_MS)) deepSleep();

  if (freq_in != 0 && millis() >= keypadtimer + DELAY_KEYPAD_TIMEOUT_MS) {
    freq_in = 0;
    ShowFreq(0);
    if (tunemode == TUNE_MEM) ShowMemoryPos();
  }
}

void ShowAudioLevel() {
  if(showaudio) {
    int segments;
    MStatus = (MStatus > 120) ? 120 : MStatus;

    if (seek || SQ) MStatus = 0;

    segments = constrain(map(MStatus, 0, 120, 0, 86), 0, 86);

    if (segments < DisplayedSegments && (millis() - AudiopreviousMillis >= 20)) {
      DisplayedSegments = max(DisplayedSegments - 3, segments);
      AudiopreviousMillis = millis();
    } else if (segments > DisplayedSegments) DisplayedSegments = segments;

    DisplayedSegments = constrain(DisplayedSegments, 0, 86);

    if (DisplayedSegments > peakholdold) {
      peakholdold = DisplayedSegments;
      peakholdmillis = millis();
    }

    if (millis() - peakholdmillis >= 1000) {
      if (millis() - AudiopeakPreviousMillis >= 20) {
        peakholdold = max(peakholdold - 3, DisplayedSegments);
        AudiopeakPreviousMillis = millis();
      }
    }

    peakholdold = constrain(peakholdold, 0, 86);

    // Skip redraw when nothing changed
    static int prevDisplayedSegments = -1;
    static int prevPeakhold = -1;
    if (!modLevelForceRedraw && DisplayedSegments == prevDisplayedSegments && peakholdold == prevPeakhold) return;
    modLevelForceRedraw = false;

    // Pre-computed gradient color lookup table (cached, recomputed only on theme change)
    static uint16_t modGradient[87];
    static uint16_t cachedInsigColor = 0;
    static uint16_t cachedSigColor = 0;
    static bool gradientReady = false;

    if (!gradientReady || cachedInsigColor != ModBarInsignificantColor || cachedSigColor != ModBarSignificantColor) {
      HSV hsv1 = RGB565toHSV(ModBarInsignificantColor);
      HSV hsv2 = RGB565toHSV(ModBarSignificantColor);
      int gStart = (86 * 25) / 100;
      int gEnd = (86 * 60) / 100;
      for (int i = 0; i < 87; i++) {
        if (i < gStart) modGradient[i] = ModBarInsignificantColor;
        else if (i < gEnd) {
          float h = map(i, gStart, gEnd, hsv1.h, hsv2.h);
          float s = map(i, gStart, gEnd, hsv1.s * 100, hsv2.s * 100) / 100.0;
          float v = map(i, gStart, gEnd, hsv1.v * 100, hsv2.v * 100) / 100.0;
          modGradient[i] = HSVtoRGB565(h, s, v);
        } else modGradient[i] = ModBarSignificantColor;
      }
      cachedInsigColor = ModBarInsignificantColor;
      cachedSigColor = ModBarSignificantColor;
      gradientReady = true;
    }

    // Draw bar segments using cached gradient colors
    for (int i = 0; i < DisplayedSegments; i++) tft.fillRect(16 + 2 * i, 133, 2, 6, modGradient[i]);

    int greyStart = 16 + 2 * DisplayedSegments;
    int greyWidth = 2 * (87 - DisplayedSegments);
    tft.fillRect(greyStart, 133, greyWidth, 6, GreyoutColor);

    int peakHoldPosition = 16 + 2 * peakholdold;
    tft.fillRect(peakHoldPosition, 133, 2, 6, (MStatus > 80) ? ModBarSignificantColor : PrimaryColor);

    if (millis() - peakholdmillis >= 1000 && (peakholdold <= DisplayedSegments || peakholdold >= 86)) tft.fillRect(peakHoldPosition, 133, 2, 6, GreyoutColor);

    prevDisplayedSegments = DisplayedSegments;
    prevPeakhold = peakholdold;
  }
}

void doLog() {
  if (!autologged && RDSstatus && radio.rds.correctPI != 0) {
    if (autolog) {
      switch (addRowToCSV()) {
        case 0: ShowFreq(2); break;
        case 1: ShowFreq(3); break;
        case 2: ShowFreq(4); break;
      }

      delay(DELAY_UI_UPDATE_MS);
      ShowFreq(0);
    }
    autologged = true;
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
      if (isSignalQualityGood(USN, WAM, OStatus, SCAN_SIGNAL_THRESHOLD_USN_MULTIPLIER, SCAN_SIGNAL_THRESHOLD_OSTATUS_WIDE) && (!scandxmode || (scandxmode && !scanmute))) {
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
            SquelchSprite.setTextColor(PrimaryColor, PrimaryColorSmooth);
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
          if (CNold == 0) tftPrint16(ARIGHT, "--", 234, 165, BackgroundColor, BackgroundColor); else tftPrint16(ARIGHT, String(CNold), 234, 165, BackgroundColor, BackgroundColor);
          if (tuned) {
            if (CN == 0) tftPrint16(ARIGHT, "--", 234, 165, PrimaryColor, PrimaryColorSmooth); else tftPrint16(ARIGHT, String(CN), 234, 165, PrimaryColor, PrimaryColorSmooth);
            CNold = CN;
          } else {
            tftPrint16(ARIGHT, "--", 234, 165, PrimaryColor, PrimaryColorSmooth);
            CNold = 0;
          }
        }

        byte MPprint = constrain(map(MP, 0, 1000, 0, 100), 0, 100);
        if (MPprint != MPold) {
          tftReplace(ARIGHT, String(MPold), (band < BAND_GAP ? String(MPprint) : "--"), 299, 165, PrimaryColor, PrimaryColorSmooth, BackgroundColor, 16);
          MPold = MPprint;
        }

        byte USprint = constrain(map(US, 0, 1000, 0, 100), 0, 100);
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

void ShowStereoStatus() {
  if (StereoToggle) {
    if (band < BAND_GAP) Stereostatus = radio.getStereoStatus(); else Stereostatus = false;
    if (Stereostatus != Stereostatusold) {
      if (Stereostatus && !screenmute) tft.drawBitmap(STEREO_ICON_X, STEREO_ICON_Y, Stereo, 32, 22, StereoColor);
      else if (!screenmute) tft.drawBitmap(STEREO_ICON_X, STEREO_ICON_Y, Stereo, 32, 22, GreyoutColor);
      Stereostatusold = Stereostatus;
    }
  }
}

void ShowRSSI() {
  if (wifi) rssi = WiFi.RSSI(); else rssi = 0;
  if (rssiold != rssi) {
    if (!wifi && batterydetect) tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, BackgroundColor);
    else if (rssi == 0) tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, GreyoutColor);
    else if (rssi > -50 && rssi < 0) tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, WifiColorHigh);
    else if (rssi > -60) {
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, GreyoutColor);
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi3, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, WifiColorHigh);
    } else if (rssi > -70) {
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, GreyoutColor);
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi2, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, WifiColorLow);
    } else if (rssi < -70) {
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi4, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, GreyoutColor);
      tft.drawBitmap(WIFI_ICON_X, WIFI_ICON_Y, WiFi1, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, WifiColorLow);
    }
    rssiold = rssi;
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

inline __attribute__((always_inline)) void updateCodetect() {
  if (band > BAND_GAP) {
    if (WAM) tftPrint16(ALEFT, "CO", 50, 61, PrimaryColor, PrimaryColorSmooth);
    else tftPrint16(ALEFT, "CO", 50, 61, BackgroundColor, BackgroundColor);
  }
}

void ShowBattery() {
  if (millis() >= batupdatetimer + TIMER_BAT_TIMER) batupdatetimer = millis();
  else return;

  float v = analogReadMilliVolts(BATTERY_PIN) * 0.002; // 0.002 converts to volts plus corrects the /2 voltage divider
  byte battery = map(constrain(v, BATTERY_LOW_VALUE, BATTERY_FULL_VALUE), BATTERY_LOW_VALUE, BATTERY_FULL_VALUE, 0, 32);
  byte batteryprobe = map(constrain(v, BATTERY_LOW_VALUE, BATTERY_FULL_VALUE), BATTERY_LOW_VALUE, BATTERY_FULL_VALUE, 0, 50);
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
        else tft.fillRoundRect(279, 8, battery, 16, 2, SecondaryColor);
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

inline __attribute__((always_inline)) const char* getBWButtonLabel(byte temp, bool isFM) {
  constexpr byte BW_OK_BUTTON = 20;
  if (temp == BW_OK_BUTTON) return "OK";
  return isFM ? BWButtonLabelsFM[temp - 1] : BWButtonLabelsAM[temp - 1];
}
inline __attribute__((always_inline)) bool shouldHighlightBWButton(byte temp, byte set) {
  return (set == temp) || (temp == 17 && set == 0) || (temp == 18 && !iMSset) || (temp == 19 && !EQset);
}

void doBWtuneUp() {
  rotary = 0;
  constexpr byte BW_OK_BUTTON = 20;
  constexpr byte BW_AM_MAX = 4;

  if (band < BAND_GAP) {
    // FM band
    drawButton(getBWButtonLabel(BWsettemp, true), BWsettemp - 1, shouldHighlightBWButton(BWsettemp, BWset), false);
    BWsettemp++;
    if (BWsettemp > BW_OK_BUTTON) BWsettemp = 1;
    drawButton(getBWButtonLabel(BWsettemp, true), BWsettemp - 1, shouldHighlightBWButton(BWsettemp, BWset), true);
  } else {
    // AM band
    drawButton(getBWButtonLabel(BWsettemp, false), BWsettemp - 1, (BWset == BWsettemp), false);
    BWsettemp++;
    if (BWsettemp > BW_AM_MAX && BWsettemp < BW_OK_BUTTON) BWsettemp = BW_OK_BUTTON;
    else if (BWsettemp > BW_OK_BUTTON) BWsettemp = 1;
    drawButton(getBWButtonLabel(BWsettemp, false), BWsettemp - 1, (BWset == BWsettemp), true);
  }
}

void doBWtuneDown() {
  rotary = 0;
  constexpr byte BW_OK_BUTTON = 20;
  constexpr byte BW_AM_MAX = 4;

  if (band < BAND_GAP) {
    // FM band
    drawButton(getBWButtonLabel(BWsettemp, true), BWsettemp - 1, shouldHighlightBWButton(BWsettemp, BWset), false);
    BWsettemp--;
    if (BWsettemp > BW_OK_BUTTON || BWsettemp == 0) BWsettemp = BW_OK_BUTTON;
    drawButton(getBWButtonLabel(BWsettemp, true), BWsettemp - 1, shouldHighlightBWButton(BWsettemp, BWset), true);
  } else {
    // AM band
    drawButton(getBWButtonLabel(BWsettemp, false), BWsettemp - 1, (BWset == BWsettemp), false);
    BWsettemp--;
    if (BWsettemp > BW_AM_MAX && BWsettemp < BW_OK_BUTTON) BWsettemp = BW_AM_MAX;
    else if (BWsettemp == 0) BWsettemp = BW_OK_BUTTON;
    drawButton(getBWButtonLabel(BWsettemp, false), BWsettemp - 1, (BWset == BWsettemp), true);
  }
}

void loop() {
  if(i2c_pc_control) {
    total_pc_control();
    if(i2c_pc_control) return;
    if(EEPROM.readByte(EE_BYTE_CONTROLMODE)) {
      saveData();
      esp_restart();
    }
  }

  handleWiFi();
  handleTouch();
  Communication();
  handleTimers();

  if (scandxmode) {
    unsigned long waitTime = (scanhold == 0) ? SCAN_HOLD_DEFAULT_MS : (scanhold * 1000);
    if (!scanholdflag) scanholdflag = isSignalQualityGood(USN, WAM, OStatus);
    bool bypassMillisCheck = scanholdonsignal && !scanholdflag;
    bool shouldScan = bypassMillisCheck || (!bypassMillisCheck && (millis() >= scantimer + waitTime));

    if (shouldScan) {
      if (scanmute && scanholdonsignal) {
        radio.setMute();
        if (!screenmute) tft.drawBitmap(SPEAKER_ICON_X, SPEAKER_ICON_Y, Speaker, SPEAKER_ICON_WIDTH, SPEAKER_ICON_HEIGHT, PrimaryColor);
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
        tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, SecondaryColor);
      } else {
        tft.fillRoundRect(2, 80, 40, 18, 2, GreyoutColor);
        tftPrint16(ACENTER, "MEM", 22, 82, BackgroundColor, GreyoutColor);
      } flashingtimer = millis();
    }

    if (!scanholdflag) delay(75);
    radio.getStatus(&SStatus, &USN, &WAM, &OStatus, &BW, &MStatus, &CN);

    if (!initdxscan) {
      switch (scancancel) {
        case CORRECTPI:
          if (RDSstatus && radio.rds.correctPI != 0) cancelDXScan();
          break;
        case SIGNAL:
          if (isSignalQualityGood(USN, WAM, OStatus) && (Squelch < SStatus || Squelch == SQUELCH_MAX_VALUE)) cancelDXScan();
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
              if (!screenmute && !advancedRDS) ShowAudioLevel();
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

  if(seek) Seek(direction);

  if ((SStatus / 10 > LowLevelSet) && !LowLevelInit && !BWtune && !menu && band < BAND_GAP) {
    if (!screenmute && !advancedRDS && !rdsstatscreen && !afscreen) {
      if (showaudio) {
        tftPrint16(ALEFT, "10", 24, 144, ActiveColor, ActiveColorSmooth);
        tftPrint16(ALEFT, "30", 54, 144, ActiveColor, ActiveColorSmooth);
        tftPrint16(ALEFT, "50", 84, 144, ActiveColor, ActiveColorSmooth);
        tftPrint16(ALEFT, "70", 114, 144, ActiveColor, ActiveColorSmooth);
        tftPrint16(ALEFT, "100", 160, 144, ActiveColor, ActiveColorSmooth);
        tftPrint16(ACENTER, "A", 7, 128, ActiveColor, ActiveColorSmooth);
        for (byte segments = 0; segments < 93; segments++) {
          if (segments > 54) {
            if (((segments - 53) % 10) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, BarSignificantColor);
          } else {
            if (((segments + 1) % 6) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, BarInsignificantColor);
          }
        }
      }
      if (radio.rds.region == 0) tftPrint16(ALEFT, "PI", 212, 193, ActiveColor, ActiveColorSmooth);
      else {
        tftPrint16(ALEFT, "PI", 212, 184, ActiveColor, ActiveColorSmooth);
        tftPrint16(ALEFT, "ID", 212, 201, ActiveColor, ActiveColorSmooth);
      }
      tftPrint16(ALEFT, "PS", 3, 193, ActiveColor, ActiveColorSmooth);
      tftPrint16(ALEFT, "RT", 3, 221, ActiveColor, ActiveColorSmooth);
      tftPrint16(ALEFT, "PTY", 3, 163, ActiveColor, ActiveColorSmooth);
      if (!showaudio) tft.drawLine(16, 143, 189, 143, GreyoutColor); else tft.drawLine(16, 143, 189, 143, ActiveColor);
    }
    LowLevelInit = true;
  }

  if ((SStatus / 10 <= LowLevelSet) && band < BAND_GAP) {
    if (LowLevelInit && !BWtune && !menu) {
      if (!screenmute && !rdsstatscreen && !afscreen && !advancedRDS) {
        if(showaudio) {
          tftPrint16(ALEFT, "10", 24, 144, GreyoutColor, BackgroundColor);
          tftPrint16(ALEFT, "30", 54, 144, GreyoutColor, BackgroundColor);
          tftPrint16(ALEFT, "50", 84, 144, GreyoutColor, BackgroundColor);
          tftPrint16(ALEFT, "70", 114, 144, GreyoutColor, BackgroundColor);
          tftPrint16(ALEFT, "100", 160, 144, GreyoutColor, BackgroundColor);
          tftPrint16(ACENTER, "A", 7, 128, GreyoutColor, BackgroundColor);
          tft.fillRect(16, 133, 174, 6, GreyoutColor);
          for (byte segments = 0; segments < 93; segments++) {
            if (segments > 54) {
              if (((segments - 53) % 10) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, GreyoutColor);
            } else {
              if (((segments + 1) % 6) == 0) tft.fillRect(22 + (2 * segments), 141, 2, 2, GreyoutColor);
            }
          }
        }
        if (radio.rds.region == 0) tftPrint16(ALEFT, "PI", 212, 193, GreyoutColor, BackgroundColor);
        else {
          tftPrint16(ALEFT, "PI", 212, 184, GreyoutColor, BackgroundColor);
          tftPrint16(ALEFT, "ID", 212, 201, GreyoutColor, BackgroundColor);
        }
        tftPrint16(ALEFT, "PS", 3, 193, GreyoutColor, BackgroundColor);
        tftPrint16(ALEFT, "RT", 3, 221, GreyoutColor, BackgroundColor);
        tftPrint16(ALEFT, "PTY", 3, 163, GreyoutColor, BackgroundColor);
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
      if (!screenmute && !rdsstatscreen && !afscreen && !advancedRDS) ShowAudioLevel();
    }
  }

  if (rotary == -1) {
    tottimer = millis();
    if (screensavertriggered) {
      if (!touchrotating) {
        rotary = 0;
        WakeToSleep(false);
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
        WakeToSleep(false);
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
      WakeToSleep(false);
      while (digitalRead(BANDBUTTON) == LOW);
    } else BANDBUTTONPress();
  }

  if (digitalRead(ROTARY_BUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(false);
      while (digitalRead(ROTARY_BUTTON) == LOW);
    } else if (!afscreen && !rdsstatscreen) ButtonPress();
  }

  if (digitalRead(MODEBUTTON) == LOW) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(false);
      while (digitalRead(MODEBUTTON) == LOW);
    } else if(!screenmute) ModeButtonPress();
  }

  if (digitalRead(BWBUTTON) == LOW && !BWtune) {
    tottimer = millis();
    if (screensavertriggered) {
      WakeToSleep(false);
      while (digitalRead(BWBUTTON) == LOW);
    } else if(!screenmute) BWButtonPress();
  }

  if (digitalRead(EXT_IRQ) == LOW) {
    if (screensavertriggered) WakeToSleep(false);
    int num = GetNum();
    if (!screenmute && !BWtune && !menu && !advancedRDS && !rdsstatscreen && !afscreen && num != -1) NumpadProcess(num);
  }

  if (screensaverset > 0 && !screensavertriggered && !BWtune && !menu && millis() >= screensavertimer + 1000 * screensaverOptions[screensaverset]) WakeToSleep(true);
}