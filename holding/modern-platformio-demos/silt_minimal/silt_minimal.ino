// SILT minimal wiring — Teensy 4.x + Audio Shield
// Requires: Teensy Audio Library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Silt.h"

AudioInputI2S        i2sIn;
AudioSilt            silt;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, silt, 0);
AudioConnection      patchCord2(i2sIn, 1, silt, 1);
AudioConnection      patchCord3(silt, 0, i2sOut, 0);
AudioConnection      patchCord4(silt, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

void setup() {
  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  silt.setMix(0.6f);
  silt.setMode(2); // both
}

void loop() {
  // Hook encoders/buttons here in a full build
}
