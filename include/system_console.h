#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

class Console {
public:
    explicit Console(TFT_eSPI* display) : tft(display), y(0) {}
    void print(String text) {
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
        tft->setTextDatum(TL_DATUM);
        auto data = "[" + String(millis() / 1000.0f) + "] " + text;
        tft->fillRect(0, y, tft->textWidth(data), tft->fontHeight(0), TFT_BLACK);
        tft->drawString(data, 0, y, 0);
        y += tft->fontHeight(0);
    }
    private:
    TFT_eSPI* tft;
    int y = 0;
};
