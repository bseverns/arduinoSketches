// glitchamin
// simple theremin synth with killswitch
// Jacob Peck (a.k.a. ~suspended-chord, http://suspended-chord.info)
// adapted from http://www.adafruit.com/blog/2009/05/19/piezo-with-an-arduino-photoresistor/

// circuit:
// pin d9: pin > +buzzer- > gnd
// pin d10: pin > switch > 5v, switch > 10kOhm resistor > gnd (optional)
// pin a0: 5v > cds > pin, pin > 10kOhm resistor > gnd
// pin a1: 5v > pot, pot > pin, pot > gnd
// pin a2: 5v > pot, pot > pin, pot > gnd

//#define DEBUG // uncomment to get serial output, but it kills the speed of the
                // algorithm, making for a very bad cycle rate
#define KILLSWITCH // comment to disable killswitch code

int photosensorPin = 0;
int cutterpotPin = 1;
int cyclepotPin = 2;
int tonePin = 9;

#ifdef KILLSWITCH
int killswitchPin = 10;
#endif

int val = 0;
int cut = 0;
int cycle = 0;
int kill = 0;

void setup() {
  pinMode(piezoPin, OUTPUT);
  
#ifdef KILLSWITCH  
  pinMode(killswitchPin, INPUT);
#endif

#ifdef DEBUG  
  Serial.begin(115200);
#endif

}

void loop() {
  digitalWrite(piezoPin, LOW);
  val = analogRead(photosensorPin);
  cut = analogRead(cutterpotPin);
  cycle = analogRead(cyclepotPin);

#ifdef KILLSWITCH
  kill = digitalRead(killswitchPin);
#endif  
  
#ifdef DEBUG  
  Serial.print("CdS: ");
  Serial.print(val);
  Serial.print("\tCutter: ");
  Serial.print(cut);
  Serial.print("\tCycle: ");
  Serial.print(cycle);
#ifdef KILLSWITCH  
  Serial.print("\tKill: ");
  Serial.println(kill);
#else
  Serial.println("");
#endif // killswitch
#endif // debug

  val = val / map(cut, 0, 1023, 15, 2);
  cycle = map(cycle, 0, 1023, 1, 1000);

#ifdef KILLSWITCH
 if (!kill) { // if not killed
#endif
    for( int i=0; i<cycle; i++ ) {  // play for cycle pot's reading
      digitalWrite(piezoPin, HIGH);
      delayMicroseconds(val);
      digitalWrite(piezoPin, LOW);
      delayMicroseconds(val);
    }
#ifdef KILLSWITCH
  }
#endif  
}
