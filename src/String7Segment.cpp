/*
 * String7Segment - Implementation
 */

#include "String7Segment.h"

// Segment patterns for displayable characters
// Bit order: DP G F E D C B A
const uint8_t SEGMENT_PATTERNS[] = {
  // 0-9
  0b00111111,  // 0: A B C D E F
  0b00000110,  // 1: B C
  0b01011011,  // 2: A B D E G
  0b01001111,  // 3: A B C D G
  0b01100110,  // 4: B C F G
  0b01101101,  // 5: A C D F G
  0b01111101,  // 6: A C D E F G
  0b00000111,  // 7: A B C
  0b01111111,  // 8: A B C D E F G
  0b01101111,  // 9: A B C D F G

  // A-F (hex)
  0b01110111,  // A: A B C E F G
  0b01111100,  // b: C D E F G (lowercase looks better)
  0b00111001,  // C: A D E F
  0b01011110,  // d: B C D E G (lowercase looks better)
  0b01111001,  // E: A D E F G
  0b01110001,  // F: A E F G

  // Extended characters (index 16+)
  0b01110110,  // H: B C E F G (index 16)
  0b00011110,  // J: B C D E (index 17)
  0b00111000,  // L: D E F (index 18)
  0b01010100,  // n: C E G (lowercase) (index 19)
  0b01011100,  // o: C D E G (lowercase) (index 20)
  0b01110011,  // P: A B E F G (index 21)
  0b01010000,  // r: E G (lowercase) (index 22)
  0b00111110,  // U: B C D E F (index 23)
  0b01101110,  // Y: B C D F G (index 24)
  0b01000000,  // - (minus): G (index 25)
  0b00001000,  // _ (underscore): D (index 26)
  0b00000000,  // (space): nothing (index 27)
};

// Get pattern for a character
uint8_t String7Segment::getPattern(char c) {
  if (c >= '0' && c <= '9') {
    return SEGMENT_PATTERNS[c - '0'];
  }
  if (c >= 'A' && c <= 'F') {
    return SEGMENT_PATTERNS[10 + (c - 'A')];
  }
  if (c >= 'a' && c <= 'f') {
    return SEGMENT_PATTERNS[10 + (c - 'a')];
  }

  // Extended characters
  switch (c) {
    case 'H': case 'h': return SEGMENT_PATTERNS[16];
    case 'J': case 'j': return SEGMENT_PATTERNS[17];
    case 'L': case 'l': return SEGMENT_PATTERNS[18];
    case 'N': case 'n': return SEGMENT_PATTERNS[19];
    case 'O': case 'o': return SEGMENT_PATTERNS[20];
    case 'P': case 'p': return SEGMENT_PATTERNS[21];
    case 'R': case 'r': return SEGMENT_PATTERNS[22];
    case 'U': case 'u': return SEGMENT_PATTERNS[23];
    case 'Y': case 'y': return SEGMENT_PATTERNS[24];
    case '-':           return SEGMENT_PATTERNS[25];
    case '_':           return SEGMENT_PATTERNS[26];
    case ' ':           return SEGMENT_PATTERNS[27];
    default:            return 0;  // Unknown = blank
  }
}

// Write color to a specific LED in the array
void String7Segment::setPixel(uint16_t index, RGB color) {
  uint8_t* pixel = _ledPtr + (index * _pixelSize);
  // Assuming RGB order (most common for CRGB)
  pixel[0] = color.r;
  pixel[1] = color.g;
  pixel[2] = color.b;
}

// Read color from a specific LED in the array
RGB String7Segment::getPixel(uint16_t index) {
  uint8_t* pixel = _ledPtr + (index * _pixelSize);
  return RGB(pixel[0], pixel[1], pixel[2]);
}

// Display a single digit (0-9)
void String7Segment::showDigit(uint8_t digit, uint8_t position, bool showDP) {
  if (digit > 9) digit = 9;
  uint8_t pattern = SEGMENT_PATTERNS[digit];
  if (showDP) pattern |= SEG_DP;
  showSegments(pattern, position);
}

