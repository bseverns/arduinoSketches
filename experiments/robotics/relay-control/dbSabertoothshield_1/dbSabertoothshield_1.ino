// We'll name the SyRen object M1 & M2.
// For how to configure the SyRen, see the DIP Switch Wizard for
//   http://www.dimensionengineering.com/datasheets/SyrenDIPWizard/start.htm
// Be sure to select Simplified Serial Mode for use with this library.
// This sample uses a baud rate of 9600.
//
// Connections to make:
//   Arduino TX->1  ->  SyRen S1
//   Arduino GND    ->  SyRen 0V
//   Arduino VIN    ->  SyRen 5V (OPTIONAL, if you want the SyRen to power the Arduino)
//
// If you want to use a pin other than TX->1, see the SoftwareSerial example.

#include <SyRenSimplified.h>

SyRenSimplified M1;
SyRenSimplified M2;

int x;

int Button1_Pin = A1;

void setup() {
  // put your setup code here, to run once:
  SyRenTXPinSerial.begin(9600); // This is the baud rate you chose with the DIP switches.

}

void loop() {
  if(analogRead(Button1_Pin) == LOW)
  {
    backwards();
    delay(500);
    turn();
    delay(500);
  }
  else
  {
    forward();
  }  
}  

void forward() {
  M1.motor(90);
  M2.motor(90);
}

void backwards() {
  M1.motor(-127);
  M2.motor(-127);
}

void turn() {
  for (int turn = 0; turn < (400+random(800)); turn++) {
    M1.motor(127);
    M2.motor(0);
  }
}




