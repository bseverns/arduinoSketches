//**************************************************************************
//*   Simple AVR wavetable synthesizer V1.0                                *
//*                                                                        *
//*   Implements 4 voices using selectable waveform and envelope tables    *
//*   Uses 8-bit PWM @ 62.5 kHz for audio output                           *
//*                                                                        *
//*   (C) DZL 2008                                                         *
//**************************************************************************

// Refactoring pass 1. Subsequent passes will address the interface to accessing CVoice members of the CIllutronB class

// The Illutron is a wave table synthesiser, which generates complex sound by combining simpler wave forms
// Each of the voices is formed by combining one waveforms - noise,ramp,saw,sin,square,triangle - included with the library
// With an envelope - the envelope controls how the sound evolves over time - the initial attach, the sustain and then the decay of the note
// The synth provides additional parameters from controlling the pitch, duration and pitch over time of the sound
//
// The synth is the original work of Nikolaj Mobius and has been refactored, optimised and documented by Duane B (rcarduino)
//
// The synth is contained in the files IllutronB.h and IllutronB.cpp
//
// This sketch presents a demonstration of the synth sound using a simple sequencer


#include <math.h>

#include "avr/interrupt.h"
#include "avr/pgmspace.h"

#include "IllutronB.h"

// Include the wavetables, you could add your own as well
#include "sin256.h"
#include "ramp256.h"
#include "saw256.h"
#include "square256.h"
#include "noise256.h"
#include "tria256.h"

// include the envelopes, again you could add more 
#include "env0.h"
#include "env1.h"
#include "env2.h"
#include "env3.h"

// include the pitches as defined by the Arduino tone function
// while these work - it would be useful to check that the tones are correct against a tuned instrument
// the midi note calculations and tone library pitches do not agree with each other.
#include "pitches.h"

#include "AmenBreak.h"

#define PLAY_BACK_BPM_PIN 0 // analog pin 0
#define PITCH_PIN 1 // analog pin 1

// Pins and other definitions for the LED Visualiser
#define REFRESH_DIVIDER 16

// These pins are the least interesting digital pins, no interrupts, no SPI, no PWM
// so lets use them for the LEDs
#define CHANNEL0_LED 8
#define CHANNEL1_LED 12
// digital pin 5 might be used for output if we are splitting channels.
// digital pin 6 is our output
#define CHANNEL2_LED 7
#define CHANNEL3_LED 13

void setup()
{
  Serial.begin(9600);

/* PROJECT SPECIFIC SETUP */
  pinMode(CHANNEL0_LED,OUTPUT);
  pinMode(CHANNEL1_LED,OUTPUT);
  pinMode(CHANNEL2_LED,OUTPUT);
  pinMode(CHANNEL3_LED,OUTPUT);
  
  CIllutronB::setBPM(120);  
  CIllutronB::initSynth();

  CIllutronB::m_Voices[0].setup((unsigned int)SinTable,200.0,(unsigned int)Env1,1.0,300);
  CIllutronB::m_Voices[1].setup((unsigned int)RampTable,100.0,(unsigned int)Env1,1.0,512);
  CIllutronB::m_Voices[2].setup((unsigned int)TriangleTable,100.0,(unsigned int)Env2,.5,1000);
  CIllutronB::m_Voices[3].setup((unsigned int)NoiseTable,1200.0,(unsigned int)Env3,.02,500);
}

uint8_t nCycle = 0;
uint8_t nBeat = 0;

