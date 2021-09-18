#pragma once

enum TriggerEnum
{
  IDLE_PATTERN,
  ACTIVATION_PATTERN,
  SWITCH_IDLE
};

class PatternTriggerLimits
{
public:
  PatternTriggerLimits(
      TriggerEnum type, int distanceThreshold, int totalCountThreshold)
      : _type(type),
        _distanceThreshold(distanceThreshold),
        _totalCountThreshold(totalCountThreshold){};
  int getDistanceThreshold()
  {
    return _distanceThreshold;
  }
  int getCountThreshold()
  {
    return _totalCountThreshold;
  }

private:
  TriggerEnum _type;
  int _distanceThreshold;
  int _totalCountThreshold;
};

class TriggerTracker
{
public:
  TriggerTracker();
  TriggerTracker(PatternTriggerLimits idleLimits, PatternTriggerLimits activeLimits);

  TriggerEnum getTriggerByDistance(int distance);

private:
  PatternTriggerLimits _idlePatternLimits;
  PatternTriggerLimits _activePatternLimits;

  bool startDistanceCounter = false;

  int activationPatternCounter = 0;
  int switchIdlePatternCounter = 0;
};
