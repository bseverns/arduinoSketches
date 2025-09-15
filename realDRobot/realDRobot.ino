//B.Severns
//1/20/14
//Arduino+Motor Shield rev3-drawing robots
//Public Functionary- 2014

int rightButton = A1;
int leftButton = A2;

void setup() {
  //setup channel A
  pinMode(13, OUTPUT);//CH B -- HIGH = forwards and LOW = backwards???
  pinMode(12, OUTPUT);//CH A -- HIGH = forwards and LOW = backwards???
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B
}

void loop() {
  forward();
  if(analogRead(leftButton) == LOW)
  {
    delay(750);
    backwards();
    delay(9000+random(1500, 5000));
    dShift1();
    delay(10000+random(2000, 9000));
  }
  else if(analogRead(rightButton) == LOW)
  {
    delay (750);
    backwards();
    delay(9000+random(1500, 5000));
    dShift2();
    delay(10000+random(2000, 9000));
  }
}

void forward() {
  analogWrite(3, 255); //A full speed
  analogWrite(11, 255);//B full speed
  digitalWrite(12, HIGH);//A forward
  digitalWrite(13, HIGH);//B forward
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake off
}

void backwards() {  
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake off
  analogWrite(3, 255); //A full speed
  analogWrite(11, 255);//B full speed
  digitalWrite(12, LOW);//A backward
  digitalWrite(13, LOW);//B backward
}

void dShift1() {
  analogWrite(3, 255); //A half++ speed
  analogWrite(11, 255);//B half++ speed
  digitalWrite(12, HIGH);//A forward
  digitalWrite(13, LOW);//B backward
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake off
}

void dShift2() {
  analogWrite(3, 255); //A full speed
  analogWrite(11, 255);//B full speed
  digitalWrite(12, LOW);//A forward
  digitalWrite(13, HIGH);//B backward
  digitalWrite(9, LOW);//A brake off
  digitalWrite(8, LOW);//B brake off
}












