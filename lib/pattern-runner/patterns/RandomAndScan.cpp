#include "RandomAndScan.h"

RandomAndScan::RandomAndScan()
{
  _init();
}
RandomAndScan::RandomAndScan(SectionManager *manager) : _sectionManager(manager)
{
  _init();
}

void RandomAndScan::_init()
{
  _tickCount = 0;
  _calcTicksTillNextScan();
}

void RandomAndScan::tick()
{
  unsigned long now = millis();

  if (now - _lastIntervalCheck > _intervalDuration)
  {
    _lastIntervalCheck = now;

    if (_tickCount > _ticksTillNextScan)
    {
      _scanAnimation();
      _tickCount = 0;
      _calcTicksTillNextScan();
    }
    else
    {
      _tickCount++;
      _randomSectionAnimation();
    }
  }
}

void RandomAndScan::_randomSectionAnimation()
{
  _sectionManager->clearAllSections();

  uint8_t index1 = random(0, _sectionManager->getSectionCount());
  uint8_t index2 = random(0, _sectionManager->getSectionCount());

  CRGB color1 = CRGB(random(20, 255), random(20, 255), random(20, 255));
  CRGB color2 = CRGB(random(20, 255), random(20, 255), random(20, 255));

  _sectionManager->fillSectionWithColor(index1, color1, FillStyle(ALL_AT_ONCE));
  _sectionManager->fillSectionWithColor(index2, color2, FillStyle(ALL_AT_ONCE));

  FastLED.show();
}

void RandomAndScan::_calcTicksTillNextScan()
{
  _ticksTillNextScan = random(30, 100);
}

void RandomAndScan::_scanAnimation()
{
  _sectionManager->clearAllSections();
  delay(600);
  _scan();
  delay(200);

  _fadeInLogo();
  // * random sparkles??
  delay(3000);
}

void RandomAndScan::_scan()
{
  // * the state we want to push across the bars
  // * this is a colleciton of colors indexes to use for each bar
  // * each byte corresponds to a bar
  byte frameState[] = {9, 9, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9};

  int totalFrames = 23;
  for (int frame = 0; frame < totalFrames; frame++)
  {
    for (int sectionIndex = 0; sectionIndex < 6; sectionIndex++)
    {
      CRGB color = CRGB::Black;
      if (sectionIndex + frame + 1 < totalFrames)
      {
        color = CRGB(_barcodeScanColors[frameState[sectionIndex + frame]]);
      }
      _sectionManager->fillSectionWithColor(sectionIndex, color, FillStyle(ALL_AT_ONCE));
      FastLED.show();
    }
    delay(20);
  }
}

void RandomAndScan::_fadeInLogo()
{
  for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
  {
    for (uint8_t j = 6; j < 9; j++)
    {
      _sectionManager->fillSectionWithColor(j, CHSV(200, 200, _barGraphFadeInBrightnessLevels[i]), FillStyle(ALL_AT_ONCE));
    }
    FastLED.show();
    delay(50);
  }
  delay(800);
  for (uint8_t i = 0; i < sizeof(_barGraphFadeInBrightnessLevels) / sizeof(_barGraphFadeInBrightnessLevels[0]); i++)
  {
    for (uint8_t j = 0; j < 6; j++)
    {
      _sectionManager->fillSectionWithColor(j, CHSV(0, 0, _barGraphFadeInBrightnessLevels[i]), FillStyle(ALL_AT_ONCE));
    }
    FastLED.show();
    delay(50);
  }
}