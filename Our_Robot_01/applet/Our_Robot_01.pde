// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>


int MotorAPin1 = 5;
int MotorAPin2 = 4;
int MotorAPWM  = 3;

int MotorBPin1 = 13;
int MotorBPin2 = 12;
int MotorBPWM  = 11;



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

int Index = 0;
unsigned int sensors[4]; // an array to hold sensor values

  int IR1Pin = 0;    // select the input pin for the potentiometer
  int IR1Val = 0;
  int IR1prevVal = 0;
  int onTime  = 100;
  int offTime = 100;
  
  int Position = 0;
  int LSCalLo = 500;
  int LSCalHi = 1100;
  int HighVal = 0;
  int DeadBand = 2;
  
void setup() {

  Serial.begin(9600);  
  
  pinMode(MotorAPin1, OUTPUT);
  pinMode(MotorAPin2, OUTPUT);
  pinMode(MotorAPWM, OUTPUT);
  
  pinMode(MotorBPin1, OUTPUT);
  pinMode(MotorBPin2, OUTPUT);
  pinMode(MotorBPWM, OUTPUT);
  
  digitalWrite(MotorAPWM, HIGH);
  digitalWrite(MotorBPWM, HIGH);

}

void loop() {
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
  
  //analogRead(IR1Pin);    // read the value from the sensor
  
 if( Position == 0) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, HIGH);
    digitalWrite(MotorBPin2, LOW); 
    
//  delay(500);  
 }
 else if( Position == 1) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, HIGH);
    digitalWrite(MotorBPin2, LOW); 
     
//  delay(500);
 }
 else if( Position == 2) 
 {
    digitalWrite(MotorAPin1, LOW);
    digitalWrite(MotorAPin2, HIGH);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 
     
//  delay(500);
 }
 else if( Position == 3) 
 {
    digitalWrite(MotorAPin1, HIGH);
    digitalWrite(MotorAPin2, LOW);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 
//  delay(500);
 }
 else if( Position == 4) 
 {
    digitalWrite(MotorAPin1, HIGH);
    digitalWrite(MotorAPin2, LOW);
    
    digitalWrite(MotorBPin1, LOW);
    digitalWrite(MotorBPin2, HIGH); 
//  delay(500);
 }
}
