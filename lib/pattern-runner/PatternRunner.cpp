#include "PatternRunner.h"
#include "../helpers.h"

void PatternRunner::runCurrentIdlePattern()
{
  switch (_currentIdlePattern)
  {
  case RANDOM_SECTIONS:
    randomSections();
    break;
  case SUNSET:
    nonBlockingSectionBySection();
    break;
  case RAINBOW_MIDDLE_OUT:
    rainbowMiddleOut();
    break;
  }
}

void PatternRunner::cycleIdlePattern()
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

void PatternRunner::scanAndFadeIn()
{
  _lightManager->clear();
  delay(600);
  gradualBarcodeScan();
  delay(200);
  fadeInBars();
  // * random sparkles??
  delay(3000);
}

void PatternRunner::randomSections()
{
  _lightManager->lightRandomSections(1);
  FastLED.show();
  delay(100);
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
      _lightManager->setSectionColor(sectionIndex, color);
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

void PatternRunner::fillSectionBySection()
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

void PatternRunner::fadeInBars()
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

void PatternRunner::rainbowMiddleOut()
{
  // ! needs adjustments
  // ^ We run into a multi part issue:
  // * - the code and graph bars have different pixel counts, so they will have different numbers of "levels"
  // * - if we handle the incrementation of section levels by blind math (e.g. start + x), then when we get
  // *    to a level where some of the sections don't actually have enough pixels to reach that level, we'll
  // *    start overwriting pixels in the ajoining sections
  // * - on top of that previous isssue, if that happens on the sections at either end of the overall led strip
  // *    then we'll extend the indexes outside of the overall strip's range likely what's causing the MC crash
  // *    we're seeing now.
  // ^ So, all of that said, it feels like we need to add awareness of "level" into the pixel range struct itself,
  // ^ and possibly change the struct to a class that we add business logic into vs keeping it as a structured
  // ^ accessor object
  unsigned long now = millis(); // TODO: refactor to grab one millis per idle pattern call
  if (now - _rainbowMiddleOutLastChecked > _rainbowMiddleOutInterval)
  {
    _rainbowMiddleOutLastChecked = now;
    CRGB color = Wheel(_rainbowMiddleOutColorWheelIndex);

    char buffer[50];
    sprintf(buffer, "Level: %i, Color: %i", _rainbowMiddleOutColorLevelInSection, color);
    Serial.println(buffer);
    Serial.println("--------");
    for (uint8_t section = 0; section < 9; section++) // TODO bring in total sections
    {
      _lightManager->setSectionLevelColor(section, _rainbowMiddleOutColorLevelInSection, color);
    }
    _rainbowMiddleOutColorWheelIndex++;
    _rainbowMiddleOutColorLevelInSection++;
  }
}