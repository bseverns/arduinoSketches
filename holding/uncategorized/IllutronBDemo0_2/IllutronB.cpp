
#ifndef ILLUTRONB
#include "illutronB.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IllutronB Wave Table Synthesizer Based on the original Illutron synthesiser work of Nikolaj Mobius
//
// Refactored and optimised and documented by Duane B aka RCArduino
// 
// Differences from original Illutron -
//
// The synthesisor is now encapsulated in two classes - CIllutronB and a nested class CIllutronB::CVoice
//
// The voice class contains the parameters of each of the musical channels/tracks/voices - if you are
// not of a musical background you can think of each voice as a separate instrument. The voice class 
// also performs the calculations required to generate the sound for each voice/instrument when it is asked by the containing
// CIllutronB class
//
// The CIllutronB class implements the main synthesisor functions which are - 
//
// 1) Containing the voices that make up the synth sound
// 2) Asking the voices to calculate thier output whenever the synth needs to update the sound - so thats 8,000 times a second !
// 3) Performing additional calculations on the voice outputs to generate the sound - scaling them so that they can share one 8 bit channel
// 4) Generating the output - its incedible, but the entire sound of the synth including the four independent channels is being output through PWM on one 8 bit timer - digital pin 6
// 5) Providing a tempo - the user can update the Beats per minute at any time by calling - setBPM - the pitch will not change, just the speed
//
// Note that a squencer is not currently provided within CIllutronB - it would be a logical development to provide an additional sequencer class
// It would also be logical to provide a class for storing and retreiving voices. But lets get creating first.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Bitwise operations - set bit, clear bit, check bit, toggle bit
// TODO - there must be standard Arduino definitions for these, if so replace the these and use the standard versions
#define SET(x,y) (x |=(1<<y))		        					//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       						// |
#define CHK(x,y) (x & (1<<y))           						// |
#define TOG(x,y) (x^=(1<<y))            						//-+

// This stores a look up table from midi note numbers to frequencies in Hz
// TODO - I am not convinced that the forumla is correct, and I do not beleive this should
// be in run time memory, a better idea is to calculate the mapping once and store
// it in progmem along with the wave tables and envelope tables.
// will move it once someone has confirmed the accuracy or not of the calculations.
unsigned int PITCHS[128];

// We use two timers to generate the sound - Timer1 provides an interrupt 8000 times a second which we use to update output
// The output itself is through PWM using timer 0 on digital pin 6 - it is incredible that this much sound and variety of sound
// is possible through 8 bit PWM.

// This is triggered 8000 times a second, we call the CIllutronB::OCR1A_ISR() function to update the output
// TODO consider renaming OCR1A_ISR to update ?
// TODO Timer1 is a 16 bit timer used by the servo library among others - should look at using a less valuable 8-bit timer instead.
SIGNAL(TIMER1_COMPA_vect)
{
  CIllutronB::OCR1A_ISR();
}

// Setup up the timers - 
// TODO - rename start and provide a stop function which will free up the timers
// allowing the synth to be used periodically within a project which requires the timers
// at other times.
void CIllutronB::initSynth()
{
  // Set up Timer 1 output compare interrupt A
  // Timer 1 is basically used as a scheduler that triggers at a regular interval for use to update the synth outputs.
  TCCR1B=0x02;          // set the timer prescaler to 8 = 16/8 = 2MHz
  SET(TIMSK1,OCIE1A);   // Enable output compare match interrupt on OCR1A
  sei();
  
  // Set up the output timer - At the risk of repeating myself, it is incredible that the full sound range of the IllutronB
  // is possible through the PWM output of just this one 8 bit timer.
  TCCR0A=0B10110011;                                    //-8 bit audio PWM
  //TCCR0A=0x83;          // Set timer waveform generation mode to FAST PWM, clear OC0A On match, set at bottom - OC0A = digital pin 6.
  TCCR0B=0x01;          // Set to clock frequency, no prescaler
  OCR0B = OCR0A=127;            // set in the middle - do we need this ? probably not.
  SET(DDRD,5);          // Set digital pin 5 to output - channels 0 and 1
  SET(DDRD,6);          // Set digital pin 6 to output - channels 2 and 3

  
  // see related comments at the definition of PITCHS - dont like this approach, will change it in future.
  for(unsigned char i=0;i<128;i++)
  { 
    // 440 is A4 on the piano 
    PITCHS[i]=(440. * exp(.057762265 * (i - 69.)))/(SAMPLE_RATE/65535.0);
    Serial.println(PITCHS[i]);    
  }
}

// Sets the beats per minute, the beatComplete function will return 
// true whenever a beat is complete
// TODO - Will need revisision - do sequencers normally operate on whole beats, quarter beats, sixteenths ?
void CIllutronB::setBPM(uint8_t sBPM)
{
  m_unBPMCounterStart = (UPDATE_RATE/((float)sBPM/60.0))/4.0;
  m_unBPMCounter = m_unBPMCounterStart;
  m_sBeatComplete = false;
}

