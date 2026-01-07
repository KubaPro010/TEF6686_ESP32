#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class ScrollingTextDisplay {
private:
  TFT_eSprite* sprite;
  int yPos;
  int maxWidth;
  int xPos;
  int textWidth;
  unsigned long lastTick;
  unsigned long holdTick;
  bool isScrolling;
  
  static const unsigned long SCROLL_INTERVAL = 5;
  static const unsigned long HOLD_DURATION = 2000;

public:
  ScrollingTextDisplay(TFT_eSprite* spr, int y, int maxW)
    : sprite(spr), yPos(y), maxWidth(maxW),
      xPos(0), textWidth(0), lastTick(0), holdTick(0), isScrolling(false) {}

  // Update the display with new text
  void update(const String& text, int width, bool status, uint16_t activeColor, uint16_t activeSmooth, uint16_t dropoutColor, uint16_t dropoutSmooth, uint16_t backgroundColor) {
    textWidth = width;
    
    if (textWidth < maxWidth) {
      xPos = 0;
      isScrolling = false;
      drawText(text, status, activeColor, activeSmooth, dropoutColor, dropoutSmooth, backgroundColor);
    } else {
      isScrolling = true;
      if (millis() - lastTick >= SCROLL_INTERVAL) {
        if (xPos < -textWidth) xPos = 0;
        
        if (xPos == 0) {
          if (millis() - holdTick >= HOLD_DURATION) {
            xPos--;
            holdTick = millis();
          }
        } else {
          xPos--;
          holdTick = millis();
        }
        
        drawScrollingText(text, status, activeColor, activeSmooth, dropoutColor, dropoutSmooth, backgroundColor);
        lastTick = millis();
      }
    }
  }

  void reset() {
    xPos = 0;
    holdTick = millis();
    lastTick = millis();
  }

  void setYPosition(int y) {
    yPos = y;
  }

  bool getIsScrolling() const {
    return isScrolling;
  }

private:
  void drawText(const String& text, bool status, uint16_t activeColor, uint16_t activeSmooth, uint16_t dropoutColor, uint16_t dropoutSmooth, uint16_t backgroundColor) {
    sprite->fillSprite(backgroundColor);
    if (status) sprite->setTextColor(activeColor, activeSmooth, false);
    else sprite->setTextColor(dropoutColor, dropoutSmooth, false);
    sprite->drawString(text, xPos, 2);
    sprite->pushSprite(35, yPos);
  }

  void drawScrollingText(const String& text, bool status, uint16_t activeColor, uint16_t activeSmooth, uint16_t dropoutColor, uint16_t dropoutSmooth, uint16_t backgroundColor) {
    sprite->fillSprite(backgroundColor);
    if (status) sprite->setTextColor(activeColor, activeSmooth, false);
    else sprite->setTextColor(dropoutColor, dropoutSmooth, false);
    sprite->drawString(text, xPos, 2);
    sprite->drawString(text, xPos + textWidth, 2);
    sprite->pushSprite(35, yPos);
  }
};