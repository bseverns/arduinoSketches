//Need an H bridge for this sketch.
//Slow motors for exhibition-robots
//

int Enable1 = 10;
int Enable2 = 11;
int M1 = 2;
int M2 = 3;
int M3 = 4;
int M4 = 5;
int Switch = 6; //switch code from drawBots tab

void setup()
{
  pinMode(Enable1, OUTPUT);
  pinMode(Enable2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  digitalWrite(Enable1, HIGH);
  digitalWrite(Enable2, HIGH);
}

void loop()
{
  if(digitalRead(Switch) == HIGH)
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
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);

}


void forward()
{

  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}

void turn()
{
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
}
