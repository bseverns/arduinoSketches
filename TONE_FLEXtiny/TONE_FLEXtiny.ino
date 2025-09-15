#include <Tone.h>
Tone tone1;
const int LED = 0;

const int FLEX_PIN = 2;

void toggle_led(){
  int val = analogRead(FLEX_PIN);
  digitalWrite(LED, !digitalRead(LED));
  delay(val/4);
}

void setup() {
  tone1.begin(1);
  pinMode(FLEX_PIN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  int val = analogRead(FLEX_PIN);
 
  if (val >= 810) {
   for( int i = 1500; i <= 1500; i++ ) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(1250);
   }
  tone1.stop();

  }
  else if (val <= 809) { 
     
    tone1.play(1200 - val);

  toggle_led();
  }
}
