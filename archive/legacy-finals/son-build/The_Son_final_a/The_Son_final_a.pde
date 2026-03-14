// Arduino Controlled Robot
// Pin  Arduino  Description
//  D01  Not Used - Serial
//  D02  Not Used - Serial
//  D03  Output - Left Motor PWM
//  D04  Output - Left Motor Forward
//  D05  Output - Left Motor Reverse
//  D06  Input  - Left Wall Switch
//  D07  Not Used - Serial
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
//  A5  Not Used

// Arduino servo library
#include <Servo.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

// Pololu QTR line sensor library
#include <PololuQTRSensors.h>

// Create LiftServo, an instance of the Servo object
Servo LiftServo;

// Create qtr, an instance of the line sensor object, use 5 pins on analog pins 0-4
PololuQTRSensorsAnalog qtr((unsigned char[]) {  0, 1, 2, 3, 4}, 5); 

// Digital pins to use for left motor
int MotorAPin1 = 5;  // Forward
int MotorAPin2 = 4;  // Reverse
int MotorAPWM  = 3;  // Speed

// Digital pins to use for right motor
int MotorBPin1 = 13;  //Forward
int MotorBPin2 = 12;  //Reverse
int MotorBPWM  = 11;  //Speep

// Digital pin for PIR sensor
int pirPin = 8;

// Digital pin for Ping sensor
int pingSignal = 10;

int Liftpos = 0;
int IsUp = 0;
int calibrationTime = 30; // PIR calibration time in seconds
long unsigned int lowIn;
long unsigned int pause = 5000;
unsigned long tooClose = 20.0;

boolean lockLow = true;
boolean takeLowTime;

// function to get distance from the ping sensor
unsigned long ping(){
  unsigned long echo = 0;
  unsigned long pingValue = 0;
  pinMode(pingSignal, OUTPUT);      // Switch signalpin to output
  digitalWrite(pingSignal, LOW);    // Send low pulse
  delayMicroseconds(2);             // Wait for 2 microseconds
  digitalWrite(pingSignal, HIGH);   // Send high pulse
  delayMicroseconds(5);             // Wait for 5 microseconds
  digitalWrite(pingSignal, LOW);    // Hold off
  pinMode(pingSignal, INPUT);       // Switch signalpin to input
  digitalWrite(pingSignal, HIGH);   // Turn on pullup resistor
  echo = pulseIn(pingSignal, HIGH); //Listen for echo
  pingValue = (echo / 58.138);      //convert to CM
  return pingValue;
}

// Calibrate PIR sensor, PIR does this itself when powered-up.
// Delay allows time to calibrate.
void calibratePIR()
{
  Serial.print("calibrating PIR sensor ");
  for (int i = 0; i < calibrationTime; i++)
  {
    Serial.print(".");
    delay (1000);
  }

  Serial.println("done");
  Serial.println("Sensor Active");
  delay(50);

  return;
}    

void calibrateLineSensors()
{
  Serial.print("calibrating Line Sensors");
  // start line sensor calibration phase and move the sensors over both  
  // reflectance extremes they will encounter in your application:  
  int q;  
  for (q = 0; q < 250; q++)  // make the calibration take about 5 seconds  
    qtr.calibrate();
    {
      Serial.print(".");
    }
    delay(1000);  
  
  return;
}  

void leftMotorStop()
{  
  digitalWrite(MotorAPin1, LOW);
  digitalWrite(MotorAPin2, LOW);
}

void rightMotorStop()
{  
  digitalWrite(MotorBPin1, LOW);
  digitalWrite(MotorBPin2, LOW);
}

void leftMotorForward()
{  
  digitalWrite(MotorAPin1, LOW);
  digitalWrite(MotorAPin2, HIGH);
}

void rightMotorForward()
{  
  digitalWrite(MotorBPin1, LOW);
  digitalWrite(MotorBPin2, HIGH);
}

void leftMotorReverse()
{  
  digitalWrite(MotorAPin1, HIGH);
  digitalWrite(MotorAPin2, LOW);
}

void rightMotorReverse()
{  
  digitalWrite(MotorBPin1, HIGH);
  digitalWrite(MotorBPin2, LOW);
}

void robotDown()
{
  // Set lift servo initial position
  for (Liftpos = 0; Liftpos > -10; Liftpos -=1)
  {
    LiftServo.write(Liftpos);
    delay(15);
  }
}

void robotUp()
{
  // Set lift servo initial position
  for (Liftpos = 0; Liftpos < 10; Liftpos +=1)
  {
    LiftServo.write(Liftpos);
    delay(15);
  }
}

