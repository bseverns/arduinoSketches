/*
  at this point I'm not too sure how these numbers work best - the variable impact and map each other
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator 
#include <tables/triangle2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 20;
const int MAX_CARRIER_FREQ = 200;

const int MIN = 1;
const int MAX = 50;

const int MIN_2 = 1;
const int MAX_2 = 75;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 100;
const int MAX_INTENSITY = 1;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 1500;
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

const int switch1 = 6; //wave shape definition

int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup() {
  Serial.begin(115200); // set up the Serial output so we can look at the light level
  startMozzi(); // :))
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
  // print the value to the Serial monitor for debugging
  Serial.print("LDR1 = ");
  Serial.print(LDR1_value);
  Serial.print("\t"); // prints a tab

  int LDR1_calibrated = kMapIntensity(LDR1_value);
  Serial.print("LDR1_calibrated = ");
  Serial.print(LDR1_calibrated);
  Serial.print("\t"); // prints a tab

  // calculate the fm_intensity
  fm_intensity = ((long)LDR1_calibrated * knob2Val * (kIntensityMod.next() + 128)) >> 8; // shift back to range after 8 bit multiply
  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.print("\t"); // prints a tab

  // read the light dependent resistor on the speed Analog input pin
  int LDR2_value = mozziAnalogRead(LDR2_PIN); // value is 0-1023
  Serial.print("LDR2 = ");
  Serial.print(LDR2_value);
  Serial.print("\t"); // prints a tab

  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(LDR2_value) / 1000;
  Serial.print("   mod_speed = ");
  Serial.print(mod_speed);
  kIntensityMod.setFreq(mod_speed);

  Serial.println(); // finally, print a carraige return for the next line of debugging info
}

void loop() {
  digitalRead(switch1);
  if (switch1 == HIGH) {
    Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aCarrier(TRIANGLE2048_DATA);
    Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aModulator(TRIANGLE2048_DATA);
    Oscil<TRIANGLE2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(TRIANGLE2048_DATA);
  } else {
    //    Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
//    Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
//    Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);
  }
  audioHook();
}

int updateAudio() {
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}
