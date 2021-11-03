#include "PatternRunner.h"
#include <helpers.h>

void PatternRunner::runActivePattern()
{
  switch (_currentIdlePattern)
  {
  case RANDOM_SECTIONS:
    _randomAndScan.tick();
    break;
  case SECTION_BY_SECTION:
    _sectionBySection.tick();
    break;
  case RAINBOW_MIDDLE_OUT:
    _rainbowMiddleOut.tick();
    break;
  }
}

void PatternRunner::cyclePattern()
{
  int newIndex = (_currentIdlePattern + 1) % TOTAL_NUMBER_OF_IDLE_PATTERNS;
  Serial.print("cylce to index: ");
  Serial.println(newIndex);

  _currentIdlePattern = static_cast<IdlePatterns>(newIndex);
  if (_currentIdlePattern == SECTION_BY_SECTION)
  {
    _sectionBySection.reset();
  }

  FastLED.clear(true);
}
