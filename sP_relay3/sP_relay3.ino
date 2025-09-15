// control powerSocket
const int powerPin = A0;
const int ledPin = 13;
boolean powerOn = false;

// control LEDs

int incomingByte;  //a variable to read incoming serial data 

void setup() {
  //initiate serial communication:
  Serial.begin(9600);
  Serial.println(F("starting up"));
  
  pinMode(ledPin, OUTPUT);
  pinMode(powerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(powerPin, HIGH);
}  

void loop() {

  // read serial messages
  if (Serial.available() > 0) {

    powerOn = !powerOn;
    
    if (powerOn) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(powerPin, LOW);
      Serial.println(F("ON"));
    } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(powerPin, HIGH);
      Serial.println(F("OFF"));
    }      
    
    //read all the bytes in the serial buffer (we don't care about the contents of the data)
    while (Serial.available()) incomingByte = Serial.read();
  }                       
}
