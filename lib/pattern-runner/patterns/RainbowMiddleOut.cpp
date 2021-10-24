#include "RainbowMiddleOut.h"
#include <helpers.h>

void RainbowMiddleOut::tick()
{
  _advanceLevel();
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