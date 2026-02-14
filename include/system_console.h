#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

class Console {
public:
    explicit Console(TFT_eSPI* display) : tft(display), y(0) {}
    void print(String text, uint16_t background = TFT_BLACK) {
        tft->setTextColor(TFT_WHITE, background);
        tft->setTextDatum(TL_DATUM);
        auto data = "[" + String(millis() / 1000.0f) + "] " + text;
        tft->fillRect(0, y, tft->textWidth(data), tft->fontHeight(2), background);
        tft->drawString(data, 0, y, 2);
        y += tft->fontHeight(2);
    }
    void reset() {
        y = 0;
    }
    private:
    TFT_eSPI* tft;
    int y = 0;
};
