const int FLEX_PIN = 2;
const int motorPin1  = 0;
const int motorPin2  = 1;



void setup() {

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(FLEX_PIN, INPUT);



}
void loop() {

  int x = analogRead(FLEX_PIN);
  int speed = map(x, 300, 200, 0, 255);

  if (speed >= 0 && speed <= 255)
  {
    analogWrite(motorPin1, speed);
  }
  delay(100);
}
