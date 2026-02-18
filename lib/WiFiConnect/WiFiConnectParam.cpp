#include "WiFiConnectParam.h"

WiFiConnectParam::WiFiConnectParam(const char *custom) {
  _id = NULL;
  _placeholder = NULL;
  _length = 0;
  _value = NULL;

  _customHTML = custom;
}

WiFiConnectParam::WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length) {
  init(id, placeholder, defaultValue, length, "");
}
WiFiConnectParam::WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom) {
  init(id, placeholder, defaultValue, length, custom);
}

void WiFiConnectParam::init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom) {
  _id = id;
  _placeholder = placeholder;
  _length = length;
  setValue(defaultValue);

  _customHTML = custom;
}

void WiFiConnectParam::setValue(const char *newValue){
  if(_length > 0) {
    // you know what was here in upstream
    _value = new char[_length + 1];
    for (int i = 0; i < _length; i++) _value[i] = 0;
    if (newValue != NULL) strncpy(_value, newValue, _length);
  }
}

const char* WiFiConnectParam::getValue() {
  return _value;
}

const char* WiFiConnectParam::getID() {
  return _id;
}

const char* WiFiConnectParam::getPlaceholder() {
  return _placeholder;
}

int WiFiConnectParam::getValueLength() {
  return _length;
}

const char* WiFiConnectParam::getCustomHTML() {
  return _customHTML;
}
