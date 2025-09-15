#define PIN_RELAY 7

boolean relayOn = false;

void setup() { 
  Serial.begin(9600); // open serialport
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); //turns the light on originally at the start of the program
  delay(500);
  digitalWrite(PIN_RELAY, LOW);
}

void loop() {
  // read from serial and respond accordingly 
  int inByte = -1;

  while(Serial.available() > 0) {
    inByte = Serial.read();   
  }
  
  if(inByte == 0 && relayOn == true) {
    digitalWrite(PIN_RELAY, LOW);
    relayOn = false;
    Serial.println("1");
  } 

  else if(inByte == 255 && relayOn == false) {
    digitalWrite(PIN_RELAY, HIGH);
    relayOn = true;
    delay(85);
    Serial.println("2");
  } 
  
  delay(50); // sleep, you're all tuckered out
}


