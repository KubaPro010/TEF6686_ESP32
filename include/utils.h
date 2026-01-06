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