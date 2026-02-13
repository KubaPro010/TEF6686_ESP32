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
#include "main.h"

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

extern bool isDST(time_t t);
