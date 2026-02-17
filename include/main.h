#pragma once

#include "system_console.h"
#include "globals.h"

extern Console console;
extern RTC_DATA_ATTR bool gpio_chip;
extern RTC_DATA_ATTR bool tef_found;

template<typename... Args>
void panic(Args... args);

void Round30K(unsigned int freq);
void Round50K(unsigned int freq);
void Round100K(unsigned int freq);
void Round200K(unsigned int freq);
