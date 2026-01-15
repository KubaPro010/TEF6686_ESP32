#pragma once

#include <ctime>
#include "constants.h"
#include <ESP32Time.h>
#include <Wire.h>

extern bool rtcset;
extern bool NTPupdated;

extern ESP32Time rtc;
extern bool rx_rtc_avail;

bool init_rtc();
void sync_from_rx_rtc(int32_t offset = 0);
void set_time(time_t time);