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
const int totalReadings = 50;
int sensorReadings[totalReadings];
int currentIndex = 0;
int currentSum = 0;

uint8_t lightLevel;
CRGB leds[LED_COUNT];
uint8_t hue = 253;
uint8_t saturation = 255;

LightManager lightManager = LightManager(leds);

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
  lightManager.setSectionRange(BAR_CODE_1, 0, 3);
  lightManager.setSectionRange(BAR_CODE_2, 4, 7);
  lightManager.setSectionRange(BAR_CODE_3, 8, 11);
  lightManager.setSectionRange(BAR_CODE_4, 12, 15);
  lightManager.setSectionRange(BAR_CODE_5, 16, 19);
  lightManager.setSectionRange(BAR_CODE_6, 20, 23);

  lightManager.setSectionRange(GRAPH_RIGHT, 24, 32);
  lightManager.setSectionRange(GRAPH_MIDDLE, 33, 36);
  lightManager.setSectionRange(GRAPH_LEFT, 37, 42);

  lightManager.setBars();
}

void setup()
{
  setupUltrasonicSensor();
  setupLedStrip();
  setupLiLightManager();

  Serial.begin(9600);
  while (!Serial)
    ;
}

void updateLEDs(int brightnessLevel)
{
  lightLevel = map(brightnessLevel, 0, 100, 255, 0);

  // for (int i = 0; i < LED_COUNT; i++)
  // {
  //   hue += 1;
  //   if (hue % 255 == 0)
  //   {
  //     hue = 0;
  //   }
  //   leds[i] = CHSV(hue, saturation, lightLevel);
  // }
  fill_solid(leds, LED_COUNT, CHSV(hue, saturation, lightLevel));
  FastLED.show();
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

void activationAnimation()
{
}

unsigned long colorChangeInterval = 500;
unsigned long colorChangeLastChecked = 0;

bool flip = false;
void loop()
{

  Serial.print("size of int: ");
  Serial.println(sizeof(int));
  Serial.print("size of pixel range: ");
  Serial.println(sizeof(PixelRange));
  Serial.print("size of pixel range for bars: ");
  Serial.println(6 * sizeof(PixelRange));
  Serial.print("size of bars: ");
  Serial.println(sizeof(lightManager.bars));
  Serial.print("Mem address of barCodeBar1: ");
  Serial.println((long)&lightManager.barCodeBar1, HEX);
  Serial.print("Mem address of bars[0]: ");
  Serial.println((long)&lightManager.bars[0], HEX);
  while (true)
    ;

  unsigned long now = millis();
  if (now - colorChangeLastChecked > colorChangeInterval)
  {
    colorChangeLastChecked = now;
    if (flip == true)
    {
      lightManager.setSectionColor(BAR_CODE_1, 255, 0, 255);
      lightManager.setSectionColor(BAR_CODE_2, 0, 255, 255);
      lightManager.setSectionColor(BAR_CODE_3, 255, 0, 255);
      lightManager.setSectionColor(BAR_CODE_4, 0, 255, 255);
      lightManager.setSectionColor(BAR_CODE_5, 255, 0, 255);
      lightManager.setSectionColor(BAR_CODE_6, 0, 255, 255);

      lightManager.setSectionColor(GRAPH_LEFT, 255, 0, 255);
      lightManager.setSectionColor(GRAPH_MIDDLE, 0, 255, 255);
      lightManager.setSectionColor(GRAPH_RIGHT, 255, 0, 255);
      flip = false;
    }
    else
    {
      lightManager.setSectionColor(BAR_CODE_2, 255, 0, 255);
      lightManager.setSectionColor(BAR_CODE_3, 0, 255, 255);
      lightManager.setSectionColor(BAR_CODE_4, 255, 0, 255);
      lightManager.setSectionColor(BAR_CODE_5, 0, 255, 255);
      lightManager.setSectionColor(BAR_CODE_6, 255, 0, 255);

      lightManager.setSectionColor(GRAPH_LEFT, 0, 255, 255);
      lightManager.setSectionColor(GRAPH_MIDDLE, 255, 0, 255);
      lightManager.setSectionColor(GRAPH_RIGHT, 0, 255, 255);
      flip = true;
    }

    //   int distance = updateMovingAverage(readSensor());
    //   logDistance(distance);
    //   updateLEDs(distance);
  }
  // lightManager.gradientAcrossBars(CRGB::Black, CRGB::BlueViolet);
  // FastLED.show();
  // delay(100);
}