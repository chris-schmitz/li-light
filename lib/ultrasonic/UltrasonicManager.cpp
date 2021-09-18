#include <UltrasonicManager.h>

void UltrasonicManager::begin()
{
  pinMode(_triggerPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

int UltrasonicManager::readFromSensor()
{

  digitalWrite(_triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_triggerPin, LOW);

  duration = pulseIn(_echoPin, HIGH);
  // * Speed of sound wave divided by 2 (out and back)
  int distance = duration * speedOfSound / 2;

  return distance;
}

void UltrasonicManager::logDistance(int distance)
{
  Serial.print(distance);
  Serial.print(" cm");
  for (int i = 0; i < distance; i++)
  {
    Serial.print("-");
  }
  Serial.println("|");
}

int UltrasonicManager::getAveragedDistance()
{
  return updateMovingAverage(readFromSensor());
}

int UltrasonicManager::updateMovingAverage(int rawDistance)
{
  currentSum -= sensorReadings[currentIndex];
  sensorReadings[currentIndex] = rawDistance;
  currentSum += rawDistance;
  currentIndex = (currentIndex + 1) % totalReadings;

  return currentSum / totalReadings;
}