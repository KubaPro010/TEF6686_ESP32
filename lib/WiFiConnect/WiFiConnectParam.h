#pragma once

#ifndef WiFiConnect_MAX_PARAMS
#define WiFiConnect_MAX_PARAMS 10 ///< The maximum size of the param array and how many custom parameters we may have
#endif

#include <Arduino.h>

class WiFiConnectParam {
  public:
    WiFiConnectParam(const char *custom);
    WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int getValueLength();
    const char *getCustomHTML();
    void setValue(const char *newValue);
  private:
    const char *_id;
    const char *_placeholder;
    char *_value;
    int _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiConnect; ///< Declarion for WiFiConnect class
};
