/*
  Example using 2 knobs to change
  FM synthesis parameters, and a knob for fundamental frequency,
  using Mozzi sonification library.

  Demonstrates analog input, audio and control oscillators, phase modulation
  and smoothing a control signal at audio rate to avoid clicks.
  Also demonstrates AutoMap, which maps unpredictable inputs to a set range.

  This example goes with a tutorial on the Mozzi site:
  http://sensorium.github.io/Mozzi/learn/Mozzi_Introductory_Tutorial.pdf

  The circuit:
     Audio output on digital pin 9 (on a Uno or similar), or
     check the README or http://sensorium.github.com/Mozzi/

     Potentiometer connected to analog pin 0.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground

  Potentiometers connected to analog pin 1.
    Center of the pot goes to the analog pin.
    side pins go to +5V and ground.

  Potentiometers connected to analog pin 2.
    Center of the pot goes to the analog pin.
    side pins go to +5V and ground.

  Mozzi help/discussion/announcements:
  https://groups.google.com/forum/#!forum/mozzi-users

  Tim Barrass 2013.
  This example code is in the public domain.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator 
#include <tables/cos2048_int8.h> // table for Oscils to play - something to dig in to/change.
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range

// int freqVal;

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 18;
const int MAX_CARRIER_FREQ = 400;

const int MIN = 1;
const int MAX = 20;

const int MIN_2 = 1;
const int MAX_2 = 25;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 12500;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0, 1023, MIN_INTENSITY, MAX_INTENSITY);
AutoMap kMapModSpeed(0, 1023, MIN_MOD_SPEED, MAX_MOD_SPEED);
AutoMap mapThis(0, 1023, MIN, MAX);
AutoMap mapThisToo(0, 1023, MIN_2, MAX_2);

const int KNOB_PIN = 0; // Carrier Frequency
const int KNOB1_PIN = 1; //  Intensity/Depth
const int KNOB2_PIN = 2; //  modulation speed
const int KNOB3_PIN = 3; // frequency multiplier
const int KNOB4_PIN = 4; // fm intensity

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 12; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup() {
  Serial.begin(115200); // set up the Serial output so we can look at the light level
  startMozzi(); // :))
}


void updateControl() {
  int freqVal = mozziAnalogRead(KNOB3_PIN); // value is 0-1023
  int FRQ = mapThis(freqVal);

  // read the knobs
  int knob_value = mozziAnalogRead(KNOB_PIN); // value is 0-1023
  int knob2 = mozziAnalogRead(KNOB4_PIN); // value is 0-1023
  int knob2Val = mapThis(knob2);

  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value);


  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio * FRQ;

  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);

  // read the light dependent resistor on the width Analog input pin
  int KNOB1_value = mozziAnalogRead(KNOB1_PIN); // value is 0-1023
  int KNOB1_calibrated = kMapIntensity(KNOB1_value);

  // calculate the fm_intensity
  fm_intensity = ((long)KNOB1_calibrated * knob2Val * (kIntensityMod.next() + 128)) >> 8; // shift back to range after 8 bit multiply

  // read the light dependent resistor on the speed Analog input pin
  int KNOB2_value = mozziAnalogRead(KNOB2_PIN); // value is 0-1023
  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(KNOB2_value) / 1000;
  kIntensityMod.setFreq(mod_speed);
}


int updateAudio() {
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop() {
  audioHook();
}
