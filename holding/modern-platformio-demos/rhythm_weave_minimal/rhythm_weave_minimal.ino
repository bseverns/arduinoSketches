// RHYTHM WEAVE minimal — event sequencer skeleton
// Compiles on Teensy 4.x with or without Audio library. Sends USB MIDI note-on triggers if available.

#include <Arduino.h>
#include "RhythmWeave.h"

RhythmWeave weave;

void onTrigger(uint8_t lane, uint8_t velocity) {
  Serial.printf("Lane %u trig vel %u\n", lane, velocity);
#ifdef USB_MIDI
  usbMIDI.sendNoteOn(36 + lane, velocity, 1);
  usbMIDI.sendNoteOff(36 + lane, 0, 1);
#endif
}

void setup() {
  Serial.begin(115200);
  weave.begin(120.0f);
  for (uint8_t i = 0; i < 4; i++) {
    LaneParams p;
    p.steps = 8 + i * 2;
    p.fills = 3 + i;
    p.prob = 0.9f;
    p.ratchet = 1;
    p.offset = 0;
    p.accent = 0.8f - 0.1f * i;
    weave.setLane(i, p);
  }
  weave.setTriggerCallback(onTrigger);
}

void loop() {
  weave.poll();
#ifdef USB_MIDI
  while (usbMIDI.read()) {}
#endif
}
