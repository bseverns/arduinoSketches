#define BUTTON1_PIN               2  // Button 1
#define BUTTON2_PIN               3  // Button 2

#define DEFAULT_LONGPRESS_LEN    25  // Min nr of loops for a long press
#define DELAY                    20  // Delay per loop in ms

//////////////////////////////////////////////////////////////////////////////

enum { EV_NONE=0, EV_SHORTPRESS, EV_LONGPRESS };

//////////////////////////////////////////////////////////////////////////////
// Class definition

class ButtonHandler {
  public:
    // Constructor
    ButtonHandler(int pin, int longpress_len=DEFAULT_LONGPRESS_LEN);

    // Initialization done after construction, to permit static instances
    void init();

    // Handler, to be called in the loop()
    int handle();

  protected:
    boolean was_pressed;     // previous state
    int pressed_counter;     // press running duration
    const int pin;           // pin to which button is connected
    const int longpress_len; // longpress duration
};

ButtonHandler::ButtonHandler(int p, int lp)
: pin(p), longpress_len(lp)
{
}

void ButtonHandler::init()
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // pull-up
  was_pressed = false;
  pressed_counter = 0;
}

int ButtonHandler::handle()
{
  int event;
  int now_pressed = !digitalRead(pin);

  if (!now_pressed && was_pressed) {
    // handle release event
    if (pressed_counter < longpress_len)
      event = EV_SHORTPRESS;
    else
      event = EV_LONGPRESS;
  }
  else
    event = EV_NONE;

  // update press running duration
  if (now_pressed)
    ++pressed_counter;
  else
    pressed_counter = 0;

  // remember state, and we're done
  was_pressed = now_pressed;
  return event;
}

//////////////////////////////////////////////////////////////////////////////

// Instantiate button objects
ButtonHandler button1(BUTTON1_PIN);
ButtonHandler button2(BUTTON2_PIN, DEFAULT_LONGPRESS_LEN*2);

void setup()
{
  Serial.begin(9600);

  // init buttons pins; I suppose it's best to do here
  button1.init();
  button2.init();
}

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

