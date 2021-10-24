#pragma once
#include "TouchTrigger.h"

TouchTrigger::TouchTrigger(int touchPin)
{
  _touchSensor = Adafruit_FreeTouch(touchPin, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
}

void TouchTrigger::begin()
{
  _touchSensor.begin();
}

void TouchTrigger::tick(unsigned long now)
{
  if (_debounce == false)
  {
    return;
  }

  if (_logTouchInformation)
  {
    char b[100];
    sprintf(b, "now: %d, last: %d, running difference: %d, duration: %d", now, _lastDebounceTick, (now - _lastDebounceTick), _debounceDuration);
    Serial.println(b);
  }

  if (now - _lastDebounceTick > _debounceDuration)
  {
    _lastDebounceTick = now;
    if (_logTouchInformation)
      Serial.println("--> End debounce");
    _debounce = false;
  }
}

bool TouchTrigger::touched()
{
  if (_logTouchInformation)
  {
    Serial.print("DEBOUNCE: ");
    Serial.println(_debounce);
  }

  if (_debounce == true)
  {
    return false;
  }

  int result = _touchSensor.measure();

  if (_logTouchInformation)
  {
    Serial.print("touch result: ");
    Serial.println(result);
  }

  if (result > _TOUCH_THRESHOLD)
  {
    if (_logTouchInformation)
      Serial.println("--> Start debounce");
    _debounce = true;
    return true;
  }
  return false;
}

void TouchTrigger::setTouchThreshold(int threshold)
{
  _TOUCH_THRESHOLD = threshold;
}

int TouchTrigger::getTouchThreshold()
{
  return _TOUCH_THRESHOLD;
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