// tests the beat complete flag to see if the last beat is finished and a new one should be send
// its optional, but allows for a simple sequencer to be run in your loop function.
unsigned char CIllutronB::beatComplete()
{
  uint8_t oldBeatComplete = m_sBeatComplete;
  if(oldBeatComplete)
  {
    m_sBeatComplete = false;
  }
  return oldBeatComplete;
}

// This is where all the work happens - or used to 
// I have introduced the CIllutronB::CVoice class to make this easier to understand it still contains all of the work
// but calls members of CIllutronB::CVoice to do a lot of the work on its behalf.
//
// This function runs 8000 times a second and updates the PWM frequency of digital pin 6 each time based on 
// the output from the four voices which it mixes together to form a single output.
void CIllutronB::OCR1A_ISR()
{
  OCR1A+=(TIMER1_FREQUENCY/UPDATE_RATE); // set timer to come back for the next update in 125us time
  // figure out which updates we need to perform
  uint8_t bUpdateEnvelope = false;
  if(0 == m_sEnvelopeDivider)
  {
    m_sEnvelopeDivider = ENVELOPE_DIVIDER;
    bUpdateEnvelope = true;
  }
  else
  {
    m_sEnvelopeDivider--;
  }
  
  //************************************************
  //  Pitch Modulation Engine
  //************************************************
  // Again, like the envelope we do not update this every time, we use a divider
  uint8_t bApplyEnvelopePitchModulation = (0 == m_unEnvelopePitchModulationDivider);
  if(0 == bApplyEnvelopePitchModulation)
  {
   m_unEnvelopePitchModulationDivider = MODULATION_PITCH_DIVIDER;
  }
  else
  {
    m_unEnvelopePitchModulationDivider--;
  }
  
  //-------------------------------
  //  Synthesizer/audio mixer
  //-------------------------------
  
  // This is incredibly simple and incredibly powerful - 
  // Take the output of the four voices and add them all together - thats basically it, four channels mixed !
  // There is a little more to it, each of the channels has an output as big as the range that our single output
  // timer allows so we need to scale them to fit into this range, how ? divide by four, thats it, four 8 bit channels
  // added together, divided by four and sent to our 8 bit PWM output on timer 0
  // to generate the output all we are doing is updating the output compare register A of timer0 to control its duty cycle
  // when we pass this through a simple RC filter it because and analogue audio signal.
  // Note that we are controlling the duty cycle - the time that it is on as opposed to off and we are not controlling
  // the frequency of the timer itself which stays constant.
  // see link for a demonstration and explanation of a single channel producing a sinewave using this technique
  // http://interface.khm.de/index.php/lab/experiments/arduino-dds-sinewave-generator/ 
  // TODO - get permission to include link
  
  // TODO - need to add a means to control whether we split the channels or not.
  // TODO - should also look at using just one timer, think its possible.
  // update the timer duty cycle 
  
  //Use this for two voices per channel (pins 5 and 6)
  OCR0A=127+((m_Voices[0].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation) + m_Voices[1].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation))>>1);
  OCR0B=127+((m_Voices[2].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation) + m_Voices[3].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation))>>1);
  //Or this for four voices on single channel pin 6
  //OCR0A=127+(((m_Voices[0].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation) + m_Voices[1].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation))
  //+(m_Voices[2].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation) + m_Voices[3].getSample(bUpdateEnvelope,bApplyEnvelopePitchModulation)))>>2);
  
    
  // very simply counts down from a value set in setBPM and sets
  // a m_sBeatComplete when it reaches 0 before starting the count
  // again. m_sBeatComplete can be read from outside to 
  // determing if a beat is complete !
  m_unBPMCounter--;
  // is beat complete 
  if(0 == m_unBPMCounter)
  {
    // yes, restart counter, set m_sBeat true
    m_unBPMCounter = m_unBPMCounterStart;
    m_sBeatComplete = true;
  }
}

// definitions of the CIllutronB static member variables - see the .h file for comments
volatile unsigned int CIllutronB::m_unBPMCounterStart = 0;
volatile unsigned int CIllutronB::m_unBPMCounter =0;                                   
volatile unsigned char CIllutronB::m_sBeatComplete=0;

volatile unsigned char CIllutronB::m_sEnvelopeDivider=ENVELOPE_DIVIDER;             
volatile unsigned int CIllutronB::m_unEnvelopePitchModulationDivider = MODULATION_PITCH_DIVIDER;

CIllutronB::CVoice CIllutronB::m_Voices[4];

