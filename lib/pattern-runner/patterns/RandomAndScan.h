#pragma once
#include "SectionManager.h"
#include <cstdint>

class RandomAndScan
{
public:
  RandomAndScan();
  RandomAndScan(SectionManager *manager);
  void tick();

private:
  SectionManager *_sectionManager;
  uint16_t _tickCount;
  uint16_t _ticksTillNextScan;
  uint32_t _barcodeScanColors[10] = {0xf8f9fa, 0xe9ecef, 0xdee2e6, 0xced4da, 0xadb5bd, 0x6c757d, 0x495057, 0x343a40, 0x212529, 0x000000};
  uint8_t _barGraphFadeInBrightnessLevels[10] = {5, 10, 15, 25, 30, 64, 92, 128, 200, 255};
  unsigned long _lastIntervalCheck = 0;
  unsigned long _intervalDuration = 250;
  void _init();
  void _randomSectionAnimation();
  void _calcTicksTillNextScan();
  void _scanAnimation();
  void _scan();
  void _fadeInLogo();
};
