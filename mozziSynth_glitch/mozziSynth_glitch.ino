/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.

    Demonstrates the use of Oscil to play a wavetable.

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or 
    check the README or http://sensorium.github.com/Mozzi/

    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h>//saw table for oscillator
#include <tables/square_analogue512_int8.h> // square table for oscillator
#include <tables/triangle2048_int8.h>//triangle table for oscillator



#include <Ead.h>
#include <EventDelay.h>


Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw(SAW2048_DATA);
Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSqu(SQUARE_ANALOGUE512_DATA);



Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> lfo_aSaw(SAW2048_DATA);
Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> lfo_aSqu(SQUARE_ANALOGUE512_DATA);

Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> lfo_aSqu2(SQUARE_ANALOGUE512_DATA);


// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 256 // powers of 2 please
EventDelay kDelay;
EventDelay kDelay2;
Ead kEnvelope(CONTROL_RATE);


int gain;
int gate=1;
int t1;
int t2;

const int Pad1=5;
const int Pad2=4;
const int Pad3=3;
const int Pad4=2;

const int LED1=10;
const int LED2=8;
const int LED3=7;
const int LED4=6;


const int VFO_type=11;
const int LFO_type=12;

int P1[7];
int P2[7];
int P3[7];
int P4[7];

int P=1;


int v;
int vt;
int l;
int lt;
int a;
int d;
int l2;

void setup(){
  Serial.begin(115200);
  Serial.println("Ready");  
  pinMode(Pad1,INPUT);
  pinMode(Pad2,INPUT);
  pinMode(Pad3,INPUT);
  pinMode(Pad4,INPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(VFO_type,INPUT);
  pinMode(LFO_type,INPUT);
  Serial.println("Start");
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  satus(1);
  satus(2);
  satus(3);
  satus(4);
}


void updateControl(){
  int temp=mozziAnalogRead(2);
  int TEMP=map(temp,0,1023,0,100);
  kDelay2.set(a+d+TEMP);

  if (digitalRead(Pad1)==1){
      ledoff();
      digitalWrite(LED1,HIGH);
      satus(1);
      P=1;
    }
    else if(digitalRead(Pad2)==1){
      ledoff();
      digitalWrite(LED2,HIGH);
      satus(2);
      P=2;
    }
    else if(digitalRead(Pad3)==1){
      ledoff();
      digitalWrite(LED3,HIGH);
      satus(3);
      P=3;
    }
    else if(digitalRead(Pad4)==1){
      ledoff();
      digitalWrite(LED4,HIGH);
      satus(4);
      P=4;
    }
    else{

    }
    switch (vt){
      case 0:
          aSin.setFreq(v);
          break;
      case 1:
          aSaw.setFreq(v);       
          break;
    }

    switch (lt){
      case 0:
          lfo_aSaw.setFreq(l);
          break;
       case 1:
          lfo_aSqu.setFreq(l);
          break;
    }            
//  lpf.setCutoffFreq(c);
  if(kDelay.ready()){
        switch(P){
      case 1:    
        vt=P1[0];v=P1[1];lt=P1[2];l=P1[3];a=P1[4];d=P1[5];c=P1[6];
        P=2;
        ledoff();
        digitalWrite(LED1,HIGH);
        break;
       case 2:
        vt=P2[0];v=P2[1];lt=P2[2];l=P2[3];a=P2[4];d=P2[5];c=P2[6];
        P=3;
        ledoff();
        digitalWrite(LED2,HIGH);
        break;
       case 3:    
        vt=P3[0];v=P3[1];lt=P3[2];l=P3[3];a=P3[4];d=P3[5];c=P3[6];
        P=4;
        ledoff();
        digitalWrite(LED3,HIGH);
        break;
       case 4:
        vt=P4[0];v=P4[1];lt=P4[2];l=P4[3];a=P4[4];d=P4[5];c=P4[6];
        P=1;
        ledoff();
        digitalWrite(LED4,HIGH);
        break;
       default:
//        P=random(1,4);
          break;
   }
    kEnvelope.start(a,d);
    kDelay.start(a+d+TEMP);
    gate=0;
  }
    gate=1;
    lfo_aSqu2.setFreq(l2);
    gain= (int) kEnvelope.next();
}

int updateAudio(){
  int vco_aSig;
  switch (vt){
    case 0:
      vco_aSig=aSin.next();
      break;
    case 1:
      vco_aSig=aSaw.next();
      break;    
  }
  int lfo_aSig;
  switch (lt){
    case 0:
      lfo_aSig=lfo_aSaw.next();
      break;
    case 1:
      lfo_aSig=lfo_aSqu.next();
      break;    
  } 
  int lfo2_aSig=lfo_aSqu2.next();
  int aSig;
  aSig=(vco_aSig+lfo_aSig)>>1;
  aSig=(aSig+lfo2_aSig)/2;
  aSig=(gain*aSig)>>8;

  return aSig*gate; 
}

void loop(){
  audioHook(); // required here
}

void satus(int p){
    int vco=mozziAnalogRead(0);
    int lfo=mozziAnalogRead(1);    
    int attack=mozziAnalogRead(5);
    int decay=mozziAnalogRead(4);
    int lfo2=mozziAnalogRead(3);


    int VCO=map(vco,0,1023,1500,50);
    int LFO=map(lfo,0,1023,1500,50);
    int A=map(attack,0,1023,500,30);
    int D=map(decay,0,1023,500,30);
    int LFO2=map(lfo2,0,1023,50,5); 
    int vt=digitalRead(VFO_type);
    int lt=digitalRead(LFO_type);

    switch(p){
      case 1:
        P1[0]=vt;
        P1[1]=VCO;
        P1[2]=lt;
        P1[3]=LFO;
        P1[4]=A;
        P1[5]=D;
        P1[6]=LFO2;
        break;
      case 2:
        P2[0]=vt;
        P2[1]=VCO;
        P2[2]=lt;
        P2[3]=LFO;
        P2[4]=A;
        P2[5]=D;
        P2[6]=LFO2;
        break;
      case 3:
        P3[0]=vt;
        P3[1]=VCO;
        P3[2]=lt;
        P3[3]=LFO;
        P3[4]=A;
        P3[5]=D;
        P3[6]=LFO2;
        break;
      case 4:
        P4[0]=vt;
        P4[1]=VCO;
        P4[2]=lt;
        P4[3]=LFO;
        P4[4]=A;
        P4[5]=D;
        P4[6]=LFO2;
        break;
      default:
        break;
    }

}

void ledoff(){
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
}
