/*
 * String7Segment - Addressable LED 7-Segment Display Library
 *
 * Drive 7-segment displays made from WS2812/NeoPixel LEDs.
 * Designed to work alongside existing LED arrays - we modify
 * only our segments and let you call show() when ready.
 *
 * Hardware: 8 LEDs per digit in segment order A,B,C,D,E,F,G,DP
 *
 * License: MIT
 */

#ifndef STRING_7_SEGMENT_H
#define STRING_7_SEGMENT_H

#include <stdint.h>

// Segment bit positions (matches physical LED order)
#define SEG_A   0x01  // bit 0
#define SEG_B   0x02  // bit 1
#define SEG_C   0x04  // bit 2
#define SEG_D   0x08  // bit 3
#define SEG_E   0x10  // bit 4
#define SEG_F   0x20  // bit 5
#define SEG_G   0x40  // bit 6
#define SEG_DP  0x80  // bit 7

// Background modes
enum BackgroundMode {
  BG_OVERWRITE,   // Unlit segments set to background color
  BG_PRESERVE,    // Unlit segments left unchanged
  BG_BLEND        // Blend lit segments with existing colors
};

// Simple color struct (named to avoid collision with FastLED's S7Color)
struct S7Color {
  uint8_t r, g, b;

  S7Color() : r(0), g(0), b(0) {}
  S7Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

  // Common colors
  static S7Color Black()   { return S7Color(0, 0, 0); }
  static S7Color White()   { return S7Color(255, 255, 255); }
  static S7Color Red()     { return S7Color(255, 0, 0); }
  static S7Color Green()   { return S7Color(0, 255, 0); }
  static S7Color Blue()    { return S7Color(0, 0, 255); }
  static S7Color Yellow()  { return S7Color(255, 255, 0); }
  static S7Color Cyan()    { return S7Color(0, 255, 255); }
  static S7Color Magenta() { return S7Color(255, 0, 255); }
  static S7Color Orange()  { return S7Color(255, 128, 0); }
};

// Segment patterns for characters
// Index 0-9 = digits, 10-15 = A-F, 16+ = special chars
extern const uint8_t SEGMENT_PATTERNS[];

class String7Segment {
public:
  // Constructor for single digit
  // ledArray: pointer to your LED array (CRGB*, S7Color*, or compatible)
  // offset: where our LEDs start in the array
  template<typename T>
  String7Segment(T* ledArray, uint16_t offset)
    : _ledPtr((uint8_t*)ledArray)
    , _offset(offset)
    , _numDigits(1)
    , _ledsPerSegment(1)
    , _pixelSize(sizeof(T))
    , _foreground(S7Color::Red())
    , _background(S7Color::Black())
    , _bgMode(BG_OVERWRITE)
  {}

  // Constructor for multiple digits
  template<typename T>
  String7Segment(T* ledArray, uint16_t offset, uint8_t numDigits)
    : _ledPtr((uint8_t*)ledArray)
    , _offset(offset)
    , _numDigits(numDigits)
    , _ledsPerSegment(1)
    , _pixelSize(sizeof(T))
    , _foreground(S7Color::Red())
    , _background(S7Color::Black())
    , _bgMode(BG_OVERWRITE)
  {}

  // Constructor with custom LEDs per segment (for larger displays)
  // Use 2 or 3 for displays with multiple LEDs per segment
  template<typename T>
  String7Segment(T* ledArray, uint16_t offset, uint8_t numDigits, uint8_t ledsPerSegment)
    : _ledPtr((uint8_t*)ledArray)
    , _offset(offset)
    , _numDigits(numDigits)
    , _ledsPerSegment(ledsPerSegment)
    , _pixelSize(sizeof(T))
    , _foreground(S7Color::Red())
    , _background(S7Color::Black())
    , _bgMode(BG_OVERWRITE)
  {}

  // Color settings
  void setForeground(S7Color color) { _foreground = color; }
  void setForeground(uint8_t r, uint8_t g, uint8_t b) { _foreground = S7Color(r, g, b); }
  void setBackground(S7Color color) { _background = color; }
  void setBackground(uint8_t r, uint8_t g, uint8_t b) { _background = S7Color(r, g, b); }
  void setBackgroundMode(BackgroundMode mode) { _bgMode = mode; }

  // Display functions
  void showDigit(uint8_t digit, uint8_t position = 0, bool showDP = false);
  void showChar(char c, uint8_t position = 0, bool showDP = false);
  void showSegments(uint8_t segmentMask, uint8_t position = 0);
  void showNumber(int32_t number, bool leadingZeros = false);
  void showHex(uint32_t value, uint8_t digits = 0);

  // Utility
  void clear();
  void clearDigit(uint8_t position = 0);
  void setDecimalPoint(uint8_t position, bool on = true);

  // Animations
  void spinStep(uint8_t step, uint8_t position = 0);  // step 0-5 for outer ring
  uint8_t getSpinSegment(uint8_t step);               // Get segment mask for spin step

  // Get segment pattern for a character
  static uint8_t getPattern(char c);

  // Configuration
  void setLedsPerSegment(uint8_t count) { _ledsPerSegment = count; }
  uint8_t getLedsPerSegment() const { return _ledsPerSegment; }
  uint8_t getLedsPerDigit() const { return 8 * _ledsPerSegment; }

private:
  uint8_t* _ledPtr;         // Pointer to LED array (as bytes)
  uint16_t _offset;         // Starting index in LED array
  uint8_t  _numDigits;      // Number of digits
  uint8_t  _ledsPerSegment; // LEDs per segment (1, 2, or 3 typical)
  uint8_t  _pixelSize;      // Size of each pixel struct (3 for RGB, 4 for RGBW)
  S7Color  _foreground;     // Lit segment color
  S7Color  _background;     // Unlit segment color
  BackgroundMode _bgMode;   // How to handle unlit segments

  // Write color to a specific LED
  void setPixel(uint16_t index, S7Color color);
  S7Color  getPixel(uint16_t index);
};

#endif // STRING_7_SEGMENT_H
