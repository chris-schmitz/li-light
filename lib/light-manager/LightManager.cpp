#pragma once
#include "LightManager.h"

void LightManager::setSectionRange(LiLightSection sectionName, int rangeStart, int rangeEnd)
{
  switch (sectionName)
  {
  case BAR_CODE_1:
    barCodeBar1 = {rangeStart, rangeEnd};
    break;
  case BAR_CODE_2:
    barCodeBar2 = {rangeStart, rangeEnd};
    break;
  case BAR_CODE_3:
    barCodeBar3 = {rangeStart, rangeEnd};
    break;
  case BAR_CODE_4:
    barCodeBar4 = {rangeStart, rangeEnd};
    break;
  case BAR_CODE_5:
    barCodeBar5 = {rangeStart, rangeEnd};
    break;
  case BAR_CODE_6:
    barCodeBar6 = {rangeStart, rangeEnd};
    break;

  case GRAPH_LEFT:
    graphLeft = {rangeStart, rangeEnd};
    break;
  case GRAPH_MIDDLE:
    graphMiddle = {rangeStart, rangeEnd};
    break;
  case GRAPH_RIGHT:
    graphRight = {rangeStart, rangeEnd};
    break;
  }
}
void LightManager::setSectionColor(LiLightSection sectionName, uint32_t color)
{
  CRGB formattedColor = CRGB(color);
  setSectionColor(sectionName, formattedColor);
}

void LightManager::setSectionColor(LiLightSection sectionName, uint8_t red, uint8_t green, uint8_t blue)
{
  CRGB color = CRGB(red, green, blue);
  setSectionColor(sectionName, color);
}

void LightManager::setSectionColor(LiLightSection sectionName, CRGB color)
{

  switch (sectionName)
  {
  case BAR_CODE_1:
    setColorValueForRange(barCodeBar1, color);
    break;
  case BAR_CODE_2:
    setColorValueForRange(barCodeBar2, color);
    break;

  case BAR_CODE_3:
    setColorValueForRange(barCodeBar3, color);
    break;
  case BAR_CODE_4:
    setColorValueForRange(barCodeBar4, color);
    break;
  case BAR_CODE_5:
    setColorValueForRange(barCodeBar5, color);
    break;
  case BAR_CODE_6:
    setColorValueForRange(barCodeBar6, color);
    break;

  case GRAPH_LEFT:
    setColorValueForRange(graphLeft, color);
    break;
  case GRAPH_MIDDLE:
    setColorValueForRange(graphMiddle, color);
    break;
  case GRAPH_RIGHT:
    setColorValueForRange(graphRight, color);
    break;
  }
  FastLED.show(); // TODO: consider either pulling this out from this level or triggering it via boolean
}

void LightManager::setColorValueForRange(PixelRange range, CRGB color)
{
  for (uint8_t i = range.start; i < range.end; i++)
  {
    _leds[i] = color;
  }
}

void LightManager::gradientAcrossPixels(uint32_t colorStart, uint32_t endColor)
{

  fill_gradient_RGB(_leds, 0, CRGB(colorStart), 64, CRGB(endColor));
}

void LightManager::randomSections() {
  
}