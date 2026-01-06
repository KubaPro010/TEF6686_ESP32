#include "utils.h"

HSV RGB565toHSV(uint16_t color) {
  uint8_t r = ((color >> 11) & 0x1F) * 8;  // Convert 5-bit to 8-bit
  uint8_t g = ((color >> 5) & 0x3F) * 4;   // Convert 6-bit to 8-bit
  uint8_t b = (color & 0x1F) * 8;          // Convert 5-bit to 8-bit

  float rf = r / 255.0, gf = g / 255.0, bf = b / 255.0;
  float maxVal = max(rf, max(gf, bf));
  float minVal = min(rf, min(gf, bf));
  float delta = maxVal - minVal;

  HSV hsv;
  hsv.v = maxVal;

  if (delta < 0.0001) {
    hsv.h = 0;
    hsv.s = 0;
  } else {
    hsv.s = delta / maxVal;

    if (maxVal == rf) hsv.h = fmod(((gf - bf) / delta), 6.0);
    else if (maxVal == gf) hsv.h = ((bf - rf) / delta) + 2.0;
    else hsv.h = ((rf - gf) / delta) + 4.0;

    hsv.h *= 60.0;
    if (hsv.h < 0) hsv.h += 360.0;
  }
  return hsv;
}

uint16_t HSVtoRGB565(float h, float s, float v) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - c;

  float rf, gf, bf;
  if (h < 60) rf = c, gf = x, bf = 0;
  else if (h < 120) rf = x, gf = c, bf = 0;
  else if (h < 180) rf = 0, gf = c, bf = x;
  else if (h < 240) rf = 0, gf = x, bf = c;
  else if (h < 300) rf = x, gf = 0, bf = c;
  else rf = c, gf = 0, bf = x;

  uint8_t r = (rf + m) * 255;
  uint8_t g = (gf + m) * 255;
  uint8_t b = (bf + m) * 255;

  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
