//Run on Arduino Pro Mini - for Made Here: Energy
#include <AlaLed.h> //library for timer constant on/fade/off/fade/on

AlaLed leds; //variable for library
int TIP = 6; //to  resistor/darlington transistor circuit

//sequence fade in/fade out cycle
AlaSeq seq[] =
{
  {ALA_FADEIN, 30000, 30000},
  {ALA_ON, 10000, 10000},
  {ALA_FADEOUT, 30000, 30000},
  {ALA_OFF, 10000, 10000},
  {ALA_ENDSEQ, 0, 0}
};

void setup()
{
  pinMode (TIP, OUTPUT);
  // establish pwm pins
  leds.initPWM(TIP);
  // set a fading animation with a duration of 30 seconds
  leds.setAnimation(seq);
}

void loop()
{
  // run the animation
  leds.runAnimation();
}
