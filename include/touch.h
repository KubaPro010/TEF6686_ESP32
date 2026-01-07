#pragma once

#include "globals.h"

void doTouchEvent(uint16_t x, uint16_t y);

extern void BuildDisplay();
extern void BuildBWSelector();
extern void SelectBand();
extern void BuildAdvancedRDS();
extern void doBandToggle();
extern void doTuneMode();
extern void doStereoToggle();
extern void cancelDXScan();
extern void doBW();
extern void drawButton(const char* text, byte button_number, bool active, bool selected);
extern void KeyDown();
extern void KeyUp();
extern void ButtonPress();
extern void DoMenu();
extern void ModeButtonPress();
extern void toggleiMSEQ();
extern void showBWSelector();
extern void updateiMS();
extern void updateEQ();
extern void DataPrint(String string);
extern void BuildAFScreen();
extern void ShowFreq(int mode);
