#pragma once

#include "system_console.h"
#include "globals.h"

extern Console console;
extern RTC_DATA_ATTR bool gpio_chip;
extern RTC_DATA_ATTR bool tef_found;

template<typename... Args>
void panic(Args... args) {
  radio.power(true);
  tft.fillScreen(TFT_RED);
  console.reset();

  (console.print(args), ...);
  while (true);
}

void Round30K(unsigned int freq);
void Round50K(unsigned int freq);
void Round100K(unsigned int freq);
void Round200K(unsigned int freq);
void WakeToSleep(bool yes);
void BANDBUTTONPress();
void BWButtonPress();
void doBandToggle();
int GetNum();
void NumpadProcess(int num);

bool isSignalQualityGood(int usn, int wam, int ostatus, int threshold_multiplier = SCAN_SIGNAL_THRESHOLD_USN_MULTIPLIER, int ostatus_threshold = SCAN_SIGNAL_THRESHOLD_OSTATUS);
void deepSleep();
inline bool IsStationEmpty() {
  return presets[memorypos].band == BAND_FM && presets[memorypos].frequency == EE_PRESETS_FREQUENCY;
}
