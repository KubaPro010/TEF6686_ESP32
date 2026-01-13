#include "rtc.hpp"

bool rtcset;
bool NTPupdated;

ESP32Time rtc(0);
bool rx_rtc_avail = false;
RX8010SJ::Adapter rx_rtc = RX8010SJ::Adapter(RX8010SJ_ADDRESS);

void timeToDateTime(time_t t, struct RX8010SJ::DateTime* dateTime) {
    struct tm* timeinfo = gmtime(&t);
    dateTime->second = timeinfo->tm_sec;
    dateTime->minute = timeinfo->tm_min;
    dateTime->hour = timeinfo->tm_hour;
    dateTime->dayOfWeek = (timeinfo->tm_wday + 6) % 7;
    dateTime->dayOfMonth = timeinfo->tm_mday;
    dateTime->month = timeinfo->tm_mon + 1;
    dateTime->year = (timeinfo->tm_year + 1900) % 100;
}

time_t dateTimeToTime(const struct RX8010SJ::DateTime* dateTime) {
    struct tm timeinfo;
    timeinfo.tm_sec = dateTime->second;
    timeinfo.tm_min = dateTime->minute;
    timeinfo.tm_hour = dateTime->hour;
    timeinfo.tm_mday = dateTime->dayOfMonth;
    timeinfo.tm_mon = dateTime->month - 1;
    timeinfo.tm_year = dateTime->year + 100;
    timeinfo.tm_isdst = 0;
    return mktime(&timeinfo);
}

void sync_from_rx_rtc(int32_t offset) {
    if(!rx_rtc_avail) return;
    RX8010SJ::DateTime dateTime = rx_rtc.readDateTime();
    rtc.setTime(dateTimeToTime(&dateTime) + offset);
}

void set_time(time_t time) {
    rtc.setTime(time);
    if(!rx_rtc_avail) return;
    RX8010SJ::DateTime rx_currenttime = RX8010SJ::DateTime();
    timeToDateTime(time, &rx_currenttime);
    rx_rtc.writeDateTime(rx_currenttime);
}