void loop()
{
    // The synth works in the background using a timer interrupt
    // Ask the IllutronB if the current beat has completed, if so lets add the next one
    if(CIllutronB::beatComplete()) 
    {
      // This is just for fun - allow the user to change the play back speed at anytime using
      // a potentiometer on analogue pin A1 - Map the potentiometer to a range of 80 to 240 BPM
      // Use this for user control of BPM
//      CIllutronB::setBPM(map(analogRead(PLAY_BACK_BPM_PIN),0,1024,80,240));        
      // use this to hear the original sequence at the original play back speed
      CIllutronB::setBPM(140);        
      
      // If there is a new beat/note for this channel, tell the Illutron B to play it
      // repeat simple repeats the note using whatever configuration it was previously given
      // its good for drum sounds where you just want to repeat without changing the tone
      unsigned char sNote = pCurrentSequence->getTrigger(0,nBeat);
      if(sNote /*&& digitalRead(CHANNEL0_GATE)*/)
      {
        CIllutronB::m_Voices[CHANNEL_0].trigger();
      }
      
      // triggerMidi allows you to trigger the channel to play back a sound at a particular note
      // it uses the midi note number, there is a look up table here - http://www.phys.unsw.edu.au/jw/notes.html
      // Notice that this is also connected to an analog input, this adjusts the base play back note.
      sNote = pCurrentSequence->getTrigger(1,nBeat);
      if(sNote /*&& digitalRead(CHANNEL1_GATE)*/)
      {
        // Use this to add user control of the pitch other wise the default will play the pitch defined in the sequence
        //CIllutronB::m_Voices[CHANNEL_1].triggerMidi(analogRead(PITCH_PIN)>>3);
        // To hear the original sequence played as intended, use the following - 
        CIllutronB::m_Voices[CHANNEL_1].triggerMidi(sNote);
      }
      
      // This is also a neat trick, it looks at the pattern in channel 2 to trigger the percussion sound
      // but uses the notes in parttern 1 to add some variety to the sound by playing it a different pitches
      sNote = pCurrentSequence->getTrigger(2,nBeat);
      if(sNote /*&& digitalRead(CHANNEL2_GATE)*/)
      {
        CIllutronB::m_Voices[CHANNEL_2].triggerMidi(sNote);
      }
      
      // another example of simply repeating a drum sound
      sNote = pCurrentSequence->getTrigger(3,nBeat);
      if(sNote /* && digitalRead(CHANNEL3_GATE)*/)
      {
        // double up for a bang and then sustain using two voices, one for the bang and one for the sustain
        CIllutronB::m_Voices[CHANNEL_0].trigger();
        CIllutronB::m_Voices[CHANNEL_3].trigger();
      }
 
      // update the beat counter
      nBeat++;     

      switch(nCycle)
      {
        case 4:
          CIllutronB::m_Voices[3].setup((unsigned int)TriangleTable,1500.0,(unsigned int)Env3,.03,100);
          break;      
        case 8:
          CIllutronB::m_Voices[3].setup((unsigned int)NoiseTable,1500.0,(unsigned int)Env3,.03,300);
          break;      
        case 12:
          CIllutronB::m_Voices[1].setup((unsigned int)TriangleTable,100.0,(unsigned int)Env1,2.0,512);
          break;      
        case 16:
          CIllutronB::m_Voices[1].setup((unsigned int)RampTable,100.0,(unsigned int)Env1,1.0,512);
          break;      
      }
      
      // if it gets to the end of our sequence, reset it and update the cycle counter
      // the cycle counter is used below to change some of the voices
      if(nBeat == pCurrentSequence->getLength())
      {
        nBeat=0;
        nCycle++;
        if(nCycle >= 16)
        {
          nCycle = 0;
        }
      }
    }
    
    // Thats it, now make some music and if its good feel free to post it here - 
    // http://rcarduino.blogspot.com/2012/08/the-must-build-arduino-project-illutron.html
    // If its really good I will add it as an option in the source code guaranteeing your future fame and fortune.
   
    // Duane B rcarduino.blogspot.com 
   
  updateVisualiser();
}

void updateVisualiser()
{
  // for each channel we have an 8-bit power level - its the amplitude
  // lets divide by 4 to give 0-15
  // lets decrement each pass, if high byte set, set top led, keep the bottom led on as long as the value is > 1
  static unsigned char sRefreshDivider = REFRESH_DIVIDER;
  static unsigned char sChannelPower[CHANNEL_MAX];
  
  if(0 == sRefreshDivider)
  {
    for(unsigned char sIndex = 0;sIndex < CHANNEL_MAX;sIndex++)
    {
      sChannelPower[sIndex] = (CIllutronB::m_Voices[sIndex].getAmplitude() >> 4);
    }
    sRefreshDivider = REFRESH_DIVIDER;
  }
  
  // this is ok for a demo, but in anything more sophisticated we would want faster updates using direct port access.
  digitalWrite(CHANNEL0_LED,sChannelPower[0]);
  digitalWrite(CHANNEL1_LED,sChannelPower[1]);
  digitalWrite(CHANNEL2_LED,sChannelPower[2]);
  digitalWrite(CHANNEL3_LED,sChannelPower[3]);
  
  for(unsigned char sIndex = 0;sIndex < CHANNEL_MAX;sIndex++)
  {
    if(sChannelPower[sIndex]>0)
      sChannelPower[sIndex]--;
  }
  
  sRefreshDivider--;
}


