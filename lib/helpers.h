#pragma once
#include <FastLED.h>

// // | Adapted from adafruit's neopixel library.
// // * https://github.com/adafruit/Adafruit_NeoPixel
// // ^ It's pretty much a one for one lift, just returning a CRGB struct instead of an
// // ^ unsigned 32 bit. Normally I don't like this type of borrowing, but I'm letting it slide
// // ^ because I'm taking the oppertunity to analyse and really understand how the logic works
// CRGB Wheel(byte WheelPosition)
// {
//   WheelPosition = 255 - WheelPosition; // flip the number

//   // ^ Note that in the adafruit wheel function, the colors are moving along
//   // ^ the color wheel 3 bits at a time. Two of the three color
//   // ^ values (r,g,b) are moving 3 bits at a time. To be able to jump from
//   // ^ 0 to 255 in steps of 3 it means our wheel value range of 0 to 255 needs
//   // ^ to be divided in thirds,
//   // * 1/3: 255 / 3 = 85
//   // * 2/3: 255 / 3 * 2 = 170
//   // * 2/3: 255 / 3 * 3 = 255 // no need for an equation, i.e. the default
//   if (WheelPosition < 85)
//   {
//     // * note here we're bringing Red down steps of 3 and taking Blue up steps of 3
//     // ? why are we doing this? I'm asuming this is some color wheel logic??
//     return CRGB(255 - WheelPosition * 3, 0, WheelPosition * 3);
//   }

//   if (WheelPosition < 170)
//   {
//     // * note that while our wheel position is a range between 0 and 255, we still
//     // * want our color hop by 3 to start at 0 and end at 255, so if our value is over
//     // * 85 (needs to be to get here) we know we can subtract 85 to get back to a starting point
//     // * of 0 and we'll be hopping up by 3 till we get to 170, our second third of 255.
//     WheelPosition -= 85;
//     return CRGB(0, WheelPosition * 3, 255 - WheelPosition * 3);
//   }

//   // * now we're to the third third of 255, so we know we need to chop down the number by two thirds
//   // * to get back to our 0. Now the upper 255 of our step up by 3 is controlled by the WheelPosition
//   // * data type, i.e. the max value of `byte` is 255.
//   WheelPosition -= 170;
//   return CRGB(WheelPosition * 3, 255 - WheelPosition * 3, 0);
// }

// ! IMPORTANT NOTE!
// * This Wheel class and rgbToUint32 is an adaptation of Adafruit's functions in their
// * strand_test example:
// ? https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest_wheel/strandtest_wheel.ino
// * I adjusted it here to be more generic -> fastled friendly, but it functions the same

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t Wheel(uint8_t WheelPosition)
{
  WheelPosition = 255 - WheelPosition;

  if (WheelPosition < 85)
  {
    return rgbToUint32(255 - WheelPosition * 3, 0, WheelPosition * 3);
  }

  if (WheelPosition < 170)
  {
    WheelPosition -= 85;
    return rgbToUint32(0, WheelPosition * 3, 255 - WheelPosition * 3);
  }

  WheelPosition -= 170;
  return rgbToUint32(WheelPosition * 3, 255 - WheelPosition * 3, 0);
}