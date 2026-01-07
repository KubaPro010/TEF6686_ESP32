#pragma once

#include "globals.h"
#include <Hash.h>

void Communication();
void XDRGTKRoutine();
void passwordcrypt();
void tryWiFi();

extern void DataPrint(String string);
extern void ShowFreq(int mode);
extern void SelectBand();
extern void doBW();
extern void BuildDisplay();
extern void BuildAdvancedRDS();
extern void ModeButtonPress();
extern void Seek(bool mode);
extern void doStereoToggle();
extern void MuteScreen(bool setting);
extern void updateiMS();
extern void updateEQ();
extern void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize);
extern void showAutoSquelch(bool mode);
extern void ShowStepSize();
extern void startFMDXScan();
extern void cancelDXScan();
extern void printLogbookCSV();
extern void NTPupdate();
extern void handleRoot();
extern void handleDownloadCSV();
extern void handleLogo();
extern void Infoboxprint(const char* input);
extern void TuneUp();
extern void TuneDown();
extern void ShowTuneMode();
extern const char* textUI(uint16_t number);
extern void saveData();