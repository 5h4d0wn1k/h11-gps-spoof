# H11 — GPS Spoofer

NMEA sentence crafting and GPS signal replay for ESP32.

## Overview

GPS spoofing tool that crafts and transmits NMEA-0183 sentences to simulate GPS positions:
- Generates GPGGA, GPRMC, GPVTG, and GPGSA sentences
- Multiple pre-defined spoof location profiles
- NMEA checksums computed correctly
- Configurable via serial commands
- Real-time position updates at 1 Hz

## Hardware

| Component | Connection | Role |
|-----------|------------|------|
| ESP32 DevKit | Main board | NMEA generation |
| GPS module RX | GPIO17 (TX) | Receives spoofed NMEA data |

## Spoof Profiles

| # | Name | Coordinates | Alt |
|---|------|-------------|-----|
| 0 | Airport Gate | 40.6413°N 73.7781°W | 13m |
| 1 | City Center | 51.5074°N 0.1278°W | 11m |
| 2 | Military Base | 38.8977°N 77.0365°W | 20m |
| 3 | Border Crossing | 32.5556°N 82.9100°W | 80m |
| 4 | Custom | 37.7749°N 122.4194°W | 16m |

## Serial Output

```
[SPF] Airport Gate: 40.6413 N, 73.7781 W, alt=13.0 m
$GPGGA,120000.00,4038.4780,N,07346.6860,W,1,08,0.9,13.0,M,0.0,M,,*7A
$GPRMC,120000.00,A,4038.4780,N,07346.6860,W,0.00,0.00,040926,,,A*XX
```

## Build & Flash

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 firmware/
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 firmware/
```

## Legal Disclaimer

**IMPORTANT: Read before use.**

This project is provided for **educational and authorized security testing purposes only**.

### Authorization Requirements
- You MUST have explicit written permission from the network owner before using this tool
- Unauthorized interception of network communications is illegal under federal and state laws
- This tool should ONLY be used on networks you own or have written authorization to test

### Legal Framework
- **Computer Fraud and Abuse Act (CFAA)**: Unauthorized access to computer systems is a federal crime
- **Wiretap Act (18 U.S.C. § 2511)**: Interception of electronic communications without consent is illegal
- **State Laws**: Many states have additional computer crime and wiretapping statutes
- **GDPR/CCPA**: Data collection may be subject to privacy regulations

### Acceptable Use
- Testing security of your own networks
- Authorized penetration testing with written scope
- Academic research in controlled lab environments
- Security education and training

### Prohibited Use
- Intercepting communications on networks you don't own
- Attacking infrastructure without authorization
- Any activity that violates applicable laws or regulations
- Commercial use without proper licensing

### No Warranty
This software is provided "AS IS" without warranty of any kind. The author is not responsible for any misuse or damage caused by this software.

### Responsible Disclosure
If you discover vulnerabilities using this tool, follow responsible disclosure practices:
1. Report to the vendor/owner privately
2. Allow reasonable time for remediation
3. Do not exploit beyond proof of concept

## License

MIT
