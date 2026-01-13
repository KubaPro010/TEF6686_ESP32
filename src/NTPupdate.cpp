#include "NTPupdate.h"

void sendNTPpacket(IPAddress &address) {
  byte packetBuffer[NTP_PACKET_SIZE] = {0}; // Initialize buffer with zeros

  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[2] = 6;          // Polling interval
  packetBuffer[3] = 0xEC;       // Peer clock precision

  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime() {
  IPAddress ntpServerIP;
  byte packetBuffer[NTP_PACKET_SIZE];

  while (Udp.parsePacket() > 0);

  if (!WiFi.hostByName(ntpServerName, ntpServerIP)) return 0;

  sendNTPpacket(ntpServerIP);

  uint32_t startWait = millis();
  while (millis() - startWait < 1500) {
    if (Udp.parsePacket() >= NTP_PACKET_SIZE) {
      Udp.read(packetBuffer, NTP_PACKET_SIZE);

      unsigned long secsSince1900 =
        ((unsigned long)packetBuffer[40] << 24) |
        ((unsigned long)packetBuffer[41] << 16) |
        ((unsigned long)packetBuffer[42] << 8)  |
        (unsigned long)packetBuffer[43];

      return secsSince1900 - 2208988800UL;
    }
  }

  return 0;
}

void NTPupdate() {
  if (!wifi) {
    NTPupdated = false;
    return;
  }

  time_t currentTime = getNtpTime();

  if (currentTime) {
    set_time(currentTime);
    rtcset = true;
    NTPupdated = true;
    radio.rds.ctupdate = false;
  } else {
    NTPupdated = false;
    radio.rds.ctupdate = true;
  }
}