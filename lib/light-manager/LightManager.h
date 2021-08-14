#pragma once
#include "../pixel-range/PixelRange.h"
#include "Arduino.h"
#include "FastLED.h"

class LightManager
{
public:
  LightManager(CRGB *leds, int totalLEDs)
  {
    _leds = leds;
    _totalLeds = totalLEDs;
  }

  void clear();

  void setSectionRange(PixelRange range);

  void setSectionColor(int sectionIndex, uint8_t red, uint8_t green, uint8_t blue);
  void setSectionColor(int sectionIndex, CRGB color);
  void setSectionLevelColor(int sectionIndex, uint8_t _rainbowMiddleOutColorLevelInSection, CRGB color);

  void gradientAcrossBars(int startIndex, CRGB startColor, int endIndex, CRGB endColor);
  void lightRandomSections(int numberOfSections);

  void setPixel(int index, CRGB color);

  // ! Utility method
  void printSections()
  {
    for (int i = 0; i < _totalSections; i++)
    {
      char buffer[31];
      sprintf(buffer, "Section %i - start: %i, end: %i", i, _sections[i].getStart(), _sections[i].getEnd());
      Serial.println(buffer);
    }
  }

private:
  CRGB *_leds;
  int _totalLeds;

  int _currentSectionIndex = 0;
  uint8_t _totalSections = 9;
  PixelRange _sections[6 * sizeof(PixelRange)];

  void setColorValueForRange(PixelRange range, CRGB color);
};
