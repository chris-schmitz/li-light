#include "LightManager.h"
#include "PatternRunner.h"
#include <Arduino.h>

#include <FastLED.h>

#define ECHO_PIN 1
#define TRIGGER_PIN 2

#define LED_STRIP 3
#define LED_COUNT 42

bool LOG_DISTANCE_TO_MONITOR = false;
long duration;
float speedOfSound = 0.034;

// * Tracking a moving average
const int totalReadings = 3;
int sensorReadings[totalReadings];
int currentIndex = 0;
int currentSum = 0;

uint8_t lightLevel;
CRGB leds[LED_COUNT];
uint8_t hue = 253;
uint8_t saturation = 255;

LightManager lightManager = LightManager(leds, LED_COUNT);
PatternRunner patternRunner = PatternRunner(&lightManager);

void setupUltrasonicSensor()
{
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void setupLedStrip()
{
  FastLED.addLeds<NEOPIXEL, LED_STRIP>(leds, LED_COUNT);
}

void setupLiLightManager()
{
  lightManager.setSectionRange(PixelRange{0, 3});
  lightManager.setSectionRange(PixelRange{4, 7});
  lightManager.setSectionRange(PixelRange{8, 11});
  lightManager.setSectionRange(PixelRange{12, 15});
  lightManager.setSectionRange(PixelRange{16, 19});
  lightManager.setSectionRange(PixelRange{20, 23});

  lightManager.setSectionRange(PixelRange{24, 31});
  lightManager.setSectionRange(PixelRange{32, 35});
  lightManager.setSectionRange(PixelRange{36, 41});
}

void setup()
{
  setupUltrasonicSensor();
  setupLedStrip();
  setupLiLightManager();

  Serial.begin(9600);
}

int readSensor()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  // * Speed of sound wave divided by 2 (out and back)
  return duration * speedOfSound / 2;
}

void logDistance(int distance)
{
  Serial.print(distance);
  Serial.print(" cm");
  for (int i = 0; i < distance; i++)
  {
    Serial.print("-");
  }
  Serial.println("|");
}

int updateMovingAverage(int rawDistance)
{
  currentSum -= sensorReadings[currentIndex];
  sensorReadings[currentIndex] = rawDistance;
  currentSum += rawDistance;
  currentIndex = (currentIndex + 1) % totalReadings;

  return currentSum / totalReadings;
}

enum TriggerEnum
{
  IDLE_PATTERN,
  ACTIVATION_PATTERN,
  SWITCH_IDLE
};

int lastLoggedDistance = 0;
const int activationPatternThreshold = 40;
const int switchIdlePatternThreshold = 10;
bool startDistanceCounter = false;
int activationPatternCounter = 0;
int switchIdlePatternCounter = 0;
int activationPatternTotalCount = 5;
int switchIdlePatternDistanceTotalCount = 2;
TriggerEnum triggerOnCloseDistanceCheck(int distance)
{
  // TODO: consider adjustments
  // ? do we really need the artifical delay here if we already an distance average?
  // ? it seems like we could use one or the other
  if (distance > activationPatternThreshold)
  {
    activationPatternCounter = 0;
    switchIdlePatternCounter = 0;
    return IDLE_PATTERN;
  }

  if (distance < activationPatternThreshold)
  {
    startDistanceCounter = true;
  }

  Serial.println(distance < activationPatternThreshold && distance >= switchIdlePatternThreshold);
  if (distance < activationPatternThreshold && distance >= switchIdlePatternThreshold)
  {
    activationPatternCounter++;
  }
  else if (distance < switchIdlePatternThreshold)
  {
    switchIdlePatternCounter++;
  }

  if (activationPatternCounter >= activationPatternTotalCount)
  {
    activationPatternCounter = 0;
    switchIdlePatternCounter = 0;
    return ACTIVATION_PATTERN;
  }

  if (switchIdlePatternCounter >= switchIdlePatternDistanceTotalCount)
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

unsigned long distanceCheckInterval = 10;
unsigned long distanceLastChecked = 0;

void loop()
{
  unsigned long now = millis();

  if (now - distanceLastChecked > distanceCheckInterval)
  {
    distanceLastChecked = now;
    lastLoggedDistance = updateMovingAverage(readSensor());

    if (LOG_DISTANCE_TO_MONITOR)
    {
      logDistance(lastLoggedDistance); // ! test method
    }

    TriggerEnum currentTrigger = triggerOnCloseDistanceCheck(lastLoggedDistance);

    switch (currentTrigger)
    {
    case ACTIVATION_PATTERN:
      Serial.println("activation pattern");
      lightManager.clear();
      delay(600);
      patternRunner.gradualBarcodeScan();
      delay(200);
      patternRunner.fadeInBars();
      // * random sparkles??
      delay(3000);
      break;
    case SWITCH_IDLE:
      patternRunner.cycleIdlePattern();
      break;
    default:
      patternRunner.runCurrentIdlePattern();
      // patternRunner.randomSections();
      break;
    }
  }
}
