const int PIN_RELAY_POS = A4;
const int PIN_RELAY_NEG = A2;     

boolean relayOn = false;

void setup() { 
  Serial.begin(9600); // open serialport
  pinMode(PIN_RELAY_POS, OUTPUT);
  digitalWrite(PIN_RELAY_POS, HIGH); //turns the light on originally at the start of the program
  pinMode(PIN_RELAY_NEG, OUTPUT);
  digitalWrite(PIN_RELAY_NEG, HIGH);
}

void loop() {

  // read from serial and respond accordingly 
  int onTime = random(2);

  if(onTime < 0.5) {
    digitalWrite(PIN_RELAY_POS, LOW);
    relayOn = false;
    Serial.println("1");
    if (relayOn = false) {
      digitalWrite(PIN_RELAY_NEG, HIGH);
    }
  } 
  else if(onTime > 0.5) {
    digitalWrite(PIN_RELAY_POS, HIGH);
    relayOn = true;
    Serial.println("2");
  } 
  if (relayOn = true) {
      digitalWrite(PIN_RELAY_NEG, LOW);
      delay(150); // sleep, you're all tuckered out
    }
}
