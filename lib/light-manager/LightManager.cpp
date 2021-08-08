#include "LightManager.h"

void LightManager::setSectionRange(PixelRange range)
{
  if (sizeof(_sections) / sizeof(_sections[0]) > _totalSections)
  {
    _sections[_currentSectionIndex] = range;
    _currentSectionIndex++;
  }
}

void LightManager::setSectionColor(int sectionIndex, uint8_t red, uint8_t green, uint8_t blue)
{
  setSectionColor(sectionIndex, CRGB(red, green, blue));
}

void LightManager::setSectionColor(int sectionIndex, CRGB color)
{
  char buffer[31];
  sprintf(buffer, "Section %i - start: %i, end: %i", sectionIndex, _sections[sectionIndex].start, _sections[sectionIndex].end);
  // Serial.println(buffer);

  if (sectionIndex < 0 || sectionIndex > _totalSections)
    return;

  for (int i = _sections[sectionIndex].start; i <= _sections[sectionIndex].end; i++)
  {
    _leds[i] = color;
  }
}

void LightManager::setColorValueForRange(PixelRange range, CRGB color)
{
  for (uint8_t i = range.start; i < range.end; i++)
  {
    _leds[i] = color;
  }
}

// ! note that this only works for single strip sections!!!
void LightManager::gradientAcrossBars(int startIndex, CRGB startColor, int endIndex, CRGB endColor)
{
  // TODO: extend
  // ^ we're starting by assuming start < end, but after getting this working make some private
  // ^ methods to go up or down and then replace the contents of this method with a check for direction

  // TODO add check to make sure start and end are within the bounds of allowed indexes
  for (uint8_t i = startIndex; i < endIndex; i++)
  {
    if (i % 2 == 0)
    {
      fill_gradient_RGB(_leds, _sections[i].start, startColor, _sections[i].end, endColor);
    }
    else
    {
      fill_gradient_RGB(_leds, _sections[i].start, endColor, _sections[i].end, startColor);
    }
  }
}

void LightManager::lightRandomSections(int numberOfSections)
{

  fill_solid(_leds, _totalLeds, CRGB::Black);

  int randomIndex1 = random(0, _totalSections);
  int randomIndex2 = random(0, _totalSections);

  setSectionColor(randomIndex1, CRGB::LightSkyBlue);
  setSectionColor(randomIndex2, CRGB::LightSkyBlue);
}

void LightManager::setPixel(int index, CRGB color)
{
  _leds[index] = color;
}