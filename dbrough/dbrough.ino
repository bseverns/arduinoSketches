//Slow motors for exhibition-robots
//

#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_8KHZ);
AF_DCMotor motor2(2, MOTOR12_2KHZ);

//////////////////////////////////////////////////////////////////////////////
int x;

int Button1_Pin = A1; //switch code from drawBots tab
//////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  Serial.println("start");
}

void loop()
{
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

void backwards()
{
  Serial.println("back");
  motor1.setSpeed(175);
  motor2.setSpeed(175);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);

}


void forward()
{
  motor1.setSpeed(125);
  motor2.setSpeed(125);
  Serial.println("forward");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void turn()
{
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  Serial.println("turn");
  for (int turn = 0; turn < (400+random(800)); turn++) {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
}






