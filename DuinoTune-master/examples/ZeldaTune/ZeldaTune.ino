
#include <DuinoTune.h>
#include <zelda.h>
#include "Arduino.h"

void setup()
{
  initTinyTune();
  playSong(&zelda);
}

void loop()
{
}
