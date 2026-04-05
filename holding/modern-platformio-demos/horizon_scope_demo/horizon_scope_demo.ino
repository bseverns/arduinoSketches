// horizon_scope.ino
// Simple "scope" sketch for AudioHorizon.
// Routes stereo input through Horizon and prints per-block telemetry
// to the Serial monitor so you can *see* width, transients and limiting.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "Horizon.h"

AudioInputI2S            audioInput;
AudioHorizon             horizon;
AudioOutputI2S           audioOutput;

AudioConnection          patchCord1(audioInput, 0, horizon, 0);
AudioConnection          patchCord2(audioInput, 1, horizon, 1);
AudioConnection          patchCord3(horizon, 0, audioOutput, 0);
AudioConnection          patchCord4(horizon, 1, audioOutput, 1);

AudioControlSGTL5000     sgtl5000_1;

elapsedMillis            printTimer;

static void printBar(const char* label, float value) {
  if (value < 0.0f) value = 0.0f;
  if (value > 1.0f) value = 1.0f;

  const int width = 32;
  int filled = (int)(value * width + 0.5f);
  if (filled < 0) filled = 0;
  if (filled > width) filled = width;

  Serial.print(label);
  Serial.print(" |");
  for (int i = 0; i < width; ++i) {
    Serial.print(i < filled ? '#' : ' ');
  }
  Serial.print("| ");
  Serial.println(value, 3);
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  AudioMemory(40);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5f);

  horizon.setWidth(0.6f);
  horizon.setDynWidth(0.4f);
  horizon.setTransientSens(0.5f);
  horizon.setMidTilt(0.0f);
  horizon.setSideAir(10000.0f, 2.0f);
  horizon.setLowAnchor(100.0f);
  horizon.setDirt(0.15f);
  horizon.setCeiling(-1.0f);
  horizon.setMix(0.7f);
}

void loop() {
  if (printTimer > 100) {
    float width = horizon.getBlockWidth();
    float trans = horizon.getBlockTransient();
    float gain = horizon.getLimiterGain();

    Serial.println();
    printBar("Width    ", width);
    printBar("Transient", trans);

    float limBar = gain;
    if (limBar < 0.0f) limBar = 0.0f;
    if (limBar > 1.0f) limBar = 1.0f;
    printBar("Limiter  ", limBar);

    printTimer = 0;
  }
}
