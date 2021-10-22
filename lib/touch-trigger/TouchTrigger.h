#pragma once
#include <Adafruit_FreeTouch.h>

class TouchTrigger
{
public:
  TouchTrigger(int touchPin)
  {
    _touchSensor = Adafruit_FreeTouch(touchPin, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
  }

  void begin()
  {
    _touchSensor.begin();
  }

  void tick(unsigned long now)
  {
    if (!_debounce)
    {
      return;
    }

    if (now - _lastDebounceTick > _debounceDuration)
    {
      _debounce = false;
    }
  }

  bool touched()
  {
    if (_debounce)
    {
      return false;
    }

    int result = _touchSensor.measure();

    Serial.print("touch result: ");
    Serial.println(result);

    if (result > _TOUCH_THRESHOLD)
    {
      _debounce = true;
      return true;
    }
    return false;
  }

  void setTouchThreshold(int threshold)
  {
    _TOUCH_THRESHOLD = threshold;
  }

  int getTouchThreshold()
  {
    return _TOUCH_THRESHOLD;
  }

private:
  Adafruit_FreeTouch _touchSensor;
  int _TOUCH_THRESHOLD = 700;

  bool _debounce = false;
  unsigned long _debounceDuration = 1000;
  unsigned long _lastDebounceTick = 0;
};