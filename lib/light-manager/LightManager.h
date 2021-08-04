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

  void setSectionRange(PixelRange range);
  void setSectionRange(LiLightSection section, int start, int end);

  void setSectionColor(int rangeIndex, uint8_t red, uint8_t green, uint8_t blue);
  void setSectionColor(int rangeIndex, CRGB color);
  void setSectionColor(LiLightSection section, uint8_t red, uint8_t green, uint8_t blue);
  void setSectionColor(LiLightSection section, uint32_t color);
  void setSectionColor(LiLightSection section, CRGB color);

  // ! Utility method
  void printSections()
  {
    for (int i = 0; i < totalSections; i++)
    {
      char buffer[31];
      sprintf(buffer, "Section %i - start: %i, end: %i", i, sections[i].start, sections[i].end);
      Serial.println(buffer);
    }
  }

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

  int currentSectionIndex = 0;
  uint8_t totalSections = 9;
  PixelRange sections[6 * sizeof(PixelRange)];

  void setColorValueForRange(PixelRange range, CRGB color);
};
