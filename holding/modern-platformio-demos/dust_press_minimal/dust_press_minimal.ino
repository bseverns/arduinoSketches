// DUST PRESS minimal wiring — Teensy 4.x + Audio Shield
// Requires: Teensy Audio Library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "DustPress.h"

AudioInputI2S        i2sIn;
AudioDustPress       dustpress;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, dustpress, 0);
AudioConnection      patchCord2(i2sIn, 1, dustpress, 1);
AudioConnection      patchCord3(dustpress, 0, i2sOut, 0);
AudioConnection      patchCord4(dustpress, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

void setup() {
  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  dustpress.setDriveDb(12.0f);
  dustpress.setMix(0.5f);
}

void loop() {
  // Poll UI here in a full build.
}
