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

## IMPORTANT: Read before use.

This project is provided for **educational and authorized security testing purposes only**.

### Authorization Requirements
- You MUST have explicit written permission from the network owner before using this tool
- Unauthorized interception of network communications is illegal under federal and state laws
- This tool should ONLY be used on networks you own or have written authorization to test

### Spectrum Regulatory Notes (GNSS / bench wiring)
- GNSS bands (L1 ≈ 1575.42 MHz, L2, L5) are protected, licensed
  spectrum: knowingly feeding false navigation data or interfering
  with them is unlawful in virtually all jurisdictions (e.g.,
  47 U.S.C. § 301 and FCC GPS-jammer enforcement).
- This bench tool only drives the serial input of a GPS module **you
  own**, inside an isolated lab — never an over-the-air GNSS emission.
- NMEA output must be confined to lab wiring; no external antennas.

### No Third-Party Disruption
Live spoofing of any third party's receiver, vehicle, aircraft, or
network is out of scope. Proofs here are NMEA sentences/fixtures and
offline simulation only.

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

## Live Lab Test Plan

Run ONLY on an isolated, authorized own-lab bench against devices, networks,
and spectrum **you own**. No third-party callers, bystanders, or spectrum users
may be within range of any test transmission.

1. **Isolate** - Put the DUT in a shielded/Faraday enclosure or a room with no
   third-party devices in range. Use attenuators on any transmit path.
2. **Own devices only** - Every target (AP, remote, tag, GPS module, drone FC,
   receiver) must be your own hardware.
3. **Lowest power, shortest duration** - Start at minimum TX power / duty cycle
   and use only the seconds needed.
4. **Record** - Save before/after logs to `reports/` (git-ignored). Never
   capture or store third-party traffic.
5. **Cleanup** - Restore placeholder SSIDs (`lab-*`), MACs (`00:11:22:33:44:55`),
   example.com / RFC5737 addresses, and clear any captured data from the device.

> Jammer / spoofer / replay projects are **proofs for study and simulation**
> only. They refuse live interference scenarios: a live bench trigger requires
> the `LAB_*` allowlist environment variable AND explicit `--yes` confirmation,
> and even then only against your own hardware in a shielded bench.

## Metrics

| Metric | Target | Where |
|---|---|---|
| Firmware compile | `arduino-cli compile --fqbn esp32:esp32:esp32 firmware/h11_gps_spoof` PASS | CI/local |
| Host helper | `python3 host/h11_cli.py --demo` exits 0 (offline) | host/ |
| Unit tests | `python3 -m unittest discover -s tests` passes | tests/ |
| py_compile | every `host/*.py` compiles clean | CI/local |

## License

MIT
