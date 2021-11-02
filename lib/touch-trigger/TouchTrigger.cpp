#pragma once
#include "TouchTrigger.h"

TouchTrigger::TouchTrigger(int touchPin)
{
  _touchSensor = Adafruit_FreeTouch(touchPin, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
}

void TouchTrigger::begin()
{
  _touchSensor.begin();
  _calculateAverageCapacitance();
}

bool TouchTrigger::touched(unsigned long now)
{

  if (_debounce == true)
  {
    _tickDebounce(now);
    return false;
  }

  if (_touchTriggered())
  {
    _debounce = true;
    _lastDebounceTick = now;
    return true;
  }

  return false;
}

void TouchTrigger::setTouchThreshold(int threshold)
{
  _touchThreshold = threshold;
}

int TouchTrigger::getTouchThreshold()
{
  return _touchThreshold;
}

void TouchTrigger::setDebounceDuration(unsigned long duration)
{
  _debounceDuration = duration;
}

unsigned long TouchTrigger::getDebounceDuration()
{
  return _debounceDuration;
}

void TouchTrigger::logTouchInformation(bool state)
{
  _logTouchInformation = state;
}

void TouchTrigger::setSampleSize(int size)
{
  _sampleSize = size;
}

void TouchTrigger::setTresholdOffset(int offset)
{
  _thresholdOffset = offset;
}

void TouchTrigger::_calculateAverageCapacitance()
{
  int samples = 0;

  for (int i = 0; i < _sampleSize; i++)
  {
    samples += _touchSensor.measure();
  }

  int average = samples / _sampleSize;
  _touchThreshold = average + _thresholdOffset;

  char b[100];
  sprintf(b, "average: %i, threshold: %i", average, _touchThreshold);
  Serial.println(b);
}

bool TouchTrigger::_touchTriggered()
{
  int reading = _touchSensor.measure();

  if (reading > _touchThreshold)
  {
    return true;
  }
  return false;
}

void TouchTrigger::_tickDebounce(unsigned long now)
{
  if (now - _lastDebounceTick > _debounceDuration)
  {
    _lastDebounceTick = now;
    _debounce = false;
  }
}