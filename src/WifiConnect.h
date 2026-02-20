#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <esp_wifi.h>
#include <memory>
#include "core.h"

#define WiFiConnect_MAX_PARAMS 10

extern const char* textUI(uint16_t number);

class WiFiConnect;
class WiFiConnectParam {
public:
  WiFiConnectParam(const char *custom);

  WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length);

  const char *getID();
  const char *getValue();
  const char *getPlaceholder();
  int         getValueLength();
  const char *getCustomHTML();

private:
  const char *_id;
  const char *_placeholder;
  char *_value;
  int _length;
  const char *_customHTML;

  void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
  void setValue(const char *newValue);

  friend class WiFiConnect;
};

class WiFiConnect {
public:
  WiFiConnect();

  boolean autoConnect();
  void addParameter(WiFiConnectParam *p);
  boolean startConfigurationPortal(int8_t cancelPin);

private:
  static constexpr int RETRY_ATTEMPTS = 3;
  static constexpr int CONNECTION_TIMEOUT_SECS = 10;
  static constexpr int MINIMUM_QUALITY = 8;

  int _paramsCount = 0;
  boolean _readyToConnect = false;
  String _ssid;
  String _password;

  WiFiConnectParam* _params[WiFiConnect_MAX_PARAMS];

  std::unique_ptr<DNSServer> dnsServer;
  std::unique_ptr<WebServer> server;

  char _apName[33];

  void setAPName();

  boolean autoConnect(const char *ssidName, const char *ssidPassword, WiFiMode_t acWiFiMode);

  void handleRoot();
  void handleWifi();
  void handleScan();
  void handleWifiSave();
  void handleLogo();
  void handleNotFound();

  boolean captivePortal();
  boolean isIp(String str);
  String  toStringIp(IPAddress ip);
  int getRSSIasQuality(int RSSI);
};