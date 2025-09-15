//B.Severns
//1/20/14
//Arduino+Motor Shield rev3-drawing robots

int Button1_Pin = A1;

boolean block;

void setup() {
  //setup channel A
  pinMode(13, OUTPUT);//CH B -- HIGH = forwards and LOW = backwards???
  pinMode(12, OUTPUT);//CH A -- HIGH = forwards and LOW = backwards???
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B
}

void loop() {
  forward();
  block = false;

  if(analogRead(Button1_Pin) == LOW)
  {
    delay(750);
    backwards();
    delay(1000);
    turn();
    delay(1000);
  }
}

void forward() {
  digitalWrite(12, HIGH);//A forward
  digitalWrite(13, HIGH);//B forward
  analogWrite(3, 100); //A half speed
  analogWrite(11, 100);//B half speed
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake off
}

void backwards() {
  digitalWrite(9, HIGH);//A brake on
  digitalWrite(8, HIGH);//B brake on
  digitalWrite(12, LOW);//A forward
  digitalWrite(13, LOW);//B forward
  analogWrite(3, 127); //A half speed
  analogWrite(11, 127);//B half speed
}

void turn() {
  analogWrite(3, 150); //A half++ speed
  analogWrite(11, 150);//B half++ speed
  digitalWrite(12, HIGH);//A forward
  digitalWrite(13, LOW);//B forward
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake oon
}










