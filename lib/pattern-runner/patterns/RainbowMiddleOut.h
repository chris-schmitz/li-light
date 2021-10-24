#pragma once
#include <cstdint>

class RainbowMiddleOut
{
public:
  RainbowMiddleOut() {}
  void tick();
  uint32_t getColor();
  uint16_t getLevel();

private:
  uint16_t _level;
  uint16_t _wheelPosition;

  uint16_t _totalWheelPositions = 256;
  uint8_t _totalLightLevels = 4;
  uint8_t _colorWheelSpacing = 30;

  void _advanceLevel();
  void _advanceWheelPosition();
};
