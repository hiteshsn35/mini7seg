/*
 * String7Segment Basic Example
 *
 * Simple demonstration of displaying digits on a WS2812-based
 * 7-segment display using FastLED.
 *
 * Hardware:
 * - ESP32 (or ESP32-S3)
 * - 8 WS2812-2020 (SMD2020 package) LEDs wired as 7-segment (A,B,C,D,E,F,G,DP)
 * - Data pin connected to GPIO 13
 */

#include <FastLED.h>
#include "String7Segment.h"

#define DATA_PIN    13
#define NUM_LEDS    8     // Single digit = 8 LEDs
#define BRIGHTNESS  50

// FastLED array
CRGB leds[NUM_LEDS];

// Our 7-segment display (starts at index 0 in the array)
String7Segment display(leds, 0);

void setup() {
  Serial.begin(115200);
  Serial.println("String7Segment Basic Example");

  // Initialize FastLED
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Configure display colors
  display.setForeground(S7Color::Red());
  display.setBackground(S7Color::Black());
  display.setBackgroundMode(BG_OVERWRITE);
}

void loop() {
  // Count 0-9
  for (int i = 0; i < 10; i++) {
    display.showDigit(i);
    FastLED.show();
    delay(500);
  }

  // Show hex A-F
  for (char c = 'A'; c <= 'F'; c++) {
    display.showChar(c);
    FastLED.show();
    delay(500);
  }

  // Show extended letters: H J L n o P r U Y
  const char* letters = "HJLnoPrUY";
  for (int i = 0; letters[i]; i++) {
    display.showChar(letters[i]);
    FastLED.show();
    delay(500);
  }

  // Show special characters: hyphen, underscore, space
  display.showChar('-');
  FastLED.show();
  delay(700);

  display.showChar('_');
  FastLED.show();
  delay(700);

  display.showChar(' ');  // blank
  FastLED.show();
  delay(700);

  // Blink decimal point
  display.showDigit(8);  // All segments on
  FastLED.show();
  delay(300);

  for (int i = 0; i < 5; i++) {
    display.setDecimalPoint(0, true);
    FastLED.show();
    delay(200);
    display.setDecimalPoint(0, false);
    FastLED.show();
    delay(200);
  }

  // Clear and pause
  display.clear();
  FastLED.show();
  delay(1000);
}
