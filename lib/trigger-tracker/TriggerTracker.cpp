#include "TriggerTracker.h"

TriggerTracker::TriggerTracker(PatternTriggerLimits idleLimits, PatternTriggerLimits activeLimits) : _idlePatternLimits(idleLimits), _activePatternLimits(activeLimits){};

TriggerEnum TriggerTracker::getTriggerByDistance(int distance)
{
  if (distance > _activePatternLimits.getDistanceThreshold())
  {
    activationPatternCounter = 0;
    switchIdlePatternCounter = 0;
    return IDLE_PATTERN;
  }

  if (distance < _activePatternLimits.getDistanceThreshold())
  {
    startDistanceCounter = true;
  }

  if (distance < _activePatternLimits.getDistanceThreshold() && distance >= _idlePatternLimits.getDistanceThreshold())
  {
    activationPatternCounter++;
  }
  else if (distance < _idlePatternLimits.getDistanceThreshold())
  {
    switchIdlePatternCounter++;
  }

  if (activationPatternCounter >= _activePatternLimits.getCountThreshold())
  {
    activationPatternCounter = 0;
    switchIdlePatternCounter = 0;
    return ACTIVATION_PATTERN;
  }

  if (switchIdlePatternCounter >= _idlePatternLimits.getCountThreshold())
  {
    activationPatternCounter = 0;
    switchIdlePatternCounter = 0;
    return SWITCH_IDLE;
  }

  // TODO: restructure
  // * we need to return this idle pattern because _something_needs to be returned,
  // * and maybe it's the right decision, but this structure is messy
  return IDLE_PATTERN;
}
