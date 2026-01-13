#pragma once
#include "globals.h"
#include "rtc.hpp"

static const char ntpServerName[] = "0.pool.ntp.org";
static const int localPort = 8944;
const int NTP_PACKET_SIZE = 48;

void sendNTPpacket(IPAddress &address);
void NTPupdate();
time_t getNtpTime();
