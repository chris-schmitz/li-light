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

  void setSampleSize(int size);
  void setTresholdOffset(int offset);

  void setDebounceDuration(unsigned long duration);
  unsigned long getDebounceDuration();

  void logTouchInformation(bool state);

private:
  Adafruit_FreeTouch _touchSensor;
  int _touchTreshold = 700;
  bool _logTouchInformation = false;

  bool _debounce = false;
  unsigned long _debounceDuration = 1000;
  unsigned long _lastDebounceTick = 0;

  int _thresholdOffset = 100;
  int _sampleSize = 100;
  void _calculateAverageCapacitance();
};