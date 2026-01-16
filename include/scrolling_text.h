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
  std::function<void(TFT_eSprite*, bool)> postDrawCallback;
  int usedH;

  static const unsigned long SCROLL_INTERVAL = 5;
  static const unsigned long HOLD_DURATION = 2000;
  static const int SCROLL_GAP = 10;

public:
  ScrollingTextDisplay(TFT_eSprite* spr, int y, int maxW, int inuseH = -1 ) :
    sprite(spr), yPos(y), maxWidth(maxW), xPos(0), textWidth(0), lastTick(0), holdTick(0), isScrolling(false), postDrawCallback(nullptr), usedH(inuseH) {}

  void setPostDrawCallback(std::function<void(TFT_eSprite*, bool)> callback) {
    postDrawCallback = callback;
  }

  void update(const String& text, bool status, uint16_t activeColor, uint16_t activeSmooth, uint16_t dropoutColor, uint16_t dropoutSmooth, uint16_t backgroundColor) {
    textWidth = sprite->textWidth(text);

    if(textWidth < maxWidth) {
      xPos = 0;
      isScrolling = false;
      drawText(text, status, activeColor, activeSmooth, dropoutColor, dropoutSmooth, backgroundColor);
    } else {
      if(!isScrolling) holdTick = millis();
      isScrolling = true;
      if(millis() - lastTick >= SCROLL_INTERVAL) {
        if(xPos <= -(textWidth + SCROLL_GAP)) xPos = 0;

        if(xPos == 0) {
          if(millis() - holdTick >= HOLD_DURATION) {
            xPos--;
            holdTick = millis();
          }
        } else {
          xPos--;
          holdTick = millis();
        }

        drawText(text, status, activeColor, activeSmooth, dropoutColor, dropoutSmooth, backgroundColor);
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
    if(usedH > 0) {
      sprite->fillSprite(TFT_TRANSPARENT);
      sprite->fillRect(0, 0, maxWidth, usedH, backgroundColor);

      sprite->setViewport(0, 0, maxWidth, usedH);

      if(status) sprite->setTextColor(activeColor, activeSmooth, false);
      else sprite->setTextColor(dropoutColor, dropoutSmooth, false);

      sprite->drawString(text, xPos, 0);
      if(isScrolling) sprite->drawString(text, xPos + textWidth + SCROLL_GAP, 0);

      sprite->resetViewport();
    } else {
      sprite->fillSprite(backgroundColor);

      if(status) sprite->setTextColor(activeColor, activeSmooth, false);
      else sprite->setTextColor(dropoutColor, dropoutSmooth, false);

      sprite->drawString(text, xPos, 0);
      if(isScrolling) sprite->drawString(text, xPos + textWidth + SCROLL_GAP, 0);
    }

    if(postDrawCallback) postDrawCallback(sprite, false);
    sprite->pushSprite(35, yPos, TFT_TRANSPARENT);
  }
};