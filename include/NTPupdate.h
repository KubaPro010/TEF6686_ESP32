#pragma once
#include "globals.h"
#include "rtc.hpp"

static constexpr char ntpServerName[] = "0.pool.ntp.org";
static constexpr int localPort = 8944;
constexpr int NTP_PACKET_SIZE = 48;

void sendNTPpacket(IPAddress &address);
void NTPupdate();
time_t getNtpTime();
