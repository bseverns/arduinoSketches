// HORIZON minimal wiring — Teensy 4.x + Audio Shield
// Requires: Teensy Audio Library

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

void setup() {
  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  horizon.setWidth(0.6f);
  horizon.setDynWidth(0.35f);
}

void loop() {
  // In a full build, poll encoders/buttons here and call horizon setters.
}
