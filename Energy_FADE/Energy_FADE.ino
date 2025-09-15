int ledControl = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
float fadeValue;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(ledControl, OUTPUT);
  fadeValue = 1;
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  analogWrite(ledControl, brightness);

  // change the brightness for next time through the loop:


  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
    // sets the value (range from 0 to 255):
    analogWrite(ledControl, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    brightness = brightness + fadeValue;
    // reverse the direction of the fading at the ends of the fade:
    if (fadeValue == 255) {
      delay(10000);
    }
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 1) {
    // sets the value (range from 0 to 255):
    analogWrite(ledControl, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    brightness = brightness + fadeValue;
    // reverse the direction of the fading at the ends of the fade:
    if (brightness == 0) {
      delay(10000);
    }
  }
}



