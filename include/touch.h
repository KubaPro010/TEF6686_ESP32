#pragma once

#include "globals.h"
#include "core.h"

void doTouchEvent(uint16_t x, uint16_t y);

extern void BuildDisplay();
extern void BuildBWSelector();
extern void BuildAdvancedRDS();
extern void doBandToggle();
extern void doTuneMode();
extern void drawButton(const char* text, byte button_number, bool active, bool selected);
extern void KeyDown();
extern void KeyUp();
extern void ButtonPress();
extern void DoMenu();
extern void toggleiMSEQ();
extern void showBWSelector();
extern void updateiMS();
extern void updateEQ();
extern void BuildAFScreen();
