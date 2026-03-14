/* 
  Demonstrates analog input, audio and control oscillators, phase modulation
  and smoothing a control signal at audio rate to avoid clicks.
  Also demonstrates AutoMap, which maps unpredictable inputs to a set range.
  
  This example goes with a tutorial on the Mozzi site:
  http://sensorium.github.io/Mozzi/learn/Mozzi_Introductory_Tutorial.pdf
  
  The circuit:
     Audio output on digital pin 9 (on a Uno or similar), or 
     check the README or http://sensorium.github.com/Mozzi/

     Potentiometer connected to analog pin 0-4.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
  
  Mozzi help/discussion/announcements:
  https://groups.google.com/forum/#!forum/mozzi-users

  Ben Severns 2020.
  This example code is in the public domain.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator 
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range
  
// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

const int MIN = 1;
const int MAX = 10;

const int MIN_2 = 1;
const int MAX_2 = 15;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);
AutoMap mapThis(0,1023,MIN,MAX);
AutoMap mapThisToo(0,1023,MIN_2,MAX_2);

// --- Control surface wiring ---
// A0: main pitch knob controlling the carrier frequency.
// A1: FM depth knob (feedback/brightness).
// A2: LFO rate knob for modulation-speed control.
// A3: Texture knob that scales modulation depth in the fm_intensity calc.
// A4: Harmonic multiplier knob nudging the modulator/carrier ratio.

const int KNOB_PIN = 0;           // carrier pitch
const int FM_INTENSITY_PIN = 1;   // FM intensity control
const int MOD_SPEED_PIN = 2;      // LFO speed
const int TEXTURE_PIN = 3;        // secondary depth/timbre scaler
const int RATIO_PIN = 4;          // harmonic ratio multiplier

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);

void setup(){
  Serial.begin(115200); // set up the Serial output so we can watch the controls breathe
  startMozzi(); // :))
}

void updateControl(){
    
  int ratio_knob = mozziAnalogRead(RATIO_PIN); // value is 0-1023
  int ratio_multiplier = mapThis(ratio_knob);

  int texture_knob = mozziAnalogRead(TEXTURE_PIN); // value is 0-1023
  int texture_multiplier = mapThisToo(texture_knob);
  
  // read the knob
  int knob_value = mozziAnalogRead(KNOB_PIN); // value is 0-1023
  
  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value);
  
  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio * ratio_multiplier;
  
  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq); 
  aModulator.setFreq(mod_freq);
  
  // read the FM depth control (A1)
  int pot1_value= mozziAnalogRead(FM_INTENSITY_PIN); // value is 0-1023
  // print the value to the Serial monitor for debugging
  Serial.print("pot1 = "); 
  Serial.print(pot1_value);
  Serial.print("\t"); // prints a tab

  int pot1_calibrated = kMapIntensity(pot1_value);
  Serial.print("pot1_calibrated = ");
  Serial.print(pot1_calibrated);
  Serial.print("\t"); // prints a tab
  
  // calculate the fm_intensity using the secondary texture control and LFO
  fm_intensity = ((long)pot1_calibrated * texture_multiplier * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply
  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.print("\t"); // prints a tab
  
  // read the modulation-speed control (A2)
  int pot2_value= mozziAnalogRead(MOD_SPEED_PIN); // value is 0-1023
  Serial.print("pot2 = "); 
  Serial.print(pot2_value);
  Serial.print("\t"); // prints a tab
  
  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(pot2_value)/1000;
  Serial.print("   mod_speed = ");
  Serial.print(mod_speed);
  kIntensityMod.setFreq(mod_speed);
  
  Serial.println(); // finally, print a carraige return for the next line of debugging info
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}
