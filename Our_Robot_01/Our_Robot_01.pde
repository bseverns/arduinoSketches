// Arduino
// Pin  Arduino  Description
//  D01
//  D02  
//  D03  Output - Left Motor PWM
//  D04  Output - Left Motor Forward
//  D05  Output - Left Motor Reverse
//  D06  Input  - Left Wall Switch
//  D07  Input  - Right Wall Switch
//  D08  Input  - PIR Sensor
//  D09  Signal - Servo 1 PWM - Defined by servo.h
//  D10  Input  - PING
//  D11  Output - Right Motor PWM
//  D12  Output - Right Motor Forward
//  D13  Output - Right Motor Reverse
//
//  A0  Line Sensor IR 0
//  A1  Line Sensor IR 1
//  A2  Line Sensor IR 2
//  A3  Line Sensor IR 3
//  A4  Line Sensor IR 4
//  A5
#include <Servo.h>
#include <avr/pgmspace.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
Servo LiftServo;

int Liftpos = 0;

int MotorAPin1 = 5;
int MotorAPin2 = 4;
int MotorAPWM  = 3;

int MotorBPin1 = 13;
int MotorBPin2 = 12;
int MotorBPWM  = 11;

int PIRSensor = 8;

int LS0Pin = 0;       // select the input pin for the line sensor
int LS1Pin = 1;       // select the input pin for the line sensor
int LS2Pin = 2;       // select the input pin for the line sensor
int LS3Pin = 3;       // select the input pin for the line sensor
int LS4Pin = 4;       // select the input pin for the line sensor

int LS0Val = 0;       // variable to store the value coming from the sensor
int LS1Val = 0;       // variable to store the value coming from the sensor
int LS2Val = 0;       // variable to store the value coming from the sensor
int LS3Val = 0;       // variable to store the value coming from the sensor
int LS4Val = 0;       // variable to store the value coming from the sensor

int LS0Pct = 0;       // sensor value in percent
int LS1Pct = 0;       // sensor value in percent
int LS2Pct = 0;       // sensor value in percent
int LS3Pct = 0;       // sensor value in percent
int LS4Pct = 0;       // sensor value in percent

unsigned long echo = 0;
int pingSignal = 10;
unsigned long pingValue = 0;
  
  int Position = 0;
  int LSCalLo = 500;
  int LSCalHi = 1100;
  int HighVal = 0;
  int DeadBand = 2;
  
   unsigned long ping(){
pinMode(pingSignal, OUTPUT); // Switch signalpin to output
digitalWrite(pingSignal, LOW); // Send low pulse
delayMicroseconds(2); // Wait for 2 microseconds
digitalWrite(pingSignal, HIGH); // Send high pulse
delayMicroseconds(5); // Wait for 5 microseconds
digitalWrite(pingSignal, LOW); // Holdoff
pinMode(pingSignal, INPUT); // Switch signalpin to input
digitalWrite(pingSignal, HIGH); // Turn on pullup resistor
  echo = pulseIn(pingSignal, HIGH); //Listen for echo
  pingValue = (echo / 58.138); //convert to CM
return echo;

}
int state = digitalRead(PIRSensor); // read the sensor and store it in "state"
    
void setup() {

  Serial.begin(9600);  
  
  pinMode(PIRSensor, INPUT);
  
  pinMode(MotorAPin1, OUTPUT);
  pinMode(MotorAPin2, OUTPUT);
  pinMode(MotorAPWM, OUTPUT);
  
  pinMode(MotorBPin1, OUTPUT);
  pinMode(MotorBPin2, OUTPUT);
  pinMode(MotorBPWM, OUTPUT);
  
  digitalWrite(MotorAPWM, HIGH);
  digitalWrite(MotorBPWM, HIGH);
  
  pinMode(pingSignal,OUTPUT);
  
  LiftServo.attach(9);
  
  delay (5000);
}

 

void loop() 

{
int x = 0;
if (state != 75)
{
Serial.println("HIDE!"); // send the string "HIDE" back to the computer
}
else
{

delay(100); // we have to make a delay to avoid overloading the serial port
}
  
  x = ping();
  Serial.println(x);
  delay(250); //delay 1/4 seconds.
  
  Position = 0;
  LS0Val = analogRead(LS0Pin);    // read the value from the sensor
  LS1Val = analogRead(LS1Pin);    // read the value from the sensor
  LS2Val = analogRead(LS2Pin);    // read the value from the sensor
  LS3Val = analogRead(LS3Pin);    // read the value from the sensor
  LS4Val = analogRead(LS4Pin);    // read the value from the sensor
  
  LS0Pct = 100 * (LS0Val - LSCalLo)/(LSCalHi-LSCalLo);
  LS1Pct = 100 * (LS1Val - LSCalLo)/(LSCalHi-LSCalLo);
  LS2Pct = 100 * (LS2Val - LSCalLo)/(LSCalHi-LSCalLo);
  LS3Pct = 100 * (LS3Val - LSCalLo)/(LSCalHi-LSCalLo);
  LS4Pct = 100 * (LS4Val - LSCalLo)/(LSCalHi-LSCalLo);  

 Serial.println("0");
  Serial.println(LS0Pct);
 Serial.println("1");
  Serial.println(LS1Pct);
 Serial.println("2");
  Serial.println(LS2Pct);
 Serial.println("3");
  Serial.println(LS3Pct);
 Serial.println("4");
  Serial.println(LS4Pct);
 Serial.println("Done.");

 Position = 0;
 HighVal = LS0Pct;
 if(LS1Pct > HighVal + DeadBand)
 { 
   HighVal = LS1Pct;
   Position = 1;}
 if(LS2Pct > HighVal + DeadBand)
 { 
   HighVal = LS2Pct;
   Position = 2;}
 if(LS3Pct > HighVal + DeadBand)
 { 
   HighVal = LS3Pct;
   Position = 3;}
 if(LS4Pct > HighVal + DeadBand)
 { 
   HighVal = LS4Pct;
   Position = 4;}

 Serial.println("Position");
 Serial.println(Position);
  
  state = digitalRead(PIRSensor);
  //analogRead(IR1Pin);    // read the value from the sensor
 if((state == 1)||(x < 75)){
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, LOW);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, LOW); 
   
   for(Liftpos = 45; Liftpos < 90; Liftpos +=1)
      {
        LiftServo.write(Liftpos);
        delay(15);
      }
 }    
 else {
  for(Liftpos = 90; Liftpos >= 45; Liftpos -=1)
      {
        LiftServo.write(Liftpos);
        delay(15);}
 if( Position == 0) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, HIGH);
    digitalWrite(MotorBPin2, LOW); 
    
  delay(250);  
 }
 else if( Position == 1) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, HIGH);
    digitalWrite(MotorBPin2, LOW); 
     
  delay(500);
 }
 else if( Position == 2) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 
     
  delay(750);
 }
 else if( Position == 3) 
 {
    digitalWrite(MotorAPin1, HIGH);
    digitalWrite(MotorAPin2, LOW);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 

  delay(500);
 }
 else if( Position == 4) 
 {
    digitalWrite(MotorAPin1, HIGH);
    digitalWrite(MotorAPin2, LOW);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 
  delay(250);
 }
}
}
