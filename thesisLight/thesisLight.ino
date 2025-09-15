#define PIN_RELAY 7

boolean relayOn = false;

void setup() { 
  Serial.begin(9600); // open serialport
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW); //turns the light off originally at the start of the program
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
    Serial.println("2");
  } 
  
  delay(900); // wait for 15 minutes
}
