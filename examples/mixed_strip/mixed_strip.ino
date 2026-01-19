/*
 * String7Segment Mixed Strip Example
 *
 * Demonstrates using a 7-segment display in the MIDDLE of a
 * larger LED strip. This is the key use case - playing nicely
 * with other people's LED arrays.
 *
 * Setup:
 * - 100 LED strip total
 * - LEDs 0-39: Rainbow animation
 * - LEDs 40-47: Our 7-segment display (8 LEDs)
 * - LEDs 48-99: Another animation
 *
 * The library only touches its 8 LEDs and leaves the rest alone.
 */

#include <FastLED.h>
#include "String7Segment.h"

#define DATA_PIN    5
#define NUM_LEDS    100
#define BRIGHTNESS  50

// FastLED array - the whole strip
CRGB leds[NUM_LEDS];

// Our 7-segment display starts at index 40
#define DISPLAY_OFFSET 40
String7Segment display(leds, DISPLAY_OFFSET);

// Counter for the display
int counter = 0;
unsigned long lastCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("String7Segment Mixed Strip Example");

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Display in green, preserve background
  // (so it doesn't mess with the animations around it)
  display.setForeground(RGB::Green());
  display.setBackgroundMode(BG_OVERWRITE);
  display.setBackground(RGB::Black());  // Dark segments on our display
}

void loop() {
  // =========================================
  // Animation for LEDs 0-39 (before display)
  // =========================================
  static uint8_t hue1 = 0;
  for (int i = 0; i < 40; i++) {
    leds[i] = CHSV(hue1 + (i * 4), 255, 150);
  }
  hue1++;

  // =========================================
  // Animation for LEDs 48-99 (after display)
  // =========================================
  static uint8_t hue2 = 128;
  for (int i = 48; i < 100; i++) {
    leds[i] = CHSV(hue2 + (i * 2), 200, 100);
  }
  hue2--;

  // =========================================
  // Our 7-segment display (LEDs 40-47)
  // =========================================
  if (millis() - lastCount > 1000) {
    counter++;
    if (counter > 9) counter = 0;
    display.showDigit(counter);
    lastCount = millis();
  }

  // =========================================
  // Show everything at once
  // =========================================
  FastLED.show();
  delay(20);
}
