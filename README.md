# String7Segment

Drive 7-segment displays made from addressable LEDs (WS2812/NeoPixel).

## What's This?

A library for custom 7-segment displays built from 8 individually addressable LEDs. Each digit uses 8 LEDs wired in segment order (A, B, C, D, E, F, G, DP).

**Key feature:** Works alongside your existing LED arrays. Your display can be at the start, middle, or end of a larger strip - we only touch our 8 LEDs and leave the rest alone.

## Hardware

Each digit is 8 WS2812 LEDs (2020 size works great) positioned as a 7-segment display:

```
    AAAA
   F    B
   F    B
    GGGG
   E    C
   E    C
    DDDD  DP
```

Wire them in order: A (index 0), B, C, D, E, F, G, DP (index 7).

## Usage

```cpp
#include <FastLED.h>
#include "String7Segment.h"

CRGB leds[100];  // Your LED array

// Display starts at index 20 in the array
String7Segment display(leds, 20);

void setup() {
  FastLED.addLeds<WS2812, 5, GRB>(leds, 100);

  display.setForeground(RGB::Red());
  display.setBackground(RGB::Black());
}

void loop() {
  display.showDigit(7);
  FastLED.show();
}
```

## Background Modes

- `BG_OVERWRITE` - Unlit segments set to background color (default)
- `BG_PRESERVE` - Don't touch unlit segments (for animations behind the display)

## API

```cpp
// Display
display.showDigit(7);           // Single digit 0-9
display.showChar('A');          // Character (0-9, A-F, H, L, P, etc.)
display.showSegments(0b01101101); // Raw segment mask

// Colors
display.setForeground(RGB::Red());
display.setBackground(RGB::Black());
display.setBackgroundMode(BG_PRESERVE);

// Utility
display.clear();
display.setDecimalPoint(0, true);
```

## Compatible With

- FastLED
- Adafruit NeoPixel
- Any RGB struct with r, g, b members

## License

MIT
