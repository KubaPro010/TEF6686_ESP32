#pragma once
#include <stdint.h>
#include <Arduino.h>

const char* textUI(uint16_t number);
uint8_t doAutoMemory(uint16_t startfreq, uint16_t stopfreq, uint8_t startmem, uint8_t stopmem, bool rdsonly, uint8_t doublepi);
void setAutoSpeedSPI();
void DoMemoryPosTune();
void startFMDXScan();
void endMenu();
void TuneUp();
void ShowFreq(int mode);
void ShowMemoryPos();
void TuneDown();
void ShowTuneMode();
void SelectBand();
void doBW();
void ModeButtonPress();
void Seek(bool mode);
void MuteScreen(bool setting);
void cancelDXScan();
void doStereoToggle();
void DataPrint(String string);
