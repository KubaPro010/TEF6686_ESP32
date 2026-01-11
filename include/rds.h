#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFiClient.h>
#include "TEF6686.h"
#include <WiFi.h>
#include <ESP32Time.h>
#include "globals.h"
#include "NTPupdate.h"
#include "rtc.hpp"

void ShowAdvancedRDS();
void readRds();
void doAF();
void showPI();
void showPTY();
void showPS();
void showRadioText();
void ShowAFEON();
void showCT();
void ShowErrors();
void ShowRDSStatistics();

extern void ShowRDSLogo(bool RDSstatus);
extern void DataPrint(String string);
extern void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize);
extern void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize);
extern bool isDST(time_t t);
extern const char* textUI(uint16_t number);
