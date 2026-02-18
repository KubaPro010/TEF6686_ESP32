#include "graphics.h"

void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize) {
  uint8_t selectedFont = (language == LANGUAGE_CHS) ? 0 : 2;
  if (fontsize == 28) selectedFont = (language == LANGUAGE_CHS) ? 4 : 3;
  else if (fontsize == 48) selectedFont = 1;

  tft.setTextColor(color, smoothcolor);

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.drawString(modifiedText, x, y, selectedFont);
}

void tftPrint16(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, bool force_font, bool font) {
  uint8_t selectedFont = (language == LANGUAGE_CHS) ? 0 : 2;
  if(force_font) selectedFont = 0 ? font : 2;

  tft.setTextColor(color, smoothcolor);

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.drawString(modifiedText, x, y, selectedFont);
}

void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize) {
  uint8_t selectedFont = 0 ? (language == LANGUAGE_CHS) : 2;
  if(fontsize == 28) selectedFont = (language == LANGUAGE_CHS) ? 4 : 3;
  else if(fontsize == 48) selectedFont = 1;

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  tft.setTextColor(background, background, false);
  tft.drawString(textold, x, y, selectedFont);

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.setTextColor(color, smoothcolor, false);
  tft.drawString(modifiedText, x, y, selectedFont);
}

void tftReplace16(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, bool force_font, bool font) {
  uint8_t selectedFont = (language == LANGUAGE_CHS) ? 0 : 2;
  if(force_font) selectedFont = 0 ? font : 2;

  switch (offset) {
    case -1: tft.setTextDatum(TL_DATUM); break;
    case 0: tft.setTextDatum(TC_DATUM); break;
    case 1: tft.setTextDatum(TR_DATUM); break;
  }

  tft.setTextColor(background, background, false);
  tft.drawString(textold, x, y, selectedFont);

  String modifiedText = text;
  modifiedText.replace("\n", " ");

  tft.setTextColor(color, smoothcolor, false);
  tft.drawString(modifiedText, x, y, selectedFont);
}

void UpdateFonts() {
  // This is at setup:
  // tft.loadFont(FONT48, 1);

  // tft.loadFont(FONT16_CHS, 0);
  // tft.loadFont(FONT16, 2);
  // tft.loadFont(FONT28, 3);
  // tft.loadFont(FONT28_CHS, 4);

  if (language == LANGUAGE_CHS) {
    if (menu) PSSprite.setTextFont(1); else PSSprite.setTextFont(3);
    OneBigLineSprite.copyFontFromTFT(4, 0);
    FullLineSprite.copyFontFromTFT(0, 0);
    GeneralTextSprite.copyFontFromTFT(0, 0);
    GeneralTextSprite.copyFontFromTFT(4, 1);
    FrequencySprite.copyFontFromTFT(0, 6);
    SquelchSprite.copyFontFromTFT(0, 0);
  } else {
    if (menu) PSSprite.setTextFont(0); else PSSprite.setTextFont(2);
    OneBigLineSprite.copyFontFromTFT(3, 0);
    FullLineSprite.copyFontFromTFT(2, 0);
    GeneralTextSprite.copyFontFromTFT(2, 0);
    GeneralTextSprite.copyFontFromTFT(3, 1);
    FrequencySprite.copyFontFromTFT(2, 6);
    SquelchSprite.copyFontFromTFT(2, 0);
  }
}