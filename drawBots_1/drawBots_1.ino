
void print_event(const char* button_name, int event)
{
  if (event)
    Serial.print(button_name);
  Serial.print(".SL"[event]);
}


void loop()
{
  // handle button
  int event1 = button1.handle();
  int event2 = button2.handle();

  // do other things
  print_event("1", event1);
  print_event("2", event2);

  // add newline sometimes
  static int counter = 0;
  if ((++counter & 0x1f) == 0)
    Serial.println();

  delay(DELAY);
}

