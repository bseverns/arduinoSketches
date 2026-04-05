// TIDE ENGINE teaching rig -- Teensy 4.x + Audio Shield
// Purpose: three presets that explain the engine surface and sonic intent.
// Press the button on kNextPresetPin to step through the teaching bank at runtime.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "../../core/TidePreset.h"
#include "../../teensy/TideEngine.h"

AudioInputI2S        i2sIn;
AudioTideEngine      tide;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, tide, 0);
AudioConnection      patchCord2(i2sIn, 1, tide, 1);
AudioConnection      patchCord3(tide, 0, i2sOut, 0);
AudioConnection      patchCord4(tide, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

constexpr uint32_t kDelaySamples = 1u << 19; // ~11.9s at 44.1kHz
EXTMEM int16_t delayL[kDelaySamples];
EXTMEM int16_t delayR[kDelaySamples];

namespace {
constexpr uint8_t kNextPresetPin = 3;
constexpr uint32_t kDebounceMs = 30;
uint8_t gPresetIndex = 0;
}

void applyCurrentPreset();

void setup() {
  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  tide.begin(delayL, delayR, kDelaySamples, AUDIO_SAMPLE_RATE_EXACT);
  pinMode(kNextPresetPin, INPUT_PULLUP);
  applyCurrentPreset();
}

void loop() {
  static bool wasDown = false;
  static uint32_t lastEdgeMs = 0;

  uint32_t now = millis();
  bool isDown = (digitalRead(kNextPresetPin) == LOW);
  bool risingEdge = isDown && !wasDown && (now - lastEdgeMs) >= kDebounceMs;
  if (risingEdge) {
    gPresetIndex = static_cast<uint8_t>((gPresetIndex + 1u) % TidePresetBank::kTeachingRigPresetCount);
    applyCurrentPreset();
    lastEdgeMs = now;
  }
  wasDown = isDown;
}

void applyCurrentPreset() {
  tide.applyPreset(TidePresetBank::kTeachingRigPresets[gPresetIndex]);
}
