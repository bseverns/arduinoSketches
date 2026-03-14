int ledControl = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(ledControl, OUTPUT);
  fadeAmount = 1;
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  analogWrite(ledControl, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // wait for 30 milliseconds to see the dimming effect
  delay(30);

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    delay(1000);
    //random(15000, 40000)
    fadeAmount = -fadeAmount;
  }
}
