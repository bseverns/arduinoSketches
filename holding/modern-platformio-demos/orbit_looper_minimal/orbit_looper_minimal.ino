// ORBIT LOOPER minimal wiring — Teensy 4.x + Audio Shield
// Requires: Teensy Audio Library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "OrbitLooper.h"

AudioInputI2S        i2sIn;
AudioOrbitLooper     orbit;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, orbit, 0);
AudioConnection      patchCord2(i2sIn, 1, orbit, 1);
AudioConnection      patchCord3(orbit, 0, i2sOut, 0);
AudioConnection      patchCord4(orbit, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

void setup() {
  AudioMemory(64);
  Wire.begin();
  Wire.setClock(400000);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);
  orbit.setLoopSeconds(3.0f);
  orbit.setMix(0.6f);
}

void loop() {
  // Hook UI here in a full build
}