// Display a character
void String7Segment::showChar(char c, uint8_t position, bool showDP) {
  uint8_t pattern = getPattern(c);
  if (showDP) pattern |= SEG_DP;
  showSegments(pattern, position);
}

// Display raw segment pattern
void String7Segment::showSegments(uint8_t segmentMask, uint8_t position) {
  if (position >= _numDigits) return;

  // Calculate LED index for this digit
  uint16_t baseIndex = _offset + (position * 8);

  // Process each segment (8 LEDs per digit)
  for (uint8_t seg = 0; seg < 8; seg++) {
    uint16_t ledIndex = baseIndex + seg;
    bool segmentOn = (segmentMask >> seg) & 0x01;

    if (segmentOn) {
      // Lit segment - always set to foreground
      setPixel(ledIndex, _foreground);
    } else {
      // Unlit segment - depends on background mode
      switch (_bgMode) {
        case BG_OVERWRITE:
          setPixel(ledIndex, _background);
          break;
        case BG_PRESERVE:
          // Don't touch it
          break;
        case BG_BLEND:
          // For now, just preserve (blend logic can be added later)
          break;
      }
    }
  }
}

// Display a number across multiple digits
void String7Segment::showNumber(int32_t number, bool leadingZeros) {
  bool negative = (number < 0);
  if (negative) number = -number;

  // Extract digits right-to-left
  uint8_t digits[10];
  uint8_t digitCount = 0;

  if (number == 0) {
    digits[0] = 0;
    digitCount = 1;
  } else {
    while (number > 0 && digitCount < 10) {
      digits[digitCount++] = number % 10;
      number /= 10;
    }
  }

  // Display from right to left
  for (uint8_t pos = 0; pos < _numDigits; pos++) {
    uint8_t displayPos = _numDigits - 1 - pos;  // Rightmost first

    if (pos < digitCount) {
      showDigit(digits[pos], displayPos);
    } else if (leadingZeros) {
      showDigit(0, displayPos);
    } else if (negative && pos == digitCount) {
      showChar('-', displayPos);
    } else {
      clearDigit(displayPos);
    }
  }
}

// Display hexadecimal value
void String7Segment::showHex(uint32_t value, uint8_t digits) {
  if (digits == 0) digits = _numDigits;
  if (digits > _numDigits) digits = _numDigits;

  for (uint8_t pos = 0; pos < digits; pos++) {
    uint8_t displayPos = _numDigits - 1 - pos;
    uint8_t nibble = (value >> (pos * 4)) & 0x0F;

    if (nibble < 10) {
      showDigit(nibble, displayPos);
    } else {
      showChar('A' + (nibble - 10), displayPos);
    }
  }

  // Clear remaining positions
  for (uint8_t pos = digits; pos < _numDigits; pos++) {
    clearDigit(_numDigits - 1 - pos);
  }
}

// Clear all digits
void String7Segment::clear() {
  for (uint8_t pos = 0; pos < _numDigits; pos++) {
    clearDigit(pos);
  }
}

// Clear a single digit
void String7Segment::clearDigit(uint8_t position) {
  if (position >= _numDigits) return;

  uint16_t baseIndex = _offset + (position * 8);
  for (uint8_t seg = 0; seg < 8; seg++) {
    if (_bgMode == BG_PRESERVE) {
      // Don't touch
    } else {
      setPixel(baseIndex + seg, _background);
    }
  }
}

// Set decimal point on/off
void String7Segment::setDecimalPoint(uint8_t position, bool on) {
  if (position >= _numDigits) return;

  uint16_t dpIndex = _offset + (position * 8) + 7;  // DP is segment 7
  if (on) {
    setPixel(dpIndex, _foreground);
  } else if (_bgMode != BG_PRESERVE) {
    setPixel(dpIndex, _background);
  }
}
