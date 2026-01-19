# Build Configuration

## Arduino IDE Location

```
/Applications/Arduino IDE.app
```

Bundled CLI:
```
/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli
```

## ESP32-S3 with USB CDC

For ESP32-S3 boards using USB CDC for serial (no external UART chip), you need these compile flags:

```
--fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc
```

This enables:
- `USBMode=hwcdc` - Hardware CDC and JTAG
- `CDCOnBoot=cdc` - CDC enabled on boot (so Serial works immediately)

## Building with Arduino CLI

```bash
# Set up alias for bundled CLI
ARDUINO_CLI="/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli"

# Install ESP32 platform (if not already)
$ARDUINO_CLI core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
$ARDUINO_CLI core install esp32:esp32

# Install FastLED
$ARDUINO_CLI lib install "FastLED"

# Compile basic example
$ARDUINO_CLI compile \
  --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc \
  examples/basic/basic.ino

# Upload (replace /dev/cu.usbmodem* with your port)
$ARDUINO_CLI upload \
  --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc \
  --port /dev/cu.usbmodem* \
  examples/basic/basic.ino
```

## Quick Build Script

```bash
#!/bin/bash
# build.sh - Compile and optionally upload

ARDUINO_CLI="/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli"
FQBN="esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc"
SKETCH="${1:-examples/basic/basic.ino}"

# Find port
PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -1)

echo "Compiling $SKETCH..."
$ARDUINO_CLI compile --fqbn $FQBN --library src $SKETCH

if [ "$2" == "upload" ] && [ -n "$PORT" ]; then
  echo "Uploading to $PORT..."
  $ARDUINO_CLI upload --fqbn $FQBN --port $PORT $SKETCH
fi
```

## Arduino IDE Settings

If using the GUI:

1. **Board:** ESP32S3 Dev Module
2. **USB Mode:** Hardware CDC and JTAG
3. **USB CDC On Boot:** Enabled
4. **Upload Speed:** 921600
5. **Partition Scheme:** Default 4MB with spiffs

## Library Dependencies

- **FastLED** (for examples) - install via Library Manager or `arduino-cli lib install "FastLED"`

The core library (`String7Segment`) has no dependencies.
