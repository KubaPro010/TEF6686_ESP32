#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "ESP32Time.h"
#include "TimeLib.h"
#include "TEF6686.h"

static const char ntpServerName[] = "0.pool.ntp.org";
static const int localPort = 8944;
const int NTP_PACKET_SIZE = 48;

extern ESP32Time rtc;
extern WiFiClient RemoteClient;
extern WiFiUDP Udp;
extern TEF6686 radio;

extern bool wifi;
extern bool NTPupdated;
extern bool rtcset;

void sendNTPpacket(IPAddress &address);
void NTPupdate();
time_t getNtpTime();
