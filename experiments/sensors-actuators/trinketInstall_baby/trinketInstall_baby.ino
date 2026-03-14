//Baby2.0
//Adafruit Trinket + Baby Guts from Erik Tinberg
//B.Severns
//July 2020 - Quarantine lifted - Masks on - Florida dying

const int echoPin = 2; //pin#
const int pingPin = 1; //pin#
const int babyTalk = 0; //pin#
const int farThresh = 24; //distance in inches
const int nearThresh = 9; //distance in inches
bool pres = false; //noise double check vs. false positive

void setup() {
  pinMode(echoPin, OUTPUT); //pin 2
  pinMode(pingPin, INPUT); //pin 1
  pinMode(babyTalk, OUTPUT); //pin 0
  pinMode(LED_BUILTIN, OUTPUT); //pin 13
}

//adjust volume on baby-amplifier for distortion possibility
void loop() {
  //variables
  long duration, inches;

  //Echo OUT
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(echoPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(echoPin, LOW);

  //PING IN
  duration = pulseIn(pingPin, HIGH);

  //math
  inches = microsecondsToInches(duration);

  //are they in the zone?
  if (inches < farThresh && inches > nearThresh) { //on
    pres = true; //but check again
  }
  if (inches < farThresh && inches > nearThresh) { //on
    if (pres == true) { //go
      digitalWrite(babyTalk, HIGH);   // turn the BABY on
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
  }
  else { //off
    pres = false; //no one here, yo
    digitalWrite(babyTalk, LOW); //baby off
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  //repeat
}

//MATHHH
long microsecondsToInches(long microseconds) {
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
