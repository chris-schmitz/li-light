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
  sprintf(buffer, "Section %i - start: %i, end: %i", sectionIndex, _sections[sectionIndex].getStart(), _sections[sectionIndex].getEnd());
  // Serial.println(buffer);

  if (sectionIndex < 0 || sectionIndex > _totalSections)
    return;

  for (int i = _sections[sectionIndex].getStart(); i <= _sections[sectionIndex].getEnd(); i++)
  {
    _leds[i] = color;
  }
}

void LightManager::setColorValueForRange(PixelRange range, CRGB color)
{
  for (uint8_t i = range.getStart(); i < range.getEnd(); i++)
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
      fill_gradient_RGB(_leds, _sections[i].getStart(), startColor, _sections[i].getEnd(), endColor);
    }
    else
    {
      fill_gradient_RGB(_leds, _sections[i].getStart(), endColor, _sections[i].getEnd(), startColor);
    }
  }
}

void LightManager::lightRandomSections(int numberOfSections)
{

  fill_solid(_leds, _totalLeds, CRGB::Black);

  int randomIndex1 = random(0, _totalSections);
  int randomIndex2 = random(0, _totalSections);

  CRGB randomColor1 = CRGB(random(20, 255), random(20, 255), random(20, 255));
  CRGB randomColor2 = CRGB(random(20, 255), random(20, 255), random(20, 255));

  // setSectionColor(randomIndex1, CRGB::LightSkyBlue);
  setSectionColor(randomIndex1, randomColor1);
  setSectionColor(randomIndex2, randomColor2);
}

void LightManager::setPixel(int index, CRGB color)
{
  _leds[index] = color;
}

void LightManager::clear()
{
  for (int i = 0; i < _totalLeds; i++)
  {
    _leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void LightManager::setSectionLevelColor(int sectionIndex, uint8_t sectionPixelLevel, CRGB color)
{
  bool reverse = sectionIndex % 2 == 0; // TODO: where do we want to put this concept?

  // * if we're not reversed, grab the index that is the start + level
  // * if we are reversed, grab the end index - level

  if (reverse)
  {
    int targetPixelIndex = _sections[sectionIndex].getEnd() - sectionPixelLevel;
    Serial.print("section: ");
    Serial.print(sectionIndex);
    Serial.print(", target pixel: ");
    Serial.println(targetPixelIndex);
    _leds[targetPixelIndex] = color;
    FastLED.show();
  }
}
