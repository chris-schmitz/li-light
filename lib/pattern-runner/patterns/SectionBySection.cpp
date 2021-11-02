#include "SectionBySection.h"

SectionBySection::SectionBySection()
{
  _init();
}
SectionBySection::SectionBySection(SectionManager *manager) : _sectionManager(manager)
{
  _init();
}

void SectionBySection::_init()
{
  _lastIntervalCheck = 0;
  _intervalDuration = 75;
  _fillSections = true;
  _currentSectionIndex = 0;
  _fillColor = _randomColor();
}

void SectionBySection::tick()
{
  unsigned long now = millis();
  if (now - _lastIntervalCheck > _intervalDuration)
  {
    _lastIntervalCheck = now;
    _advanceSection();
  }
}

CRGB SectionBySection::_randomColor()
{
  return CRGB(random8(), random8(), random8());
}

void SectionBySection::_advanceSection()
{
  if (_fillSections)
  {
    _sectionManager->fillSectionWithColor(_currentSectionIndex, _fillColor, FillStyle(ALL_AT_ONCE));
  }
  else
  {
    _sectionManager->fillSectionWithColor(_currentSectionIndex, CRGB::Black, FillStyle(ALL_AT_ONCE));
  }
  FastLED.show();

  _currentSectionIndex = (_currentSectionIndex + 1) % 9; // TODO: pull in total sections
  if (_currentSectionIndex == 0)
  {
    _fillSections = !_fillSections;
    _fillColor = _randomColor();
  }
}