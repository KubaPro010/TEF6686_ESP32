#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
using fs::FS;
#include <WebServer.h>
#include <DNSServer.h>
#include <memory>

#include <esp_wifi.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())///< Gets an ID from the chip

#include "WC_AP_HTML.h"
#include "WiFiConnectParam.h"

class WiFiConnect {
  public:
    WiFiConnect();

    boolean startConfigurationPortal();
    boolean startConfigurationPortal(const char* apName, const char*  apPassword = NULL, bool paramsMode = false);

    void addParameter(WiFiConnectParam *p);

    void setAPName(const char* apName);
    const char* getAPName();

    void resetSettings();

    boolean autoConnect();
    boolean autoConnect(const char*  ssidName, const char*  ssidPassword = NULL, WiFiMode_t acWiFiMode = WIFI_STA);

    void setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    void setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);

    void setRetryAttempts(int attempts);
    void setConnectionTimeoutSecs(int timeout);
    void setAPModeTimeoutMins(int mins);

    boolean captivePortal();

    //helpers
    const char* statusToString(int state);
    int getRSSIasQuality(int RSSI);
    boolean isIp(String str);
    String toStringIp(IPAddress ip);
  private:
    int _retryAttempts = 3; ///< Number of attempts when trying to connect to WiFi network
    int _connectionTimeoutSecs = 10; ///< How log to wait for the connection to succeed or fail
    int _apTimeoutMins = 3; ///< The amount of minutes of inactivity before the access point exits it routine
  
    long _lastAPPage = 0;
    int _minimumQuality = 8;
    int _paramsCount = 0;
    boolean _readyToConnect = false;
    String _ssid      = "                            ";
    String _password  = "                            ";

    WiFiConnectParam* _params[WiFiConnect_MAX_PARAMS]; ///< Array to hold custom parameters

    std::unique_ptr<DNSServer> dnsServer;
    std::unique_ptr<WebServer> server;

     char _apName[33];
     char _apPassword[65];
    
    IPAddress _ap_static_ip; ///< Variable for holding Static IP Address for the access point
    IPAddress _ap_static_gw; ///< Variable for holding Static Gateway IP Address for the access point
    IPAddress _ap_static_sn; ///< Variable for holding Static Subnet Mask IP Address for the access point
    IPAddress _sta_static_ip; ///< Variable for holding Static IP Address for the network connection
    IPAddress _sta_static_gw; ///< Variable for holding Static Gateway IP Address for the network connection
    IPAddress _sta_static_sn; ///< Variable for holding Static Subnet Mask IP Address for the network connection

    void handleRoot();
    void handleParamRoot();
    void handleParams();
    void handleWifi(boolean scan);
    void handleWifiSave();
    void handleInfo();
    void handleReset();
    void handle204();
    void handleNotFound();

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(obj->fromString(s)) { return obj->fromString(s); }
    bool optionalIPFromString(...) {return false;}
};
