#pragma once
#include "SectionManager.h"
#include <cstdint>

class SectionBySection
{
public:
  SectionBySection();
  SectionBySection(SectionManager *manager);
  void tick();
  void reset();

private:
  SectionManager *_sectionManager;
  unsigned long _lastIntervalCheck;
  unsigned long _intervalDuration;
  bool _fillSections;
  int _currentSectionIndex;
  uint8_t _holdCount = 0;
  uint8_t _totalHoldCount = 100;
  CRGB _fillColor;
  bool _hold = false;
  void _init();
  void _advanceSection();
  bool _shouldHold();
  CRGB _randomColor();
};
