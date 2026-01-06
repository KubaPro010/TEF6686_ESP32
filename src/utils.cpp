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

String convertToUTF8(const wchar_t* input) {
  String output;
  while (*input) {
    uint32_t unicode = *input;
    if (unicode < 0x80) output += (char)unicode;
    else if (unicode < 0x800) {
      output += (char)(0xC0 | (unicode >> 6));
      output += (char)(0x80 | (unicode & 0x3F));
    } else if (unicode < 0x10000) {
      output += (char)(0xE0 | (unicode >> 12));
      output += (char)(0x80 | ((unicode >> 6) & 0x3F));
      output += (char)(0x80 | (unicode & 0x3F));
    } else {
      output += (char)(0xF0 | (unicode >> 18));
      output += (char)(0x80 | ((unicode >> 12) & 0x3F));
      output += (char)(0x80 | ((unicode >> 6) & 0x3F));
      output += (char)(0x80 | (unicode & 0x3F));
    }
    input++;
  }
  return output;
}

String trimTrailingSpaces(String str) {
  int end = str.length() - 1;
  while (end >= 0 && isspace(str[end])) end--;
  return str.substring(0, end + 1);
}

String ucs2ToUtf8(const char* ucs2Input) {
  String utf8Output;

  size_t length = 0;
  while (ucs2Input[length] != '\0' || ucs2Input[length + 1] != '\0') length += 2;

  for (size_t i = 0; i < length; i += 2) {
    uint16_t ucs2Char = ((uint8_t)ucs2Input[i] << 8) | (uint8_t)ucs2Input[i + 1];

    if (ucs2Char <= 0x7F) utf8Output += (char)ucs2Char;
    else if (ucs2Char <= 0x7FF) {
      utf8Output += (char)(0xC0 | (ucs2Char >> 6));
      utf8Output += (char)(0x80 | (ucs2Char & 0x3F));
    } else {
      utf8Output += (char)(0xE0 | (ucs2Char >> 12));
      utf8Output += (char)(0x80 | ((ucs2Char >> 6) & 0x3F));
      utf8Output += (char)(0x80 | (ucs2Char & 0x3F));
    }
  }
  return utf8Output;
}

String extractUTF8Substring(const String & utf8String, size_t start, size_t length, bool underscore) {
  String substring;
  size_t utf8Length = utf8String.length();
  size_t utf8Index = 0;
  size_t charIndex = 0;

  while (utf8Index < utf8Length && charIndex < start + length) {
    uint8_t currentByte = utf8String.charAt(utf8Index);
    uint8_t numBytes = 0;

    if (currentByte < 0x80) numBytes = 1;
    else if ((currentByte >> 5) == 0x6) numBytes = 2;
    else if ((currentByte >> 4) == 0xE) numBytes = 3;
    else if ((currentByte >> 3) == 0x1E) numBytes = 4;

    if (charIndex >= start) substring += utf8String.substring(utf8Index, utf8Index + numBytes);

    utf8Index += numBytes;
    charIndex++;
  }

  if (underscore) {
    while (substring.length() < length) substring += '_';
  }

  return substring;
}