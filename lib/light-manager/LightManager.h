#pragma once
#include "Arduino.h"
#include "FastLED.h"

struct PixelRange
{
  int start;
  int end;
};

class LightManager
{
public:
  LightManager(CRGB *leds, int totalLEDs)
  {
    _leds = leds;
    _totalLeds = totalLEDs;
  }

  void clear()
  {
    for (int i = 0; i < _totalLeds; i++)
    {
      _leds[i] = CRGB::Black;
    }
    FastLED.show();
  }

  void setSectionRange(PixelRange range);

  void setSectionColor(int rangeIndex, uint8_t red, uint8_t green, uint8_t blue);
  void setSectionColor(int rangeIndex, CRGB color);

  void gradientAcrossBars(int startIndex, CRGB startColor, int endIndex, CRGB endColor);
  void lightRandomSections(int numberOfSections);

  void setPixel(int index, CRGB color);

  // ! Utility method
  void printSections()
  {
    for (int i = 0; i < _totalSections; i++)
    {
      char buffer[31];
      sprintf(buffer, "Section %i - start: %i, end: %i", i, _sections[i].start, _sections[i].end);
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
