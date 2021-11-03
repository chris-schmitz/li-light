#pragma once

#include "FastLED.h"
#include "patterns/RainbowMiddleOut.h"
#include "patterns/RandomAndScan.h"
#include "patterns/SectionBySection.h"
#include <SectionManager.h>

enum IdlePatterns
{
  RANDOM_SECTIONS,
  SECTION_BY_SECTION,
  RAINBOW_MIDDLE_OUT
};
const uint8_t TOTAL_NUMBER_OF_IDLE_PATTERNS = 3;

class PatternRunner
{
public:
  PatternRunner(SectionManager *manager) : _sectionManager(manager)
  {
    _rainbowMiddleOut = RainbowMiddleOut(_sectionManager);
    _randomAndScan = RandomAndScan(_sectionManager);
    _sectionBySection = SectionBySection(_sectionManager);
  }

  void runActivePattern();
  void cyclePattern();

private:
  SectionManager *_sectionManager;
  IdlePatterns _currentIdlePattern = RAINBOW_MIDDLE_OUT;

  // ^ Patterns
  RainbowMiddleOut _rainbowMiddleOut;
  RandomAndScan _randomAndScan;
  SectionBySection _sectionBySection;

  void _lightRandomSections(int numberOfSections);
};