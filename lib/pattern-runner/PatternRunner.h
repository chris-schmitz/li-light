#pragma once

// #include "../light-manager/LightManager.h"
#include "FastLED.h"
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
  PatternRunner(SectionManager *manager) : _sectionManager(manager) {}

  void runCurrentIdlePattern();
  void cycleIdlePattern();

  // * Patterns
  void scanAndFadeIn();
  void randomSections();
  void gradualBarcodeScan();
  void nonBlockingSectionBySection();
  void fillSectionBySection();
  void fadeInBars();
  void rainbowMiddleOut();
  void rainbowMiddleIn();

private:
  SectionManager *_sectionManager;

  uint32_t _barcodeScanColors[10] = {0xf8f9fa, 0xe9ecef, 0xdee2e6, 0xced4da, 0xadb5bd, 0x6c757d, 0x495057, 0x343a40, 0x212529, 0x000000};
  uint8_t _barGraphFadeInBrightnessLevels[10] = {5, 10, 15, 25, 30, 64, 92, 128, 200, 255};

  IdlePatterns _currentIdlePattern;

  // TODO: cleanup/refactor
  // ^ this feels super sloppy, clean it up
  unsigned long _idlePatternCycleDebounceDuration = 5000;
  unsigned long _idlePatternCycleLastChecked = 0;

  uint8_t _idlePatternSectionBySectionCurrentIndex = 0;
  bool _idlePatternSectionBySectionFill = true;
  unsigned long lastIdleSeciontBySectionCheck;
  int lastIdleSeciontBySectionDuration = 100;

  uint8_t _rainbowMiddleOutColorWheelIndex = 0;
  uint8_t _rainbowMiddleOutColorLevelInSection = 0;
  unsigned long _rainbowMiddleOutLastChecked = 0;
  unsigned long _rainbowMiddleOutInterval = 500;

  void _lightRandomSections(int numberOfSections);
};