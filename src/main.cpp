#include "PatternRunner.h"
#include "SectionManager.h"
#include "TouchTrigger.h"
#include <Adafruit_DotStar.h>
#include <Arduino.h>
#include <FastLED.h>

#define LED_STRIP 0
#define LED_COUNT 42
#define TOUCH_PIN A0
#define TOUCH_THRESHOLD 800
#define LOG_TO_SERIAL_MONITOR true

uint8_t lightLevel;
CRGB leds[LED_COUNT];
uint8_t hue = 253;
uint8_t saturation = 255;

Adafruit_DotStar dot = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);
SectionManager sectionManager = SectionManager(leds);
PatternRunner patternRunner = PatternRunner(&sectionManager);
TouchTrigger touchTrigger = TouchTrigger(TOUCH_PIN);

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
  while (!Serial)
  {
    ;
  }
  Serial.println("setup");
  // delay(2000);

  dot.begin();
  dot.clear();
  dot.show();

  setupSectionManager();
  setupLedStrip();
  touchTrigger.setTouchThreshold(TOUCH_THRESHOLD);
  touchTrigger.begin();
}

unsigned long frameInterval = 10;
unsigned long lastFrame = 0;
int lastLoggedDistance = 0;

void renderFrame()
{

  if (touchTrigger.touched())
  {
    Serial.println("Switching pattern");
    patternRunner.cycleIdlePattern();
  }
  else
  {
    patternRunner.runCurrentIdlePattern();
  }
}

void loop()
{
  unsigned long now = millis();
  touchTrigger.tick(now);

  if (now - lastFrame > frameInterval)
  {
    lastFrame = now;
    renderFrame();
  }
}
