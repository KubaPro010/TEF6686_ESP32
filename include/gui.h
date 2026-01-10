#pragma once

#include "globals.h"
#include "utils.h"
#include "menugraphics.h"

static const char* const unitString[] = {"dBμV", "dBf", "dBm"};
static const char* const FreqFont[] = {"Classic", "Roubenstil", "Motoya", "Aura2", "Comic", "Modern"};
static const char* const Theme[] = {"Essence", "Cyan", "Crimson", "Monochrome", "Volcano", "Dendro", "Sakura", "Whiteout", "Tangerine", "Ocean", "Indigo", "Queer", "GoldBrite", "Bubblegum"};
static const char* BWButtonLabelsFM[] = {"56 kHz", "64 kHz", "72 kHz", "84 kHz", "97 kHz", "114 kHz", "133 kHz", "151 kHz", "168 kHz", "184 kHz", "200 kHz", "217 kHz", "236 kHz", "254 kHz", "287 kHz", "311 kHz", "Auto", "iMS", "EQ"};
static const char* BWButtonLabelsAM[] = {"3 kHz", "4 kHz", "6 kHz", "8 kHz"};

void BuildAFScreen();
void BuildRDSStatScreen();
void BuildMenu();
void BuildAdvancedRDS();
void BuildDisplay();
void BuildBWSelector();
void MenuUpDown(bool dir);
void DoMenu();
void doTheme();
void Infoboxprint(const char* input);
void drawButton(const char* text, byte button_number, bool active, bool selected);
String shortLine(String text);
void showMenuOpenTouchButtons();
void showBWSelector();

extern void ShowFreq(int mode);
extern void ShowBandSelectionFM(bool notglanceview, bool normaldisplay);
extern void ShowBandSelectionAM(bool notglanceview, bool normaldisplay);
extern void ScreensaverTimerSet(byte value);
extern void ShowMemoryPos();
extern void ShowTuneMode();
extern void updateBW();
extern void ShowStepSize();
extern void updateiMS();
extern void updateEQ();
extern void doTheme();
extern void tryWiFi();
extern void TuneUp();
extern void endMenu();
extern void startFMDXScan();
extern void DoMemoryPosTune();
extern void UpdateFonts(byte mode);
extern void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize);
extern void setAutoSpeedSPI();
extern void showAutoSquelch(bool mode);
extern uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi);
extern void ClearMemoryRange(uint8_t start, uint8_t stop);
extern bool handleCreateNewLogbook();
extern const char* textUI(uint16_t number);