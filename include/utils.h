#pragma once
#include <Arduino.h>
#include <math.h>
#include <stdint.h>

struct HSV {
  float h;
  float s;
  float v;
};

HSV RGB565toHSV(uint16_t color);
uint16_t HSVtoRGB565(float h, float s, float v);
String convertToUTF8(const wchar_t* input);
String trimTrailingSpaces(String str);
String ucs2ToUtf8(const char* ucs2Input);
String extractUTF8Substring(const String & utf8String, size_t start, size_t length, bool underscore);
String removeNewline(String inputString);