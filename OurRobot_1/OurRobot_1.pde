#include <AFMOTOR.h>

AF_DCMotor motorL(3,MOTOR12_64KHZ); // create motor #1, 64Khz pwm
AF_DCMotor motorR(4,MOTOR12_64KHZ); // create motor #2, 64Khz pwm

int IR1Pin = 0;  // select the pin for the potentiometer
int IR1Val = 0;
int IR1prevVal = 0
int onTime = 100;
int offTime = 100;

void setup() {
  
  motorL.setSpeed(100);  // set the speed to 100/255
  motorR.setSpeed(100);  // set the speed to 100/255
  
}

void loop() {
  
  IR1Val = analogRead(IR1Pin);  // read the value from the sensor
  
  if(IR1Val< 100)
  {
    motorL.run(FORWARD);  // turn it on going forward
    motorR.run(FORWARD);  // turn it on going forward
  }
  else
  {
    motorL.run(RELEASE);  // stop
    motorR.run(RELEASE);  // stop
  }
}
