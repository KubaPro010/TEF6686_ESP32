#include "rtc.hpp"

// the hardware rtc driver was made with the support of Wh1teRabbitHU's implementation

bool rtcset;
bool NTPupdated;

ESP32Time rtc(0);
bool rx_rtc_avail = false;

byte readFromModule(byte address) {
    Wire.beginTransmission(RX8010SJ_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(RX8010SJ_ADDRESS, 1);
    if (Wire.available()) return Wire.read();
    return -1;
}

inline void writeToModule(byte address, byte data) {
    Wire.beginTransmission(RX8010SJ_ADDRESS);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

inline byte toBCD(byte val) {
    return ((val / 10) << 4) | (val % 10);
}

inline byte sumValueFromBinary(byte binary, byte length) {
    byte sum = 0;

    for (byte i = 0; i < length; i++) {
        byte value;

        if (i < 4 ) value = 1 << i;
        else value = 10 * (1 << (i - 4));

        sum += ((binary >> i) & 1) == 1 ? value : 0;
    }

    return sum;
}

bool init_rtc() {
    rtc.setTime(0);
    byte flagregister = readFromModule(0x1E);
    if((flagregister >> 1) & 1) {
        while((flagregister >> 1) & 1) {
            writeToModule(0x1E, 0); // clear VLF
            flagregister = readFromModule(0x1E);
        }
        writeToModule(0x17, 216); // Reserved register
        writeToModule(0x1F, 0); // Control register
        writeToModule(0x31, 8); // Reserved register
        writeToModule(0x32, 0); // IRQ control register

        Wire.beginTransmission(RX8010SJ_ADDRESS);
        Wire.write(0x10);
        Wire.write(0);
        Wire.write(0);
        Wire.write(toBCD(12));
        Wire.write(1 << 2);
        Wire.write(toBCD(14));
        Wire.write(1);
        Wire.write(toBCD(26));
        Wire.endTransmission();
        return true;
    }
    return false;
}

void sync_from_rx_rtc(int32_t offset) {
    if(!rx_rtc_avail) return;
    struct tm timeinfo;

    Wire.beginTransmission(RX8010SJ_ADDRESS);
    Wire.write(0x10);
    Wire.endTransmission();
    Wire.requestFrom(RX8010SJ_ADDRESS, 7);
    if (Wire.available()) {
        timeinfo.tm_sec = sumValueFromBinary(Wire.read(), 7);
        timeinfo.tm_min = sumValueFromBinary(Wire.read(), 7);
        timeinfo.tm_hour = sumValueFromBinary(Wire.read(), 6);

        auto dayOfWeekBin = Wire.read();
        for (int i = 0; i < 7; i++) {
            if (dayOfWeekBin & (1u << i)) timeinfo.tm_wday += i;
        }

        timeinfo.tm_mday = sumValueFromBinary(Wire.read(), 6);
        timeinfo.tm_mon = sumValueFromBinary(Wire.read(), 5) - 1;
        timeinfo.tm_year = sumValueFromBinary(Wire.read(), 8) + 100;

        rtc.setTime(mktime(&timeinfo) + offset);
    }
}

void set_time(time_t time) {
    rtc.setTime(time);
    if(!rx_rtc_avail) return;
    struct tm* timeinfo = gmtime(&time);
    Wire.beginTransmission(RX8010SJ_ADDRESS);
    Wire.write(0x10);
    Wire.write(toBCD(timeinfo->tm_sec));
    Wire.write(toBCD(timeinfo->tm_min));
    Wire.write(toBCD(timeinfo->tm_hour));
    Wire.write(1 << timeinfo->tm_wday);
    Wire.write(toBCD(timeinfo->tm_mday));
    Wire.write(toBCD(timeinfo->tm_mon + 1));
    Wire.write(toBCD((1900 + timeinfo->tm_year) % 100));
    Wire.endTransmission();
}