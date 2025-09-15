    #include <AF_Wave.h>
    #include <avr/pgmspace.h>
    #include <util.h>
    #include <wave.h>

    #define DEBOUNCE 100

    #define swPin 14

    AF_Wave card;
    File f;
    Wavefile wave;

    void setup() {
      // set up serial port
      Serial.begin(9600);

      // set up waveshield pins
      pinMode(2, OUTPUT);
      pinMode(3, OUTPUT);
      pinMode(4, OUTPUT);
      pinMode(5, OUTPUT);

      // enable pull-up resistors on
      // switch pins (analog inputs)
      digitalWrite(14, HIGH);
      digitalWrite(15, HIGH);
      digitalWrite(16, HIGH);
      digitalWrite(17, HIGH);
      digitalWrite(18, HIGH);
      digitalWrite(19, HIGH);

      // open memory card
      if (!card.init_card()) {
        putstring_nl("Card init failed!"); return;
      }
      if (!card.open_partition()) {
        putstring_nl("No partition!"); return;
      }
      if (!card.open_filesys()) {
        putstring_nl("Couldn't open filesys"); return;
      }
      if (!card.open_rootdir()) {
        putstring_nl("Couldn't open dir"); return;
      }
    }

    void loop() {   
      switch (check_switches()) {
        case 1:
          playcomplete("SOUND001.WAV");
          break;
        case 2:
          playcomplete("SOUND002.WAV");
          break;
      }
    }

    byte check_switches()
    {
      static byte previous[6];
      static long time[6];
      byte reading;
      byte pressed;
      byte index;

      for (byte index = 0; index < 6; ++index) {
        reading = digitalRead(14 + index);
        if (reading == LOW && previous[index] == HIGH && millis() - time[index] > DEBOUNCE)
        {
          // switch pressed
          time[index] = millis();
          pressed = index + 1;
          break;
        }
        previous[index] = reading;
      }
      // return switch number (1 - 6)
      return (pressed);
    }

    void playcomplete(char *name) {
      playfile(name);
      while (wave.isplaying);
      card.close_file(f);
    }

    void playfile(char *name) {
      // stop any file already playing
      if (wave.isplaying) {
        wave.stop();
        card.close_file(f);
      }

      f = card.open_file(name);
      if (f && wave.create(f)) {
        wave.play();
      }
    }

