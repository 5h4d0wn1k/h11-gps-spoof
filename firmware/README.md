# GPS Spoofing Firmware

## Purpose

Study NMEA sentence crafting and validation offline. SIMULATION ONLY: any live GPS-module feed requires explicit lab authorization.

## Board

- **Board**: ESP32 + serial GPS module (intercept line)
- **FQBN**: `esp32:esp32:esp32`
- **Sketch**: `h11_gps_spoof/h11_gps_spoof.ino`

## Wiring

```
Serial2 TX -> GPS module RX (GPIO17), Serial2 RX <- GPS module TX (GPIO16), GND common, 3V3/5V as the module requires.
```

## Build

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 firmware/h11_gps_spoof
# upload (example, ESP32-C6):
# arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 firmware/h11_gps_spoof
```

## Runtime

See the root README "IMPORTANT" section before powering on. This firmware is
for authorized own-lab study. Serial console exposes the interactive command
set described in the root README. All identifiers in the sketch are
placeholders (`lab-*` SSIDs, `00:11:22:33:44:55`, RFC 5737 / example.com).
