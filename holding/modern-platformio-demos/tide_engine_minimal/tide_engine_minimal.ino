// TIDE ENGINE minimal wiring — Teensy 4.x + Audio Shield
// This is a skeleton for testing audio graph wiring.
// Requires: Teensy Audio Library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

void setup() {
  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  tide.begin(delayL, delayR, kDelaySamples, AUDIO_SAMPLE_RATE_EXACT);
  tide.setTempo(120.0f);
  tide.setMix(0.4f);
  tide.setXfeed(0.2f, 0.2f);
  HeadParams heads[4];
  heads[0].num = 1; heads[0].den = 1; heads[0].fb = 0.35f; heads[0].pan = -0.5f;
  heads[1].num = 1; heads[1].den = 2; heads[1].fb = 0.35f; heads[1].pan = 0.5f;
  heads[2].num = 1; heads[2].den = 3; heads[2].fb = 0.30f; heads[2].pan = -0.25f;
  heads[3].num = 2; heads[3].den = 3; heads[3].fb = 0.30f; heads[3].pan = 0.25f;
  for (uint8_t i = 0; i < 4; ++i) {
    tide.setHead(i, heads[i]);
  }
}

void loop() {
  // In a full build, poll encoders/buttons here and call tide setters.
}
