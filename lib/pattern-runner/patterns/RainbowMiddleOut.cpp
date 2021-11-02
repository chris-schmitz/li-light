#include "RainbowMiddleOut.h"
#include <helpers.h>

RainbowMiddleOut::RainbowMiddleOut()
{
  _init();
}
RainbowMiddleOut::RainbowMiddleOut(SectionManager *manager) : _sectionManager(manager)
{
  _init();
}

void RainbowMiddleOut::_init()
{
}

void RainbowMiddleOut::tick()
{
  _advanceLevel();
}

void RainbowMiddleOut::_tickRainbow()
{
  uint32_t color = getColor();
  uint16_t level = getLevel();
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

uint32_t RainbowMiddleOut::getColor()
{
  return Wheel((_level * _colorWheelSpacing + _wheelPosition) & 255);
}

uint16_t RainbowMiddleOut::getLevel()
{
  return _level;
}

void RainbowMiddleOut::_advanceLevel()
{
  // char b[100];
  // sprintf(b, "level: %u, wheel pos: %u", _level, _wheelPosition);
  // Serial.println(b);

  if (_level < _totalLightLevels)
  {
    _level++;
  }
  else
  {
    _level = 0;
    _advanceWheelPosition();
  }
}

void RainbowMiddleOut::_advanceWheelPosition()
{
  if (_wheelPosition < _totalWheelPositions)
  {
    _wheelPosition++;
  }
  else
  {
    _wheelPosition = 0;
  }
}