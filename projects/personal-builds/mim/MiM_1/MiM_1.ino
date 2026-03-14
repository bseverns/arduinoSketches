/*
Receiving '6', '12', '25', or '45'
 from Serial motion detector-- MiM
 */

// give the motor control pins names:
const int pwmA = 3;
const int brakeA = 9;
const int dirA = 12;

//serial in
int input = 0;


void setup() {

  //Setup Channel A
  pinMode(dirA, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakeA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(dirB, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakeB, OUTPUT); //Initiates Brake Channel A pin

  Serial.begin(9600);
}

void loop() {
  input = Serial.read();
  if (input == '6') { 
  //Motor A forward @ full speed
  digitalWrite(dirA, HIGH); //Establishes forward direction of Channel A
  digitalWrite(brakeA, LOW); //Disengage the Brake for Channel A
  analogWrite(pwmA, 6); //Spins the motor on channel A at full speed

  }
  else if (input == '12') {
      //Motor A forward @ full speed
  digitalWrite(dirA, HIGH); //Establishes forward direction of Channel A
  digitalWrite(brakeA, LOW); //Disengage the Brake for Channel A
  analogWrite(pwmA, 12); //Spins the motor on channel A at full speed
  }
    else if (input == '25') {
      //Motor A forward @ full speed
  digitalWrite(dirA, HIGH); //Establishes forward direction of Channel A
  digitalWrite(brakeA, LOW); //Disengage the Brake for Channel A
  analogWrite(pwmA, 25); //Spins the motor on channel A at full speed
  }
    else if (input == '45') {
      //Motor A forward @ full speed
  digitalWrite(dirA, HIGH); //Establishes forward direction of Channel A
  digitalWrite(brakeA, LOW); //Disengage the Brake for Channel A
  analogWrite(pwmA, 45); //Spins the motor on channel A at full speed
  }
}