//////////////////////////////////////////////////////
// The CIllutronB::CVoice class
//
// The CIllutronB class does the scheduling and mixing of channel updates and outputs
// The CIllutronB::CVoice class does the work of calculating the raw outputs for a voice
// It also contains the voice configuration - mainly the current wave and envelope
//
//////////////////////////////////////////////////////

CIllutronB::CVoice::CVoice()
{  
  // record the voice configuration - see the .h file for details
  
  // TODO - add a sensible default here or have the synth create a sensible default ?
  // or because we do not want to force inclusion of a specific set of wavetables and envelopes
  // put additional checks in the code to cope with a null wavetable and or envelope 
  
  // the same comment really applies to all default values below - 
  m_unEnvelopeTableStart = NULL;
  m_unEnvelopePhaseAccumulator = 0x8000;
  m_unEnvelopePhaseIncrement = 10;
  m_sAmplitude = 255;
  
  m_nEnvelopePitchModulation = 0;
  
  m_unWaveTableStart = NULL;
  m_unWavePhaseAccumulator = 0;
  m_unWavePhaseIncrement = 1000;
  
  m_unPitch = 500;
}

// This function could equally have been called decay - everytime its call it moves to the next point in the envelope table
// determined by m_unEnvelopePhaseIncrement, bigger value means we get to the end quicker for a short sound
// lower vales gives a longer note
// the envelope tables describe the strength of the note at different times in its life, generally they start loud and get quieter
// There is not reason you cannot do the opposite - start quiet and get louder - just create a new envelope table
// Thats why the function is not called decay, it could have inverse decay or a pulsed sound for example

// Note - the eventual output of the IllutronB is 8 bit PWM however a lot of the calculations are performed
// at higher resolution using 16 bit or floating point values to give more accuracy than is possible in 8 bit maths.
// where you see >>7 this i division by 128 used to down scale from 16 bits to 8
//
// At first glance this is odd, why not shift by 8 ?
// the reason is that it simplifies the test for the end of the table
// if the top bit 0x8000 is ever set, we know we have passed the end of the table and should clear the amplitude 
// stopping the note.
// For the wave tables we do not follow this approach because it does not matter if they rollover and start again from zero, in fact we want them to.
// void CIllutronB::CVoice::applyEnvelopeToAmplitude()
// NOTE - this is now moved into the if(bUpdateEnvelope) section of get sample
signed char CIllutronB::CVoice::getSample(uint8_t bUpdateEnvelope,uint8_t bApplyEnvelopePitchModulation)
{
  // calculate the amplitude based on the position within the enveloped
  if(bUpdateEnvelope)
  {
    if(!(m_unEnvelopePhaseAccumulator&0x8000))
    {
      // amplitude = envelope position determined by adding envelope increment to envelope accumulator
      m_sAmplitude=pgm_read_byte(m_unEnvelopeTableStart + ((m_unEnvelopePhaseAccumulator+=m_unEnvelopePhaseIncrement)>>7) );
        
      if(m_unEnvelopePhaseAccumulator&0x8000)
      {
        m_sAmplitude=0;
      }
    }
    else
    {
      m_sAmplitude=0;
    }
  }
  
  if(bApplyEnvelopePitchModulation)
  {
    // this works
    // m_unWavePhaseIncrement=m_unPitch+(m_unPitch*(m_unEnvelopePhaseAccumulator/(32767.5*128.0  ))*((int)m_nEnvelopePitchModulation-512));
    // this probably doesn't, as and when we understand the objective of m_nEnvelopePitchModulation we will rework for integer maths.
    // m_unWavePhaseIncrement=m_unPitch+(m_unPitch*(m_unEnvelopePhaseAccumulator/(32767*128))*((int)m_nEnvelopePitchModulation-512));
  }
  
  m_unWavePhaseAccumulator+=m_unWavePhaseIncrement;
  
  // if the amplitude is 0, the sample will always be 0 so there is no need to waste time calculating it.
  if(m_sAmplitude == 0)
  {
    return 0;
  }

  // read a byte representing the current point in the waveform from program memory, multiply it by the current amplitude
  // to mix the waveform with the envelope - its so simple, but this is what makes the rich range of sound from a wavetable synth possible
 
  return (((signed char)pgm_read_byte(m_unWaveTableStart+((m_unWavePhaseAccumulator)>>8))*m_sAmplitude)>>8);
}

unsigned char CIllutronB::CVoice::getAmplitude()
{
  return m_sAmplitude;
}

