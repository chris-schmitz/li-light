#include "LightManager.h"
#include <Arduino.h>

#include <FastLED.h>

#define ECHO_PIN 1
#define TRIGGER_PIN 2

#define LED_STRIP 3
#define LED_COUNT 42

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

void randomSections()
{
  lightManager.lightRandomSections(1);
  FastLED.show();
  delay(100);
}

uint32_t colors[] = {0xf8f9fa, 0xe9ecef, 0xdee2e6, 0xced4da, 0xadb5bd, 0x6c757d, 0x495057, 0x343a40, 0x212529, 0x000000};

void gradualBarcodeScan()
{
  // * the state we want to push across the bars
  // * this is a colleciton of colors indexes to use for each bar
  // * each byte corresponds to a bar
  byte frameState[] = {9, 9, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9};

  int totalFrames = 23;
  for (int frame = 0; frame < totalFrames; frame++)
  {
    for (int sectionIndex = 0; sectionIndex < 6; sectionIndex++)
    {
      CRGB color = CRGB::Black;
      if (sectionIndex + frame + 1 < totalFrames)
      {
        color = CRGB(colors[frameState[sectionIndex + frame]]);
      }
      lightManager.setSectionColor(sectionIndex, color);
      FastLED.show();
      // delay(10);
    }
    delay(20);
  }
}

void fillSectionBySection()
{
  for (uint8_t i = 0; i < 9; i++)
  {
    lightManager.setSectionColor(i, CRGB::BlueViolet);
    FastLED.show();
    delay(100);
  }
  for (uint8_t i = 0; i < 9; i++)
  {
    lightManager.setSectionColor(i, CRGB::Black);
    FastLED.show();
    delay(100);
  }
}

int level1[6] = {24, 31, 32, 35, 36, 41};
int level2[6] = {25, 30, 33, 34, 37, 40};
int level3[4] = {26, 29, 38, 39};
int level4[2] = {27, 28};
int raiseDelay = 500;
void raiseGraphBars()
{
  for (uint8_t i = 0; i < 6; i++)
  {
    lightManager.setPixel(level1[i], CRGB::LimeGreen);
  }
  FastLED.show();
  delay(raiseDelay);
  for (uint8_t i = 0; i < 6; i++)
  {
    lightManager.setPixel(level2[i], CRGB::LimeGreen);
  }
  FastLED.show();
  delay(raiseDelay);
  for (uint8_t i = 0; i < 4; i++)
  {
    lightManager.setPixel(level3[i], CRGB::LimeGreen);
  }
  FastLED.show();
  delay(raiseDelay);
  for (uint8_t i = 0; i < 2; i++)
  {
    lightManager.setPixel(level4[i], CRGB::LimeGreen);
  }
  FastLED.show();
  delay(raiseDelay);
}

uint8_t brightnessLevels[] = {5, 10, 15, 25, 30, 64, 92, 128, 200, 255};
void fadeInBars()
{
  for (uint8_t i = 0; i < sizeof(brightnessLevels) / sizeof(brightnessLevels[0]); i++)
  {
    Serial.println(i);
    for (uint8_t j = 6; j < 9; j++)
    {
      lightManager.setSectionColor(j, CHSV(200, 200, brightnessLevels[i]));
    }
    FastLED.show();
    delay(50);
  }
  delay(800);
  for (uint8_t i = 0; i < sizeof(brightnessLevels) / sizeof(brightnessLevels[0]); i++)
  {
    Serial.println(i);
    for (uint8_t j = 0; j < 6; j++)
    {
      lightManager.setSectionColor(j, CHSV(0, 0, brightnessLevels[i]));
    }
    FastLED.show();
    delay(50);
  }
  Serial.println("---");
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

  // Serial.print("activationPatternCounter: ");
  // Serial.println(activationPatternCounter);
  // Serial.print("activationPatterntotal: ");
  // Serial.println(activationPatternTotalCount);
  // Serial.print("switchIdlePatternCounter: ");
  // Serial.println(switchIdlePatternCounter);
  // Serial.print("switch idle total count: ");
  // Serial.println(switchIdlePatternDistanceTotalCount);

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

    logDistance(lastLoggedDistance); // ! test method

    TriggerEnum currentTrigger = triggerOnCloseDistanceCheck(lastLoggedDistance);

    switch (currentTrigger)
    {
    case ACTIVATION_PATTERN:
      Serial.println("activation pattern");
      lightManager.clear();
      delay(600);
      gradualBarcodeScan();
      delay(200);
      fadeInBars();
      // * random sparkles??
      delay(3000);
      break;
    case SWITCH_IDLE:
      Serial.println("SWITCH IDLE");
      break;
    default:
      randomSections();
      break;
    }
  }
}
