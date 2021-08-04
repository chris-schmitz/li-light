#pragma once
#include "Arduino.h"
#include "FastLED.h"

enum LiLightSection
{
  BAR_CODE_1,
  BAR_CODE_2,
  BAR_CODE_3,
  BAR_CODE_4,
  BAR_CODE_5,
  BAR_CODE_6,
  GRAPH_LEFT,
  GRAPH_MIDDLE,
  GRAPH_RIGHT
};

struct PixelRange
{
  int start;
  int end;
};

class LightManager
{
public:
  LightManager(CRGB *leds)
  {
    _leds = leds;
  }

  void setSectionRange(LiLightSection section, int start, int end);
  void setSectionColor(LiLightSection section, uint8_t red, uint8_t green, uint8_t blue);
  void setSectionColor(LiLightSection section, uint32_t color);
  void setSectionColor(LiLightSection section, CRGB color);

  void gradientAcrossBars(uint32_t colorStart, uint32_t endColor);

  void setBars()
  {
    bars[0 * sizeof(PixelRange)] = &barCodeBar1;
    bars[1 * sizeof(PixelRange)] = &barCodeBar2;
    bars[2 * sizeof(PixelRange)] = &barCodeBar3;
  }

  PixelRange getBar1()
  {
    return barCodeBar1;
  }
  // ^ terrible way of organizing it, come back and adjust later
private:
  CRGB *_leds;
  PixelRange barCodeBar1;
  PixelRange barCodeBar2;
  PixelRange barCodeBar3;
  PixelRange barCodeBar4;
  PixelRange barCodeBar5;
  PixelRange barCodeBar6;
  PixelRange graphRight;
  PixelRange graphMiddle;
  PixelRange graphLeft;

  PixelRange *bars[6 * sizeof(PixelRange)];

  void setColorValueForRange(PixelRange range, CRGB color);
};
