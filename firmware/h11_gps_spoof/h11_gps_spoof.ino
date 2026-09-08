// H11 — GPS Spoofer (ESP32)
// NMEA sentence crafting, GPS signal replay, location spoofing
// Uses TX pin to emit NMEA sentences to connected GPS receiver

#include <Arduino.h>

// ── Configuration ──────────────────────────────────────────────
#define GPS_TX_PIN        17   // GPIO17 → GPS module RX (via Serial2)
#define GPS_RX_PIN        16   // GPIO16 → GPS module TX (via Serial2)
#define NMEA_BAUD         9600
#define SERIAL_BAUD       115200
#define SENTENCE_INTERVAL  1000

// Predefined spoof locations
struct SpoofProfile {
  const char* name;
  double lat;
  char   latDir;
  double lon;
  char   lonDir;
  float  alt;
  float  spd;
  float  crs;
};

SpoofProfile profiles[] = {
  { "Airport Gate",    40.6413, 'N', -73.7781, 'W', 13.0, 0.0, 0.0 },
  { "City Center",     51.5074, 'N',  -0.1278, 'W', 11.0, 0.0, 0.0 },
  { "Military Base",   38.8977, 'N', -77.0365, 'W', 20.0, 5.0, 180.0 },
  { "Border Crossing", 32.5556, 'N',  -82.9100, 'W', 80.0, 30.0, 45.0 },
  { "Custom",          37.7749, 'N',-122.4194, 'W', 16.0, 0.0, 0.0 },
};
const int profileCount = sizeof(profiles) / sizeof(profiles[0]);
int currentProfile = 0;



// ── Forward declarations ──────────────────────────────────────
void sendGPGGA(SpoofProfile& p, uint32_t fixTime);
void sendGPRMC(SpoofProfile& p, uint32_t fixTime);
void sendGPVTG(SpoofProfile& p);
void sendGPGSA(SpoofProfile& p);
String formatLat(double lat, char dir);
String formatLon(double lon, char dir);
String formatTime(uint32_t ts);
uint32_t getUnixTime();

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);
  Serial.println(F("\n=== H11 — GPS Spoofer ==="));
  Serial.println(F("WARNING: For authorized testing only.\n"));

  Serial2.begin(NMEA_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.printf("NMEA output on GPIO%d @ %d baud\n", GPS_TX_PIN, NMEA_BAUD);

  Serial.println(F("\nSpoof profiles:"));
  for (int i = 0; i < profileCount; i++) {
    Serial.printf("  [%d] %s — %.4f %c, %.4f %c\n",
      i, profiles[i].name, profiles[i].lat, profiles[i].latDir,
      profiles[i].lon, profiles[i].lonDir);
  }

  Serial.println(F("\nCommands: 0-4=select profile  n=next  c=custom lat/lon\n"));
}

// ── Main loop ─────────────────────────────────────────────────
void loop() {
  static uint32_t lastSent = 0;

  // Handle serial commands
  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '0' && c < '0' + profileCount) {
      currentProfile = c - '0';
      Serial.printf("Profile: %s\n", profiles[currentProfile].name);
    } else if (c == 'n' || c == 'N') {
      currentProfile = (currentProfile + 1) % profileCount;
      Serial.printf("Profile: %s\n", profiles[currentProfile].name);
    }
  }

  // Send NMEA sentences at 1 Hz
  if (millis() - lastSent >= SENTENCE_INTERVAL) {
    lastSent = millis();
    uint32_t fixTime = getUnixTime();
    SpoofProfile& p = profiles[currentProfile];

    sendGPGGA(p, fixTime);
    delay(20);
    sendGPRMC(p, fixTime);
    delay(20);
    sendGPVTG(p);
    delay(20);
    sendGPGSA(p);

    Serial.printf("[SPF] %s: %.4f %c, %.4f %c, alt=%.1f m\n",
      p.name, p.lat, p.latDir, p.lon, p.lonDir, p.alt);
  }
}