void setup() 
{
  Serial.begin(9600);  // set serial speed to 9600 baud

  // Set PIR pin to INPUT
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  // Set Ping pin to OUTPUT
  pinMode(pingSignal,OUTPUT);

  // Motor digital pins to OUTPUTs
  pinMode(MotorAPin1, OUTPUT);
  pinMode(MotorAPin2, OUTPUT);
  pinMode(MotorAPWM,  OUTPUT);

  pinMode(MotorBPin1, OUTPUT);
  pinMode(MotorBPin2, OUTPUT);
  pinMode(MotorBPWM, OUTPUT);

  // Set motors speed to maximum
  digitalWrite(MotorAPWM, HIGH);
  digitalWrite(MotorBPWM, HIGH);

  // Attach lift servo to digital pin 9
  LiftServo.attach(9);

  robotDown();

  Serial.println("Calibrate PIR");  
  calibratePIR();
  
  Serial.println("Calibrate Line Sensors");
  calibrateLineSensors();

  robotUp();
  delay(1000);
  
  robotDown();
  delay(1000);

  leftMotorForward();
  delay(1000);
  
  leftMotorReverse();
  delay(1000);
  
  rightMotorForward();
  delay(1000);
  
  rightMotorReverse();
  delay(1000);
  
  leftMotorStop();
  rightMotorStop();
}


// ***** Main control loop ******
void loop()
{
  unsigned int sensors[5];
  int x = 0;

  x = ping();
  Serial.print("Start Loop: ");
  Serial.println(x);
  delay(250); //delay 1/4 seconds.
  
  //makes sure we wait for a PIR transition to LOW before any further output is made:
  if(digitalRead(pirPin) == HIGH)
  {
    if(lockLow)
    {
      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec");
      delay(50);
      
      leftMotorForward;
      rightMotorForward;
      robotUp;
    }
    takeLowTime = true;
  }

  if(digitalRead(pirPin) == LOW)
  {
    if(takeLowTime)
    {
      lowIn = millis();          //save the time of the transition  from high to LOW
      takeLowTime = false;       //make sure this is only done at  the start of a LOW phase
    }
    //if the sensor is low for more than the given pause,
    //we assume that no more motion is going to happen
    if(!lockLow && millis() - lowIn > pause)
    {
      //makes sure this block of code is only executed again after
      //a new motion sequence has been detected
      lockLow = true;
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);

      leftMotorStop;
      rightMotorStop;
      robotDown;

      if(IsUp == 0)
      {
        robotUp;

        IsUp = 1;
        delay(10000);
      }
    }
    else {
      leftMotorStop;
      rightMotorStop;
      robotDown;

      IsUp = 0;
    }
    if (IsUp == 1)
    {
      if (ping() <= tooClose)   // stop if too close
      { 
        leftMotorReverse;     // back up
        rightMotorReverse;
        delay (1000);
        leftMotorForward;     //pivot right 
        rightMotorStop;
        delay (250);          
        leftMotorStop;
      }

      // get calibrated sensor values returned in the sensors array, along with the line position  
      // position will range from 0 to 2000, with 1000 corresponding to the line over the middle sensor  
      int position = qtr.readLine(sensors);  

  Serial.print("Position: ");
  Serial.println(position);
  Serial.print("Sensors: ");
  Serial.print(sensors[0]);
  Serial.print(", ");
  Serial.print(sensors[1]);
  Serial.print(", ");
  Serial.print(sensors[2]);
  Serial.print(", ");
  Serial.print(sensors[3]);
  Serial.print(", ");
  Serial.print(sensors[4]);
  Serial.println(" ");
  
      // if all sensors see very low reflectance, take some appropriate action for this situation  
     if (sensors[0] > 750 && sensors[1] > 750 && sensors[2] > 750 && sensors[3] > 750 && sensors[4] > 750)  
      {  
        // edge detected, full stop
        leftMotorStop;
        rightMotorStop;
        delay(300);
        // pivot
        leftMotorForward;
        delay(300);
        leftMotorStop;
        return;  
      }  
      // compute our "error" from the line position.  We will make it so that the error is zero when  
      // the middle sensor is over the line, because this is our goal.  Error will range from  
      // -1000 to +1000.  If we have sensor 0 on the left and sensor 4 on the right,  a reading of -1000  
      // means that we see the line on the left and a reading of +1000 means we see the line on  
      // the right.  
      int error = position - 1000;  
      
      if (error < -500)      // the line is on the left  
      {
        leftMotorStop();
        rightMotorForward();
      } 
      else if(error > 500)     // the line is on the right  
      {
        rightMotorStop();
        leftMotorForward();
      }
      else
      {
        leftMotorForward();
        rightMotorForward();
      }
    }
  }
}
