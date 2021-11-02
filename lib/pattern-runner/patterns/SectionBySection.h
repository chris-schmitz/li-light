#pragma once
#include "SectionManager.h"
#include <cstdint>

class SectionBySection
{
public:
  SectionBySection();
  SectionBySection(SectionManager *manager);
  void tick();

private:
  SectionManager *_sectionManager;
  unsigned long _lastIntervalCheck;
  unsigned long _intervalDuration;
  bool _fillSections;
  int _currentSectionIndex;
  CRGB _fillColor;
  void _init();
  void _advanceSection();
  CRGB _randomColor();
};
