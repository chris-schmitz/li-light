#include "PatternRunner.h"
#include "TriggerTracker.h"
#include "UltrasonicManager.h"
#include <Arduino.h>
#include <SectionManager.h>

#include <FastLED.h>

#define ECHO_PIN 1
#define TRIGGER_PIN 2

#define LED_STRIP 3
#define LED_COUNT 42

#define LOG_TO_SERIAL_MONITOR true

uint8_t lightLevel;
CRGB leds[LED_COUNT];
uint8_t hue = 253;
uint8_t saturation = 255;

UltrasonicManager ultrasonicManager = UltrasonicManager(TRIGGER_PIN, ECHO_PIN);
SectionManager sectionManager = SectionManager(leds);
PatternRunner patternRunner = PatternRunner(&sectionManager);
TriggerTracker triggerTracker = TriggerTracker(
    PatternTriggerLimits(IDLE_PATTERN, 10, 2),
    PatternTriggerLimits(ACTIVATION_PATTERN, 40, 5));

void setupLedStrip()
{
  FastLED.addLeds<NEOPIXEL, LED_STRIP>(leds, LED_COUNT);
}

void setupSectionManager()
{
  sectionManager.addSections(9);

  sectionManager.addRangeToSection(0, 0, 3, true);
  sectionManager.addRangeToSection(1, 4, 7, false);
  sectionManager.addRangeToSection(2, 8, 11, true);
  sectionManager.addRangeToSection(3, 12, 15);
  sectionManager.addRangeToSection(4, 16, 19, true);
  sectionManager.addRangeToSection(5, 20, 23);

  sectionManager.addRangeToSection(6, 24, 27);
  sectionManager.addRangeToSection(6, 28, 31, true);

  sectionManager.addRangeToSection(7, 32, 33);
  sectionManager.addRangeToSection(7, 34, 35, true);

  sectionManager.addRangeToSection(8, 36, 38);
  sectionManager.addRangeToSection(8, 39, 41, true);
}

void setup()
{
  Serial.begin(9600);

  setupSectionManager();
  ultrasonicManager.begin();

  setupLedStrip();
}

unsigned long frameInterval = 10;
unsigned long lastFrame = 0;
int lastLoggedDistance = 0;

void renderFrame()
{
  lastLoggedDistance = ultrasonicManager.getAveragedDistance();

  if (LOG_TO_SERIAL_MONITOR)
  {
    ultrasonicManager.logDistance(lastLoggedDistance); // ! test method
  }

  TriggerEnum currentTrigger = triggerTracker.getTriggerByDistance(lastLoggedDistance);

  switch (currentTrigger)
  {
  case ACTIVATION_PATTERN:
    Serial.println("activation pattern");
    break;
  case SWITCH_IDLE:
    patternRunner.cycleIdlePattern();
    break;
  default:
    patternRunner.runCurrentIdlePattern();
    break;
  }
}

void loop()
{
  unsigned long now = millis();

  if (now - lastFrame > frameInterval)
  {
    lastFrame = now;
    renderFrame();
  }
}
