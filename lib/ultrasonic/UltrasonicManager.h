#pragma once
#include <Arduino.h>

class UltrasonicManager
{
public:
  UltrasonicManager(int triggerPin, int echoPin) : _triggerPin(triggerPin), _echoPin(echoPin)
  {
  }

  void begin();
  void begin(bool logDistanceToSerialMonitor);

  int readFromSensor();
  void logDistance(int distance);
  int getAveragedDistance();
  int updateMovingAverage(int rawDistance);

private:
  int _triggerPin;
  int _echoPin;

  long duration;
  float speedOfSound = 0.034;

  // * Tracking a moving average
  const static int totalReadings = 3;
  int sensorReadings[totalReadings];
  int currentIndex = 0;
  int currentSum = 0;
};
