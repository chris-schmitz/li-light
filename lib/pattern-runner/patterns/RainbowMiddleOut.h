#pragma once
#include "SectionManager.h"
#include <cstdint>

class RainbowMiddleOut
{
public:
  RainbowMiddleOut();
  RainbowMiddleOut(SectionManager *manager);
  void tick();
  uint32_t getColor();
  uint16_t getLevel();

private:
  SectionManager *_sectionManager;

  uint16_t _level;
  uint16_t _wheelPosition;

  uint16_t _totalWheelPositions = 256;
  uint8_t _totalLightLevels = 4;
  uint8_t _colorWheelSpacing = 30;

  void _init();
  void _tickRainbow();
  void _advanceLevel();
  void _advanceWheelPosition();
};
