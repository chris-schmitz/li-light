#include "PatternRunner.h"
#include <helpers.h>

void PatternRunner::runCurrentIdlePattern()
{
  switch (_currentIdlePattern)
  {
  case RANDOM_SECTIONS:
    // Serial.println("---> RANDOM SECTIONS");
    randomSections();
    break;
  case SUNSET:
    // Serial.println("---> NON BLOCKING SECTION BY SECTION");
    nonBlockingSectionBySection();
    break;
  case RAINBOW_MIDDLE_OUT:
    // Serial.println("---> RAINBOW MIDDLE IN");
    rainbowMiddleIn();
    break;
  }
}

void PatternRunner::cycleIdlePattern()
{
  // TODO: consider reimplementing
  // * we may need to put this back in but shortening the duration
  // * if troubleshooting the double touch with the tough trigger doesn't
  // * work.
  // unsigned long now = millis();
  // if (now - _idlePatternCycleLastChecked > _idlePatternCycleDebounceDuration)
  // {
  //   _idlePatternCycleLastChecked = now;
  int newIndex = (_currentIdlePattern + 1) % TOTAL_NUMBER_OF_IDLE_PATTERNS;
  Serial.print("cylce to index: ");
  Serial.println(newIndex);

  _currentIdlePattern = static_cast<IdlePatterns>(newIndex);
  FastLED.clear(true);
  // }
}

void PatternRunner::scanAndFadeIn()
{
  _sectionManager->clearAllSections();
  delay(600);
  gradualBarcodeScan();
  delay(200);
  fadeInBars();
  // * random sparkles??
  delay(3000);
}

void PatternRunner::randomSections()
{
  _lightRandomSections(1);
  FastLED.show();
  delay(200);
}

void PatternRunner::gradualBarcodeScan()
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
      _sectionManager->fillSectionWithColor(sectionIndex, color, FillStyle(ALL_AT_ONCE));
      FastLED.show();
    }
    delay(20);
  }
}

void PatternRunner::nonBlockingSectionBySection()
{
  unsigned long now = millis();
  if (now - lastIdleSeciontBySectionCheck > lastIdleSeciontBySectionDuration)
  {
    lastIdleSeciontBySectionCheck = now;
    if (_idlePatternSectionBySectionFill)
    {
      _sectionManager->fillSectionWithColor(_idlePatternSectionBySectionCurrentIndex, CRGB::DarkMagenta, FillStyle(ALL_AT_ONCE));
    }
    else
    {
      _sectionManager->fillSectionWithColor(_idlePatternSectionBySectionCurrentIndex, CRGB::Black, FillStyle(ALL_AT_ONCE));
    }
    FastLED.show();

    _idlePatternSectionBySectionCurrentIndex = (_idlePatternSectionBySectionCurrentIndex + 1) % 9; // TODO: pull in total sections
    if (_idlePatternSectionBySectionCurrentIndex == 0)
    {
      _idlePatternSectionBySectionFill = !_idlePatternSectionBySectionFill;
    }
  }
}

void PatternRunner::fillSectionBySection()
{
  _sectionManager->fillSectionWithColor(0, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));
  _sectionManager->fillSectionWithColor(1, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));
  _sectionManager->fillSectionWithColor(2, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));
  _sectionManager->fillSectionWithColor(3, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));
  _sectionManager->fillSectionWithColor(4, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));
  _sectionManager->fillSectionWithColor(5, 0x00FFFF, FillStyle(ONE_AT_A_TIME, 100));

  _sectionManager->fillSectionWithColor(6, 0xFFFF00, FillStyle(ONE_AT_A_TIME, 150));
  _sectionManager->fillSectionWithColor(7, 0xFFFF00, FillStyle(ONE_AT_A_TIME, 150));
  _sectionManager->fillSectionWithColor(8, 0xFF00FF, FillStyle(ONE_AT_A_TIME, 150));
}

void PatternRunner::fadeInBars()
{
  for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
  {
    for (uint8_t j = 6; j < 9; j++)
    {
      _sectionManager->fillSectionWithColor(j, CHSV(200, 200, _barGraphFadeInBrightnessLevels[i]), FillStyle(ALL_AT_ONCE));
    }
    FastLED.show();
    delay(50);
  }
  delay(800);
  for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
  {
    for (uint8_t j = 0; j < 6; j++)
    {
      _sectionManager->fillSectionWithColor(j, CHSV(0, 0, _barGraphFadeInBrightnessLevels[i]), FillStyle(ALL_AT_ONCE));
    }
    FastLED.show();
    delay(50);
  }
}

void PatternRunner::rainbowMiddleIn()
{

  // ?! wtf
  _rainbowMiddleOut.tick();
  uint32_t color = _rainbowMiddleOut.getColor();
  uint16_t level = _rainbowMiddleOut.getLevel();
  // // char b[100];
  // // sprintf(b, "level: %d, color: %d ", level, color);
  // // Serial.println(b);

  for (uint8_t i = 0; i < _sectionManager->getSectionCount(); i++)
  {
    _sectionManager->setColorAtLocalIndex(i, level, color);
    FastLED.show();
  }

  delay(10);
}

void PatternRunner::_lightRandomSections(int numberOfSections)
{
  _sectionManager->clearAllSections();

  int randomIndex1 = random(0, _sectionManager->getSectionCount());
  int randomIndex2 = random(0, _sectionManager->getSectionCount());

  CRGB randomColor1 = CRGB(random(20, 255), random(20, 255), random(20, 255));
  CRGB randomColor2 = CRGB(random(20, 255), random(20, 255), random(20, 255));

  // char buffer[100];
  // sprintf(buffer, "Random color 1: %d, Random color 2: %d", randomColor1, randomColor2);
  // Serial.println(buffer);

  _sectionManager->fillSectionWithColor(randomIndex1, randomColor1, FillStyle(ALL_AT_ONCE));
  _sectionManager->fillSectionWithColor(randomIndex2, randomColor2, FillStyle(ALL_AT_ONCE));

  FastLED.show();
}