// ── GPGGA — Global Positioning System Fix Data ────────────────
void sendGPGGA(SpoofProfile& p, uint32_t fixTime) {
  String timeStr = formatTime(fixTime);
  String latStr  = formatLat(p.lat, p.latDir);
  String lonStr  = formatLon(p.lon, p.lonDir);

  // Satellite count (faked — 8 satellites, HDOP=0.9)
  String body = "GPGGA," + timeStr + "," + latStr + "," + lonStr
    + ",1,08,0.9," + String(p.alt, 1) + ",M,0.0,M,,*";

  uint8_t cs = 0;
  const char* s = body.c_str();
  while (*s && *s != '*') { cs ^= *s; s++; }

  String sentence = "$" + body + (cs < 16 ? "0" : "") + String(cs, HEX);
  Serial2.println(sentence);
  Serial.println(sentence);
}

// ── GPRMC — Recommended Minimum Navigation Information ───────
void sendGPRMC(SpoofProfile& p, uint32_t fixTime) {
  String timeStr = formatTime(fixTime);
  String latStr  = formatLat(p.lat, p.latDir);
  String lonStr  = formatLon(p.lon, p.lonDir);

  String spdKnots = String(p.spd * 1.94384, 2);
  String crsStr   = String(p.crs, 1);

  String body = "GPRMC," + timeStr + ",A," + latStr + "," + lonStr
    + "," + spdKnots + "," + crsStr + ",*";

  uint8_t cs = 0;
  const char* s = body.c_str();
  while (*s && *s != '*') { cs ^= *s; s++; }

  String sentence = "$" + body + (cs < 16 ? "0" : "") + String(cs, HEX);
  Serial2.println(sentence);
  Serial.println(sentence);
}

// ── GPVTG — Track Made Good and Ground Speed ──────────────────
void sendGPVTG(SpoofProfile& p) {
  String spdKmH = String(p.spd * 3.6, 2);
  String spdKnot = String(p.spd * 1.94384, 2);
  String crs = String(p.crs, 1);

  String body = "GPVTG," + crs + ",T," + crs + ",M,"
    + spdKnot + ",N," + spdKmH + ",K*";

  uint8_t cs = 0;
  const char* s = body.c_str();
  while (*s && *s != '*') { cs ^= *s; s++; }

  String sentence = "$" + body + (cs < 16 ? "0" : "") + String(cs, HEX);
  Serial2.println(sentence);
  Serial.println(sentence);
}

// ── GPGSA — GNSS DOP and Active Satellites ───────────────────
void sendGPGSA(SpoofProfile& p) {
  String body = "GPGSA,A,3,01,02,03,04,05,06,07,08,,,,,"
    + String(9) + "," + String(p.alt * 3.28084, 1) + "," + String(p.alt * 3.28084 * 0.3, 1) + "*";

  uint8_t cs = 0;
  const char* s = body.c_str();
  while (*s && *s != '*') { cs ^= *s; s++; }

  String sentence = "$" + body + (cs < 16 ? "0" : "") + String(cs, HEX);
  Serial2.println(sentence);
  Serial.println(sentence);
}

// ── Helpers ───────────────────────────────────────────────────
String formatLat(double lat, char dir) {
  int deg = (int)lat;
  double min = (lat - deg) * 60.0;
  char buf[16];
  snprintf(buf, sizeof(buf), "%02d%07.4f,%c", deg, min, dir);
  return String(buf);
}

String formatLon(double lon, char dir) {
  int deg = (int)lon;
  double min = (lon - deg) * 60.0;
  char buf[16];
  snprintf(buf, sizeof(buf), "%03d%07.4f,%c", deg, min, dir);
  return String(buf);
}

String formatTime(uint32_t ts) {
  uint32_t h = (ts / 3600) % 24;
  uint32_t m = (ts / 60) % 60;
  uint32_t s = ts % 60;
  char buf[10];
  snprintf(buf, sizeof(buf), "%02d%02d%02d.00", h, m, s);
  return String(buf);
}

uint32_t getUnixTime() {
  static uint32_t fakeTime = 43200;
  return fakeTime++;
}
