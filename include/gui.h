#pragma once

#include "globals.h"
#include "utils.h"
#include "menugraphics.h"
#include "core.h"

static const char* const unitString[] = {"dBμV", "dBf", "dBm"};
static const char* const FreqFont[] = {"Classic", "Roubenstil", "Motoya", "Aura2", "Comic", "Modern"};
static const char* const Theme[] = {"Essence", "Cyan", "Crimson", "Monochrome", "Volcano", "Dendro", "Sakura", "Whiteout", "Tangerine", "Ocean", "Indigo", "Queer", "GoldBrite", "Bubblegum"};
extern const char* BWButtonLabelsFM[];
extern const char* BWButtonLabelsAM[];

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

extern void ShowBandSelectionFM(bool notglanceview, bool normaldisplay);
extern void ShowBandSelectionAM(bool notglanceview, bool normaldisplay);
extern void ScreensaverTimerSet(byte value);
extern void doTheme();
extern void tryWiFi();
extern void ClearMemoryRange(uint8_t start, uint8_t stop);
extern bool handleCreateNewLogbook();