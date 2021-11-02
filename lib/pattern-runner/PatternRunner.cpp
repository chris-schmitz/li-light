#include "PatternRunner.h"
#include <helpers.h>

void PatternRunner::runCurrentIdlePattern()
{
  switch (_currentIdlePattern)
  {
  case RANDOM_SECTIONS:
    // Serial.println("---> RANDOM SECTIONS");
    _randomAndScan.tick();
    break;
  case SUNSET:
    // Serial.println("---> NON BLOCKING SECTION BY SECTION");
    _sectionBySection.tick();
    break;
  case RAINBOW_MIDDLE_OUT:
    // Serial.println("---> RAINBOW MIDDLE IN");
    _rainbowMiddleOut.tick();
    break;
  }
}

void PatternRunner::cycleIdlePattern()
{
  int newIndex = (_currentIdlePattern + 1) % TOTAL_NUMBER_OF_IDLE_PATTERNS;
  Serial.print("cylce to index: ");
  Serial.println(newIndex);

  _currentIdlePattern = static_cast<IdlePatterns>(newIndex);
  FastLED.clear(true);
}
