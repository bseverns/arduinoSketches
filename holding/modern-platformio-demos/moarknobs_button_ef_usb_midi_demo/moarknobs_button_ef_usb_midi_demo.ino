/*
 * Button + Envelope demo sketch
 * ------------------------------
 * Archived from MOARkNOBS-42 and trimmed into a standalone Teensy sketch.
 *
 * Wiring assumptions:
 *   - Control button on pin 12, active LOW
 *   - Envelope follower output patched into A0
 */

#include <Arduino.h>
#include <math.h>
#include <usb_midi.h>

namespace {
constexpr uint8_t kButtonPin = 12;
constexpr uint8_t kEnvelopePin = A0;
constexpr uint8_t kMidiChannel = 1;
constexpr uint8_t kNote = 60;
constexpr uint8_t kEnvelopeCC = 21;
constexpr uint32_t kCcIntervalMs = 8;
constexpr float kBaselineLerp = 0.0025f;
constexpr float kEnvelopeLerp = 0.18f;

float baseline = 0.0f;
float envelope = 0.0f;
uint8_t lastCcValue = 0xFF;
bool noteActive = false;
uint32_t lastCcStamp = 0;

uint16_t readRawEnvelope() { return static_cast<uint16_t>(analogRead(kEnvelopePin)); }

void calibrateBaseline() {
  uint32_t total = 0;
  constexpr uint16_t samples = 128;
  for (uint16_t i = 0; i < samples; ++i) {
    total += readRawEnvelope();
    delayMicroseconds(50);
  }
  baseline = static_cast<float>(total) / samples;
  envelope = 0.0f;
}

uint8_t levelToMidi(float level) {
  level = constrain(level, 0.0f, 1023.0f);
  return static_cast<uint8_t>(roundf((level / 1023.0f) * 127.0f));
}

void pumpEnvelope() {
  float raw = static_cast<float>(readRawEnvelope());
  baseline = (1.0f - kBaselineLerp) * baseline + kBaselineLerp * raw;
  float delta = raw - baseline;
  if (delta < 0.0f) {
    delta = 0.0f;
  }
  envelope = (1.0f - kEnvelopeLerp) * envelope + kEnvelopeLerp * delta;
}

void maybeSendEnvelope() {
  uint32_t now = millis();
  if (now - lastCcStamp < kCcIntervalMs) {
    return;
  }
  lastCcStamp = now;
  uint8_t value = levelToMidi(envelope);
  if (value == lastCcValue) {
    return;
  }
  lastCcValue = value;
  usbMIDI.sendControlChange(kEnvelopeCC, value, kMidiChannel);
}

void serviceButton() {
  bool pressed = digitalRead(kButtonPin) == LOW;
  if (pressed && !noteActive) {
    usbMIDI.sendNoteOn(kNote, 100, kMidiChannel);
    noteActive = true;
  } else if (!pressed && noteActive) {
    usbMIDI.sendNoteOff(kNote, 0, kMidiChannel);
    noteActive = false;
  }
}

void drainUsbMidi() {
  while (usbMIDI.read()) {
    // This sketch only emits MIDI.
  }
}
}  // namespace

void setup() {
  pinMode(kButtonPin, INPUT_PULLUP);
  pinMode(kEnvelopePin, INPUT);

  Serial.begin(115200);
  while (!Serial && millis() < 2000) {
    // Give USB a moment to enumerate on laptop-powered rigs.
  }

  Serial.println("MOARkNOBS: 1-button/1-envelope USB MIDI demo");
  Serial.println("Hold things quiet for a sec while baseline calibration runs...");
  calibrateBaseline();
  Serial.println("Baseline locked. Mash the button and feed the envelope follower.");

  usbMIDI.begin();
}

void loop() {
  pumpEnvelope();
  maybeSendEnvelope();
  serviceButton();
  drainUsbMidi();
}
