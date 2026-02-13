#pragma once
#include <stdint.h>
#include <Arduino.h>

void tftPrint(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, uint8_t fontsize);
void tftPrint16(int8_t offset, const String & text, int16_t x, int16_t y, int color, int smoothcolor, bool force_font = true, bool font = false);
void tftReplace(int8_t offset, const String & textold, const String & text, int16_t x, int16_t y, int color, int smoothcolor, int background, uint8_t fontsize);
const char* textUI(uint16_t number);
uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi);
void showAutoSquelch(bool mode);
void UpdateFonts();
void setAutoSpeedSPI();
void DoMemoryPosTune();
void startFMDXScan();
void endMenu();
void TuneUp();
void ShowStepSize();
void ShowFreq(int mode);
void ShowMemoryPos();
void TuneDown();
void ShowTuneMode();
void SelectBand();
void doBW();
void ModeButtonPress();
void updateEQ();
void updateiMS();
void Seek(bool mode);
void MuteScreen(bool setting);
void cancelDXScan();
void doStereoToggle();
void updateBW();
void ShowRDSLogo(bool RDSstatus);
void DataPrint(String string);
