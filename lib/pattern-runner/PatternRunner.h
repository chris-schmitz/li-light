#pragma once

#include "FastLED.h"
#include "patterns/RainbowMiddleOut.h"
#include "patterns/RandomAndScan.h"
#include "patterns/SectionBySection.h"
#include <SectionManager.h>

enum IdlePatterns
{
  RANDOM_SECTIONS,
  SUNSET,
  RAINBOW_MIDDLE_OUT
};
const uint8_t TOTAL_NUMBER_OF_IDLE_PATTERNS = 3;

class PatternRunner
{
public:
  PatternRunner(SectionManager *manager) : _sectionManager(manager)
  {
    _rainbowMiddleOut = RainbowMiddleOut();
    _randomAndScan = RandomAndScan(_sectionManager);
    _sectionBySection = SectionBySection(_sectionManager);
  }

  void runCurrentIdlePattern();
  void cycleIdlePattern();

  // * Patterns
  void scanAndFadeIn();
  void randomSections();
  void gradualBarcodeScan();
  void fillSectionBySection();
  void fadeInBars();
  void rainbowMiddleOut();

private:
  SectionManager *_sectionManager;


  IdlePatterns _currentIdlePattern = RAINBOW_MIDDLE_OUT;

  // TODO: cleanup/refactor
  // ^ this feels super sloppy, clean it up
  unsigned long _idlePatternCycleDebounceDuration = 5000;
  unsigned long _idlePatternCycleLastChecked = 0;

  // ^ Patterns
  RainbowMiddleOut _rainbowMiddleOut;
  RandomAndScan _randomAndScan;
  SectionBySection _sectionBySection;

  void _lightRandomSections(int numberOfSections);
};