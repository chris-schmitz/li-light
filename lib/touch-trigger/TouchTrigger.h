#pragma once
#include <Adafruit_FreeTouch.h>

class TouchTrigger
{
public:
  TouchTrigger(int touchPin);

  void begin();
  void tick(unsigned long now);
  bool touched();

  void setTouchThreshold(int threshold);
  int getTouchThreshold();

  void setDebounceDuration(unsigned long duration);
  unsigned long getDebounceDuration();

  void logTouchInformation(bool state);

private:
  Adafruit_FreeTouch _touchSensor;
  int _TOUCH_THRESHOLD = 700;
  bool _logTouchInformation = false;

  bool _debounce = false;
  unsigned long _debounceDuration = 1000;
  unsigned long _lastDebounceTick = 0;
};