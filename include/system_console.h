#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

#define CONSOLE_FONT 2

class Console {
public:
    explicit Console(TFT_eSPI* display) : tft(display), y(0) {}
    void print(String text, uint16_t background = TFT_BLACK) {
        tft->setTextColor(TFT_WHITE, background);
        tft->setTextDatum(TL_DATUM);
        auto data = "[" + String(millis() / 1000.0f) + "] " + text;
        tft->fillRect(0, y, tft->textWidth(data, CONSOLE_FONT), tft->fontHeight(CONSOLE_FONT), background);
        tft->drawString(data, 0, y, CONSOLE_FONT);
        y += tft->fontHeight(CONSOLE_FONT);
    }
    void reset() {
        y = 0;
    }
    private:
    TFT_eSPI* tft;
    int y = 0;
};
