#pragma once
#include <EEPROM.h>
#include "globals.h"
#include "logbook.h"

void StoreFrequency();
void ClearMemoryRange(uint8_t start, uint8_t stop);
void saveData();
void loadData();
void DefaultSettings();