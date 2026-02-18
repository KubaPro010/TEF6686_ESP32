#pragma once

#include "globals.h"

void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize);
void tftPrint16(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, bool force_font = true, bool font = false);
void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize);
void tftReplace16(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, bool force_font = true, bool font = false);
void UpdateFonts();
