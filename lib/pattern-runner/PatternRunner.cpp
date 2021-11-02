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
    rainbowMiddleOut();
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

void PatternRunner::rainbowMiddleOut()
{

  _rainbowMiddleOut.tick();
  uint32_t color = _rainbowMiddleOut.getColor();
  uint16_t level = _rainbowMiddleOut.getLevel();
  // char b[100];
  // sprintf(b, "level: %d, color: %d ", level, color);
  // Serial.println(b);

  for (uint8_t i = 0; i < _sectionManager->getSectionCount(); i++)
  {
    _sectionManager->setColorAtLocalIndex(i, level, color);
    FastLED.show();
  }

  delay(10);
}
