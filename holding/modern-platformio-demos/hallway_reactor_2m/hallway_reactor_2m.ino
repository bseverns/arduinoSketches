#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L1X.h>
#include <FastLED.h>
#include "config.h"

// ---------- LED setup ----------
CRGB leds[NUM_LEDS];

// ---------- Sensors ----------
Adafruit_VL53L1X tofE = Adafruit_VL53L1X();
Adafruit_VL53L1X tofM = Adafruit_VL53L1X();
Adafruit_VL53L1X tofW = Adafruit_VL53L1X();

// ---------- State ----------

enum State { IDLE, APPROACH_EW, APPROACH_WE, MID_ENTRY, DWELL };

inline bool isQuiet() {
  pinMode(QUIET_SWITCH_PIN, INPUT_PULLUP);
  int v = digitalRead(QUIET_SWITCH_PIN);
  bool activeLow = (QUIET_ACTIVE_LOW != 0);
  return activeLow ? (v == LOW) : (v == HIGH);
}

State state = IDLE;

uint32_t lastPresenceMs = 0;
uint32_t lastEdgeE = 0, lastEdgeM = 0, lastEdgeW = 0;

void setAddress(Adafruit_VL53L1X &s, uint8_t xshutPin, uint8_t newAddr) {
  pinMode(xshutPin, OUTPUT);
  digitalWrite(xshutPin, LOW);
  delay(10);
  digitalWrite(xshutPin, HIGH);
  delay(10);
  if (!s.begin(0x29)) {
    while (1) {
      delay(10);
    }
  }
  s.setAddress(newAddr);
  s.setDistanceMode(VL53L1X_DISTANCEMODE_LONG);
  s.setROISize(8, 8);
  s.setROICenter(199);
  s.startRanging();
}

inline uint16_t readDistance(Adafruit_VL53L1X &s) {
  if (!s.dataReady()) return 8191;
  int16_t d = s.distance();
  s.clearInterrupt();
  if (d < 0) return 8191;
  return (uint16_t)d;
}

inline bool nearE(uint16_t mm) { return (mm < NEAR_EW_MM) && (mm > 40); }
inline bool presentE(uint16_t mm) { return (mm < PRES_EW_MM) && (mm > 40); }
inline bool nearM(uint16_t mm) { return (mm < NEAR_MID_MM) && (mm > 40); }

void pattern_idle() {
  static uint8_t h = 0;
  h++;
  fill_solid(leds, NUM_LEDS, CHSV(h, 30, 4));
  FastLED.show();
}

void pattern_comet(bool leftToRight) {
  static uint16_t pos = 0;
  static uint8_t hue = 160;
  fadeToBlackBy(leds, NUM_LEDS, 40);
  uint16_t i = leftToRight ? pos : (NUM_LEDS - 1 - pos);
  leds[i] = CHSV(hue, 200, 200);
  for (int t = 1; t <= 6; ++t) {
    int j = leftToRight ? (int)pos - t : (int)(NUM_LEDS - 1 - pos) + t;
    if (j >= 0 && j < NUM_LEDS) leds[j] += CHSV(hue, 180, 100 - t * 12);
  }
  FastLED.show();
  pos = (pos + 1) % NUM_LEDS;
}

void pattern_mid_bloom() {
  fadeToBlackBy(leds, NUM_LEDS, 30);
  int c = NUM_LEDS / 2;
  for (int r = 0; r < NUM_LEDS / 2; ++r) {
    uint8_t v = qsub8(200, r * 6);
    if (c + r < NUM_LEDS) leds[c + r] += CHSV(32, 200, v);
    if (c - r >= 0) leds[c - r] += CHSV(32, 200, v);
  }
  FastLED.show();
}

void pattern_dwell() {
  static uint8_t t = 0;
  t++;
  uint8_t v = beatsin8(8, 6, 24);
  fill_solid(leds, NUM_LEDS, CHSV(100 + (t >> 3), 120, v));
  FastLED.show();
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS_NORMAL);
  FastLED.clear(true);

  Wire.begin();
  pinMode(QUIET_SWITCH_PIN, INPUT_PULLUP);

  pinMode(XSHUT_E, OUTPUT);
  pinMode(XSHUT_M, OUTPUT);
  pinMode(XSHUT_W, OUTPUT);
  digitalWrite(XSHUT_E, LOW);
  digitalWrite(XSHUT_M, LOW);
  digitalWrite(XSHUT_W, LOW);
  delay(50);

  setAddress(tofE, XSHUT_E, 0x31);
  setAddress(tofM, XSHUT_M, 0x33);
  setAddress(tofW, XSHUT_W, 0x35);
}

void loop() {
  uint32_t now = millis();

  uint16_t dE = readDistance(tofE);
  uint16_t dM = readDistance(tofM);
  uint16_t dW = readDistance(tofW);

  bool eNear = nearE(dE), wNear = nearE(dW), mNear = nearM(dM);
  bool ePres = presentE(dE), wPres = presentE(dW);
  bool anyPres = ePres || wPres || mNear;

  if (eNear && (now - lastEdgeE) > DEBOUNCE_MS) lastEdgeE = now;
  if (mNear && (now - lastEdgeM) > DEBOUNCE_MS) lastEdgeM = now;
  if (wNear && (now - lastEdgeW) > DEBOUNCE_MS) lastEdgeW = now;
  if (anyPres) lastPresenceMs = now;

  if ((now - lastEdgeE) < DIR_WINDOW_MS && (now - lastEdgeW) > DIR_WINDOW_MS) {
    state = APPROACH_EW;
  } else if ((now - lastEdgeW) < DIR_WINDOW_MS && (now - lastEdgeE) > DIR_WINDOW_MS) {
    state = APPROACH_WE;
  } else if ((now - lastEdgeM) < DIR_WINDOW_MS && !ePres && !wPres) {
    state = MID_ENTRY;
  } else if (!anyPres && (now - lastPresenceMs) > DWELL_MS) {
    state = IDLE;
  } else if (anyPres) {
    state = DWELL;
  }

  switch (state) {
    case APPROACH_EW: pattern_comet(true); break;
    case APPROACH_WE: pattern_comet(false); break;
    case MID_ENTRY: pattern_mid_bloom(); break;
    case DWELL: pattern_dwell(); break;
    default: pattern_idle(); break;
  }

  delay(18);
}
