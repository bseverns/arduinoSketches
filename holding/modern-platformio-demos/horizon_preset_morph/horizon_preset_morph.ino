// HORIZON preset morph showcase — Teensy 4.x + Audio Shield
// This sketch cycles between contrasting "bus" presets and lets the
// built-in smoothing show off its slow-motion morphing. One scene is a
// cinema-wide wash with dynamic side gain; the other is a gentle
// mastering chain meant to keep things solid but not sleepy.
//
// Plug in stereo program material, watch Serial prints for preset
// switches, and listen for the width breathing between scenes. No
// external UI required — just headphones/speakers and patience.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Horizon.h"

AudioInputI2S        i2sIn;
AudioHorizon         horizon;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, horizon, 0);
AudioConnection      patchCord2(i2sIn, 1, horizon, 1);
AudioConnection      patchCord3(horizon, 0, i2sOut, 0);
AudioConnection      patchCord4(horizon, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

struct HorizonPreset {
  const char* name;
  float width;
  float dynWidth;
  float transientSens;
  float midTiltDbPerOct;
  float airFreqHz;
  float airGainDb;
  float lowAnchorHz;
  float dirt;
  float ceilingDb;
  float mix;
  bool morphWidth;
};

static const HorizonPreset kPresets[] = {
  {
    "Cinema Morph",
    1.25f,
    0.65f,
    0.45f,
    -0.25f,
    12000.0f,
    3.0f,
    90.0f,
    0.18f,
    -1.5f,
    0.9f,
    true
  },
  {
    "Bus Pillow",
    0.55f,
    0.25f,
    0.35f,
    0.5f,
    9500.0f,
    1.5f,
    120.0f,
    0.12f,
    -2.5f,
    0.75f,
    false
  }
};

static constexpr int kNumPresets = sizeof(kPresets) / sizeof(kPresets[0]);
static int sCurrentPreset = 0;
static elapsedMillis sPresetTimer;
static elapsedMillis sMorphClock;

static void applyPreset(const HorizonPreset& preset) {
  horizon.setWidth(preset.width);
  horizon.setDynWidth(preset.dynWidth);
  horizon.setTransientSens(preset.transientSens);
  horizon.setMidTilt(preset.midTiltDbPerOct);
  horizon.setSideAir(preset.airFreqHz, preset.airGainDb);
  horizon.setLowAnchor(preset.lowAnchorHz);
  horizon.setDirt(preset.dirt);
  horizon.setCeiling(preset.ceilingDb);
  horizon.setMix(preset.mix);

  Serial.println();
  Serial.print("Loaded preset: ");
  Serial.println(preset.name);
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  AudioMemory(56);

  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.55f);

  applyPreset(kPresets[sCurrentPreset]);
}

void loop() {
  if (sPresetTimer > 10000) {
    sCurrentPreset = (sCurrentPreset + 1) % kNumPresets;
    applyPreset(kPresets[sCurrentPreset]);
    sPresetTimer = 0;
  }

  const HorizonPreset& preset = kPresets[sCurrentPreset];
  if (preset.morphWidth) {
    float seconds = 0.001f * static_cast<float>(sMorphClock);
    float lfo = 0.5f + 0.5f * sinf(seconds * 0.7f);
    float widened = preset.width * (0.85f + 0.3f * lfo);
    float dyn = preset.dynWidth * (0.8f + 0.4f * lfo);
    horizon.setWidth(widened);
    horizon.setDynWidth(dyn);
  } else {
    horizon.setWidth(preset.width);
    horizon.setDynWidth(preset.dynWidth);
  }
}
