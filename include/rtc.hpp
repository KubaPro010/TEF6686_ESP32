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
void set_time(time_t time);