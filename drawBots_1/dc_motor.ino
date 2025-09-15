//Need an H bridge for this sketch.
//Slow motors for exhibition-robots
//

#define BUTTON1_PIN               2  // Button 1
#define BUTTON2_PIN               3  // Button 2

#define DEFAULT_LONGPRESS_LEN    25  // Min nr of loops for a long press
#define DELAY                    20  // Delay per loop in ms

//////////////////////////////////////////////////////////////////////////////

enum { 
  EV_NONE=0, EV_SHORTPRESS, EV_LONGPRESS };


int Enable1 = 10;
int Enable2 = 11;
int M1 = 2;
int M2 = 3;
int M3 = 4;
int M4 = 5;
int Switch = 6; //switch code from drawBots tab


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

ButtonHandler:
:
ButtonHandler(int p, int lp)

: 
pin(p), longpress_len(lp)
{
}

void ButtonHandler::
init()
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
  pinMode(Enable1, OUTPUT);
  pinMode(Enable2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  digitalWrite(Enable1, HIGH);
  digitalWrite(Enable2, HIGH);
  Serial.begin(9600);

  // init buttons pins; I suppose it's best to do here
  button1.init();
  button2.init();
}

void loop()
{
  if(digitalRead(Switch) == HIGH)
  {
    backwards();
    delay(500);
    turn();
    delay(500);
  }

  else
  {
    forward();
  }
}

void backwards()
{
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);

}


void forward()
{

  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}

void turn()
{
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, HIGH);
}

