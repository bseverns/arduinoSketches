/*
  at this point I'm not too sure how these numbers work best - the variable impact and map each other
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator 
#include <tables/triangle2048_int8.h> // table for Oscils to play
#include <Smooth.h> //not so choppy
#include <AutoMap.h> // maps unpredictable inputs to a range
#include <mozzi_utils.h>//light switch/out

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 10;
const int MAX_CARRIER_FREQ = 200;

const int MIN = 1;
const int MAX = 100;

const int MIN_2 = 1;
const int MAX_2 = 100;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 1000;
const int MAX_INTENSITY = 1;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 2500;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0, 1023, MIN_INTENSITY, MAX_INTENSITY);
AutoMap kMapModSpeed(0, 1023, MIN_MOD_SPEED, MAX_MOD_SPEED);
AutoMap mapThis(0, 1023, MIN, MAX);
AutoMap mapThisToo(0, 1023, MIN_2, MAX_2);

const int KNOB_PIN = 0; // carrier freq
const int LDR1_PIN = 1; // set the analog input for fm_intensity to pin 1
const int LDR2_PIN = 2; // set the analog input for mod rate to pin 2
const int LDR3_PIN = 4; //freqVal
const int LDR4_PIN = 3; //knob2

Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aCarrier(TRIANGLE2048_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aModulator(TRIANGLE2048_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(TRIANGLE2048_DATA);

int mod_ratio = 7; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.99f;
Smooth <long> aSmoothIntensity(smoothness);


void setup() {
  Serial.begin(115200); // set up the Serial output so we can look at the light level
  startMozzi(); // :))
  setPin13Out();
}


void updateControl() {
  int freqVal = mozziAnalogRead(LDR3_PIN); // value is 0-1023
  int FRQ = mapThis(freqVal);

  int knob2 = mozziAnalogRead(LDR4_PIN); // value is 0-1023
  int knob2Val = mapThis(knob2);

  int knob_value = mozziAnalogRead(KNOB_PIN); // value is 0-1023

  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value);

  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio * FRQ;

  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);

  // read the light dependent resistor on the width Analog input pin
  int LDR1_value = mozziAnalogRead(LDR1_PIN); // value is 0-1023

  int LDR1_calibrated = kMapIntensity(LDR1_value);

  // calculate the fm_intensity
  fm_intensity = ((long)LDR1_calibrated * knob2Val * (kIntensityMod.next() + 128) * random(1, 1.45)) >> 8; // shift back to range after 8 bit multiply

  // read the light dependent resistor on the speed Analog input pin
  int LDR2_value = mozziAnalogRead(LDR2_PIN); // value is 0-1023

  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(LDR2_value) / 1000;
  kIntensityMod.setFreq(mod_speed);
}


int updateAudio() {
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop() {
  audioHook();
}
