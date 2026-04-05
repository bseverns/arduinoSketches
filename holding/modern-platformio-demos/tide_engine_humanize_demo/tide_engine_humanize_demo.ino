// TIDE ENGINE humanize demo -- Teensy 4.x + Audio Shield
// Demonstrates per-head timing jitter for a more organic polyrhythm.

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

  tide.setTempo(98.0f);
  tide.setMix(0.45f);
  tide.setTone(8000.0f);
  tide.setXfeed(0.2f, 0.2f);
  tide.setWow(2.0f, 0.3f);
  tide.setFlutter(0.8f, 6.0f);

  HeadParams heads[4];

  heads[0].num = 1; heads[0].den = 1;
  heads[0].fb = 0.32f;
  heads[0].pan = -0.5f;
  heads[0].humanize_ms = 0.0f;

  heads[1].num = 1; heads[1].den = 3;
  heads[1].fb = 0.30f;
  heads[1].pan = 0.5f;
  heads[1].humanize_ms = 8.0f;

  heads[2].mute = true;
  heads[3].mute = true;

  for (uint8_t i = 0; i < 4; ++i) {
    tide.setHead(i, heads[i]);
  }
}

void loop() {
  // In a full build, map a knob to heads[1].humanize_ms and update at control rate.
}
