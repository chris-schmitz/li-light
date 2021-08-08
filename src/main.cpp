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

void randomSections()
{
  for (int i = 0; i < 20; i++)
  {
    lightManager.lightRandomSections(1);
    FastLED.show();
    delay(100);
  }
}

void barcodeSwipe()
{
  for (int i = 0; i < 6; i++) // * six being the number of bars in the barcode
  {
    Serial.print(i - 1);
    Serial.print(", ");
    // TODO: fix
    // ! this is a dumb way of doing it. refactor
    // if (i < 6)
    lightManager.setSectionColor(i, CRGB(0x495057));
    // if (i < 6)
    lightManager.setSectionColor(i - 1, CRGB::Black);
    // if (i < 6)
    //   lightManager.setSectionColor(i - 2, CRGB(0xFFFFFF));
    // if (i < 6)
    //   lightManager.setSectionColor(i - 3, CRGB(0x6c757d));
    // if (i < 6)
    //   lightManager.setSectionColor(i - 4, CRGB(0x495057));
    FastLED.show();
    delay(100);
  }
  for (int i = 0; i < 6; i++) // * six being the number of bars in the barcode
  {
    lightManager.setSectionColor(i, CRGB::Black);
  }
  FastLED.show();

  Serial.println("");
  // delay(500);
}

uint32_t colors[] = {0xf8f9fa, 0xe9ecef, 0xdee2e6, 0xced4da, 0xadb5bd, 0x6c757d, 0x495057, 0x343a40, 0x212529, 0x000000};

void gradualBarcodeScan()
{
  // * the state we want to push across the bars
  // * this is a colleciton of colors indexes to use for each bar
  // * each byte corresponds to a bar
  byte frameState[] = {9, 9, 9, 9, 9, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9};

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
    delay(10);
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

unsigned long colorChangeInterval = 500;
unsigned long colorChangeLastChecked = 0;

bool flip = false;
void loop()
{

  if (false)
  {
    Serial.print("size of int: ");
    Serial.println(sizeof(int));
    Serial.print("size of pixel range: ");
    Serial.println(sizeof(PixelRange));
    Serial.print("size of pixel range for bars: ");
    Serial.println(6 * sizeof(PixelRange));
    lightManager.printSections();
  }

  // unsigned long now = millis();
  // if (now - colorChangeLastChecked > colorChangeInterval)
  // {
  //   colorChangeLastChecked = now;

  //   int distance = updateMovingAverage(readSensor());
  //   logDistance(distance);
  //   //   updateLEDs(distance);
  // }

  // lightManager.gradientAcrossBars(0, CRGB(0x8ecae6), 6, CRGB(0xffb703));
  // lightManager.setSectionColor(6, CRGB(0xffb703));
  // lightManager.setSectionColor(7, CRGB(0xffb703));
  // lightManager.setSectionColor(8, CRGB(0xffb703));
  // lightManager.gradientAcrossBars(7, CRGB::BlueViolet, 8, CRGB::Black);
  // FastLED.show();

  // randomSections();
  // delay(100);
  // barcodeSwipe();
  lightManager.clear();
  delay(1000);
  gradualBarcodeScan();
  // raiseGraphBars();
  fadeInBars();
  // fillSectionBySection();
  delay(3000);
}

void flipColors()
{
  if (flip == true)
  {
    lightManager.setSectionColor(0, 255, 0, 255);
    lightManager.setSectionColor(1, 0, 255, 255);
    lightManager.setSectionColor(2, 255, 0, 255);
    lightManager.setSectionColor(3, 0, 255, 255);
    lightManager.setSectionColor(4, 255, 0, 255);
    lightManager.setSectionColor(5, 0, 255, 255);

    lightManager.setSectionColor(6, 255, 0, 255);
    lightManager.setSectionColor(7, 0, 255, 255);
    lightManager.setSectionColor(8, 255, 0, 255);
    flip = false;
  }
  else
  {
    lightManager.setSectionColor(0, 0, 255, 255);
    lightManager.setSectionColor(1, 255, 0, 255);
    lightManager.setSectionColor(2, 0, 255, 255);
    lightManager.setSectionColor(3, 255, 0, 255);
    lightManager.setSectionColor(4, 0, 255, 255);
    lightManager.setSectionColor(5, 255, 0, 255);

    lightManager.setSectionColor(6, 0, 255, 255);
    lightManager.setSectionColor(7, 255, 0, 255);
    lightManager.setSectionColor(8, 0, 255, 255);
    flip = true;
  }
  FastLED.show();
}