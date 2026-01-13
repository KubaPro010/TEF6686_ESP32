#pragma once

#include <ctime>
#include "constants.h"
#include <ESP32Time.h>
#include <RX8010SJ.h>

extern bool rtcset;
extern bool NTPupdated;

extern ESP32Time rtc;
extern bool rx_rtc_avail;
extern RX8010SJ::Adapter rx_rtc;

void sync_from_rx_rtc(int32_t offset = 0);
void set_time(time_t time);