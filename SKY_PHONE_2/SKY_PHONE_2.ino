

/***************************************************
  As this is not a redistribution, the text did not need to be kept.
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

unsigned long sleepTime = 600000; //ten minutes of music
unsigned long playTime = 0;

int vol = 15; //louder = lower

boolean playingMusic;

Adafruit_VS1053_FilePlayer musicPlayer =
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (!musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(vol, vol);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  musicPlayer.startPlayingFile("/track001.mp3");
  //musicPlayer.sineTest(0x44,500); //make a tone to indicate that switch is working
  playingMusic = true; //we are go in
  musicPlayer.GPIO_pinMode(1, INPUT);
}

void loop() {
  for (uint8_t i = 0; i < 8; i++) {

    if (musicPlayer.GPIO_digitalRead(1) == HIGH) {
      Serial.println("stopped");
      musicPlayer.pausePlaying(true); //start pausing
      playingMusic = false;//stopppppppped
    }

    if (musicPlayer.GPIO_digitalRead(1) == LOW) {
      Serial.println("play");
      musicPlayer.pausePlaying(false); //stop pausing
      playingMusic = true; //we are playing
      playTime = millis(); //what's the COUNT!
    }
    delay(5);
  }
  if (playTime > sleepTime) {
   setup();
  }
}
