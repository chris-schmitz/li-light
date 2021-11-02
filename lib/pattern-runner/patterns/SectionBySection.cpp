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

// TODO: cleanup
// * this feels a bit sloppy, either abstract or rethink the implementaiton
bool SectionBySection::_shouldHold()
{
  if (_hold == false)
  {
    return false;
  }

  _holdCount++;
  if (_holdCount > _totalHoldCount)
  {
    _hold = false;
    _holdCount = 0;
    return false;
  }
  return true;
}

void SectionBySection::_advanceSection()
{
  if (_shouldHold())
  {
    return;
  }

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
    if (_fillSections == false)
    {
      _hold = true;
    }
  }
}