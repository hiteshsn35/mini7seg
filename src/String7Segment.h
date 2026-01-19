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

// Simple RGB struct for color-agnostic operation
// Compatible with CRGB, Adafruit NeoPixel, or raw arrays
struct RGB {
  uint8_t r, g, b;

  RGB() : r(0), g(0), b(0) {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

  // Common colors
  static RGB Black()   { return RGB(0, 0, 0); }
  static RGB White()   { return RGB(255, 255, 255); }
  static RGB Red()     { return RGB(255, 0, 0); }
  static RGB Green()   { return RGB(0, 255, 0); }
  static RGB Blue()    { return RGB(0, 0, 255); }
  static RGB Yellow()  { return RGB(255, 255, 0); }
  static RGB Cyan()    { return RGB(0, 255, 255); }
  static RGB Magenta() { return RGB(255, 0, 255); }
  static RGB Orange()  { return RGB(255, 128, 0); }
};

// Segment patterns for characters
// Index 0-9 = digits, 10-15 = A-F, 16+ = special chars
extern const uint8_t SEGMENT_PATTERNS[];

class String7Segment {
public:
  // Constructor for single digit
  // ledArray: pointer to your LED array (CRGB*, RGB*, or compatible)
  // offset: where our 8 LEDs start in the array
  template<typename T>
  String7Segment(T* ledArray, uint16_t offset)
    : _ledPtr((uint8_t*)ledArray)
    , _offset(offset)
    , _numDigits(1)
    , _pixelSize(sizeof(T))
    , _foreground(RGB::Red())
    , _background(RGB::Black())
    , _bgMode(BG_OVERWRITE)
  {}

  // Constructor for multiple digits
  template<typename T>
  String7Segment(T* ledArray, uint16_t offset, uint8_t numDigits)
    : _ledPtr((uint8_t*)ledArray)
    , _offset(offset)
    , _numDigits(numDigits)
    , _pixelSize(sizeof(T))
    , _foreground(RGB::Red())
    , _background(RGB::Black())
    , _bgMode(BG_OVERWRITE)
  {}

  // Color settings
  void setForeground(RGB color) { _foreground = color; }
  void setForeground(uint8_t r, uint8_t g, uint8_t b) { _foreground = RGB(r, g, b); }
  void setBackground(RGB color) { _background = color; }
  void setBackground(uint8_t r, uint8_t g, uint8_t b) { _background = RGB(r, g, b); }
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

  // Get segment pattern for a character
  static uint8_t getPattern(char c);

private:
  uint8_t* _ledPtr;       // Pointer to LED array (as bytes)
  uint16_t _offset;       // Starting index in LED array
  uint8_t  _numDigits;    // Number of digits
  uint8_t  _pixelSize;    // Size of each pixel struct (3 for RGB, 4 for RGBW)
  RGB      _foreground;   // Lit segment color
  RGB      _background;   // Unlit segment color
  BackgroundMode _bgMode; // How to handle unlit segments

  // Write color to a specific LED
  void setPixel(uint16_t index, RGB color);
  RGB  getPixel(uint16_t index);
};

#endif // STRING_7_SEGMENT_H
