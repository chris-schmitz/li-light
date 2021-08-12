#pragma once

#include "FastLED.h"
#include "LightManager.h"

enum IdlePatterns
{
  RANDOM_SECTIONS,
  SUNSET
};
const uint8_t TOTAL_NUMBER_OF_IDLE_PATTERNS = 2;

class PatternRunner
{
public:
  PatternRunner(LightManager *lightManager) : _lightManager(lightManager)
  {
  }

  void runCurrentIdlePattern()
  {
    switch (_currentIdlePattern)
    {
    case RANDOM_SECTIONS:
      randomSections();
      break;
    case SUNSET:
      idleSectionBySection();
      break;
    }
  }

  void cycleIdlePattern()
  {
    unsigned long now = millis();
    if (now - _idlePatternCycleLastChecked > _idlePatternCycleDebounceDuration)
    {
      _idlePatternCycleLastChecked = now;
      int newIndex = (_currentIdlePattern + 1) % TOTAL_NUMBER_OF_IDLE_PATTERNS;
      Serial.print("current pattern index: ");
      Serial.println(_currentIdlePattern);
      Serial.print("New idle pattern index: ");
      Serial.println(newIndex);
      Serial.print("total enums: ");
      Serial.println(TOTAL_NUMBER_OF_IDLE_PATTERNS);
      _currentIdlePattern = static_cast<IdlePatterns>((_currentIdlePattern + 1) % TOTAL_NUMBER_OF_IDLE_PATTERNS);
      FastLED.clear(true);
    }
  }

  void randomSections()
  {
    _lightManager->lightRandomSections(1);
    FastLED.show();
    delay(100);
  }

  // TODO: after confirming that these work, move out to cpp file
  void gradualBarcodeScan()
  {
    // * the state we want to push across the bars
    // * this is a colleciton of colors indexes to use for each bar
    // * each byte corresponds to a bar
    byte frameState[] = {9, 9, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9};

    int totalFrames = 23;
    for (int frame = 0; frame < totalFrames; frame++)
    {
      for (int sectionIndex = 0; sectionIndex < 6; sectionIndex++)
      {
        CRGB color = CRGB::Black;
        if (sectionIndex + frame + 1 < totalFrames)
        {
          color = CRGB(_barcodeScanColors[frameState[sectionIndex + frame]]);
        }
        _lightManager->setSectionColor(sectionIndex, color);
        FastLED.show();
      }
      delay(20);
    }
  }

  void idleSectionBySection()
  {
    unsigned long now = millis();
    if (now - lastIdleSeciontBySectionCheck > lastIdleSeciontBySectionDuration)
    {
      lastIdleSeciontBySectionCheck = now;
      if (_idlePatternSectionBySectionFill)
      {
        _lightManager->setSectionColor(_idlePatternSectionBySectionCurrentIndex, CRGB::Aquamarine);
      }
      else
      {
        _lightManager->setSectionColor(_idlePatternSectionBySectionCurrentIndex, CRGB::Black);
      }
      FastLED.show();

      _idlePatternSectionBySectionCurrentIndex = (_idlePatternSectionBySectionCurrentIndex + 1) % 9; // TODO: pull in total sections
      if (_idlePatternSectionBySectionCurrentIndex == 0)
      {
        _idlePatternSectionBySectionFill = !_idlePatternSectionBySectionFill;
      }
    }
  }

  void fillSectionBySection()
  {
    for (uint8_t i = 0; i < 9; i++)
    {
      _lightManager->setSectionColor(i, CRGB::BlueViolet);
      FastLED.show();
      delay(100);
    }
    for (uint8_t i = 0; i < 9; i++)
    {
      _lightManager->setSectionColor(i, CRGB::Black);
      FastLED.show();
      delay(100);
    }
  }

  void fadeInBars()
  {
    for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
    {
      for (uint8_t j = 6; j < 9; j++)
      {
        _lightManager->setSectionColor(j, CHSV(200, 200, _barGraphFadeInBrightnessLevels[i]));
      }
      FastLED.show();
      delay(50);
    }
    delay(800);
    for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
    {
      for (uint8_t j = 0; j < 6; j++)
      {
        _lightManager->setSectionColor(j, CHSV(0, 0, _barGraphFadeInBrightnessLevels[i]));
      }
      FastLED.show();
      delay(50);
    }
  }

private:
  LightManager *_lightManager;

  uint32_t _barcodeScanColors[10] = {0xf8f9fa, 0xe9ecef, 0xdee2e6, 0xced4da, 0xadb5bd, 0x6c757d, 0x495057, 0x343a40, 0x212529, 0x000000};
  uint8_t _barGraphFadeInBrightnessLevels[10] = {5, 10, 15, 25, 30, 64, 92, 128, 200, 255};

  IdlePatterns _currentIdlePattern;

  unsigned long _idlePatternCycleDebounceDuration = 5000;
  unsigned long _idlePatternCycleLastChecked = 0;

  uint8_t _idlePatternSectionBySectionCurrentIndex = 0;
  bool _idlePatternSectionBySectionFill = true;
  unsigned long lastIdleSeciontBySectionCheck;
  int lastIdleSeciontBySectionDuration = 100;
};