// Set the characteristics or a voice - waveform table , pitch, envelope table, length of the note, and the pitch modulation
// TODO - at present the length of a note is not changed by changing the BPM - undecided as to whether it should be.
void CIllutronB::CVoice::setup(unsigned int waveform, float pitch, unsigned int envelope, float length, unsigned int mod)
{
  // do the maths before we turn off interrupts
  unsigned int tempEnvelopePhaseIncrement = (1.0/length)/(SAMPLE_RATE/(32767.5*10.0));//[s];
  pitch = pitch/(SAMPLE_RATE/TIMER1_MAX); //[Hz] // based for pitch adjustment - transpose ?
  
  // turn off interrupts and copy the calculated values into the voice
  unsigned char sreg = SREG;
  cli();
  
  m_unWaveTableStart=waveform;//[address in program memory]
  m_unEnvelopeTableStart=envelope;//[address in program memory]
  m_unEnvelopePhaseIncrement = tempEnvelopePhaseIncrement;
  m_unPitch = pitch;
  m_nEnvelopePitchModulation=mod;//0-1023 512=no mod
  
  SREG = sreg;
}

// Play a note in the supplied midi note number using the current voice configuration
void CIllutronB::CVoice::triggerMidi(unsigned char note)
{
//  PITCH[voice]=(440. * exp(.057762265 * (note - 69.)))/(FS/65535.0); //[MIDI note]

  // Original code
  // It looks wrong on two counts -
  // 1) m_unEnvelopePhaseAccumulator is set to zero and then used as a multiplier in the following calculation, its already 0 so has no effect.
  // 2) I am not convinced that envelope pitch modulation should be applied immediatley - I may be wrong on this and will revise accordingly.
  // m_unPitch=PITCHES[note];
  // m_unEnvelopePhaseAccumulator=0;
  // m_unWavePhaseIncrement=m_unPitch+(m_unPitch*(m_unEnvelopePhaseAccumulator/(32767.5*128.0  ))*((int)m_nEnvelopePitchModulation-512));
  
  // Current Thinking of Duane B
  // Each wave form is composed of 256 segments, to play a wave at a given frequency, say A4 (440Hz)
  // We need to get through 256 segments 440 times per second
  // our timer1 interrupt occurs 16000 times per second
  // m_unWavePhaseIncrement = (440 cycles * 256 segments)/16000
  // if we are using midi not numbers we need to calculate the frequency of the pitch first
  // we could use a look up table, but why have 128 notes hanging around in memory especially is we are only using 10 or 20 ?
  
  // lets do this first - its a lot of maths
//  m_unPitch = PITCHS[note];// getFrequencyFromMidiNoteNumber(note);
  unsigned char sreg = SREG;
  cli();
  m_unPitch=PITCHS[note];
  m_unEnvelopePhaseAccumulator=0;
  m_unWavePhaseIncrement=m_unPitch;
  SREG = sreg;
}

// refer to the comments regarding PITCHS - this is not currently used
// ideally a table of midi note to frequency mappings should be pre calculated and included with the library as progmem
unsigned int CIllutronB::CVoice::getFrequencyFromMidiNoteNumber(unsigned char note)
{
  // Based on information provided here - http://www.phys.unsw.edu.au/jw/notes.html
  // as this is a function we can change it to use a look up table without effecting the rest of the code.
  // the best option might be to put a precalculated lookup table into progmem - should do this, its a good option.
  
  return pow(2,((float)note-69.0)/12.0)*440;
}

// play the most recently played note on this voice using the last pitch played and all other configuration unchanged
// its good for repetition like percussion and drums.
void CIllutronB::CVoice::trigger()
{
  m_unEnvelopePhaseAccumulator=0;
}

// trigger using a pitch defined in the pitches.h file supplied with Arduino IDE in the tone examples.
void CIllutronB::CVoice::triggerPitch(uint16_t sPitch)
{
  // do the maths before we block interrupts, this will prevent any glitches that would happen if
  // a lot of work has to be done while interrupts are blocked
  unsigned int tempWavePhaseIncrement = UPDATE_RATE/(sPitch*256.0);
  // now lets turn off interrupts and copy our new value
  // not interrupts = no glitches that would happen from the ISR reading part of the old value and part of the new value.
  uint8_t sreg = SREG;
  cli();
  m_unWavePhaseIncrement = tempWavePhaseIncrement;
  m_unEnvelopePhaseAccumulator = 0;
  SREG = sreg;
}

// TODO - these and other functions can be added to the library once it is clear who is using it and how.
//*********************************************************************
//  Setup Length
//*********************************************************************
/*
void setup_length(unsigned char voice,unsigned char length)
{

	m_unEnvelopePhaseIncrement[voice]=EFTWS[length];

//  m_unEnvelopePhaseIncrement[voice]=(1.0/exp(.057762265 * (length - 69.)))/(FS/(32767.5*10.0));//[s];
//	TOG(PORTB,5);
}
*/

//*********************************************************************
//  Setup mod
//*********************************************************************
/*
void setup_mod(unsigned char voice,unsigned char mod)
{
  m_nEnvelopePitchModulation[voice]=mod*8;//0-1023 512=no mod
//TOG(PORTB,4);

}
*/



