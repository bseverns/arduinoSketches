#define PIN_RELAY 7

#include "WProgram.h"
void setup();
void loop();
boolean relayOn = false;

void setup() { 
  Serial.begin(9600); // open serialport
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); //turns the light on originally at the start of the program
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
  } else if(inByte == 255 && relayOn == false) {
    digitalWrite(PIN_RELAY, HIGH);
    relayOn = true;
  } 
  delay(50); // sleep, you're all tuckered out
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

