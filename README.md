> **⚠️ EDUCATIONAL USE ONLY — AUTHORIZED TESTING ONLY.**
> This project exists for education, research, and **defense of systems you own
> or hold explicit written authorization to assess**. Unauthorized use is
> prohibited and may be illegal. Read [ETHICS.md](ETHICS.md) and
> [SCOPE.md](SCOPE.md) before use. Use at your own risk; **AS IS**, no warranty.

# H11 — GPS Spoofing Simulator

![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)
![GitHub Stars](https://img.shields.io/github/stars/5h4d0wn1k/h11-gps-spoof)
![Last Commit](https://img.shields.io/github/last-commit/5h4d0wn1k/h11-gps-spoof)
![GitHub Issues](https://img.shields.io/github/issues/5h4d0wn1k/h11-gps-spoof)

> **GPS spoofing simulator for hardware security research** — craft and replay
> NMEA-0183 GPGGA/GPRMC/GPVTG/GPGSA sentences on an ESP32 to study GNSS
> reliability, in isolated bench experiments only.

## Why

GNSS is a single point of failure in countless systems — from drones and fleet
tracking to grid synchronisation — and spoofed navigation data is a documented
hardware-security threat. H11 explores that problem at the byte level: it
generates valid NMEA-0183 sentences (GPGGA, GPRMC, GPVTG, GPGSA) with correct
checksums across several spoof location profiles, driven by serial commands from
an ESP32 at 1 Hz. It is a **bench tool only**: NMEA output stays on lab wiring
into a GPS module you own. There is no over-the-air GNSS emission, no third-party
receiver is ever addressed, and the Python host helper runs fully offline so the
sentence-crafting logic is testable anywhere.

## Features

- **NMEA-0183 sentence crafting** — GPGGA, GPRMC, GPVTG, GPGSA with correct
  checksums.
- **Spoof location profiles** — airport, city center, custom, and more.
- **Serial control** — set/cycle profiles and update position at 1 Hz.
- **Offline host helper** — `python3 host/h11_cli.py --demo` validates sentence
  generation (exit 0, no radio).
- **Fixtures** — bundled NMEA fixtures for deterministic tests.

## Quickstart

```bash
# Offline analysis (no radio required)
python3 host/h11_cli.py --demo
python3 -m unittest discover -s tests
```

## ESP32 firmware

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 firmware/h11_gps_spoof
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 firmware/h11_gps_spoof
```

Typical wiring: ESP32 TX → GPS module RX (e.g. GPIO17). All demo IPs and
coordinates in the repo are public landmarks or documentation placeholders.

## Project structure

```
firmware/h11_gps_spoof/   # ESP32 Arduino firmware
host/                     # Python host helpers (h11_cli.py, hw_common.py)
fixtures/                 # offline and bench fixtures
tests/                    # offline unit tests
```

## Documentation

- [ETHICS.md](ETHICS.md) — ethical-use policy, read first
- [SCOPE.md](SCOPE.md) — authorized-scope definition
- [CONTRIBUTING.md](CONTRIBUTING.md) — how to contribute
- [SECURITY.md](SECURITY.md) — vulnerability reporting

## Contributing

New sentence types, checksum tests, and lab-fixture builders are welcome. See
[CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT — see [LICENSE](LICENSE).