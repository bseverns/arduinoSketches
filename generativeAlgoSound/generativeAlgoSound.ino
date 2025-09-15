#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PCM.h>

#define str(s) #s
#define xstr(s) str(s)

//Music
#define SAMPLE_RATE 8000
#define NUMBER_OF_ALGORITHMS 22
#define ALGO_01 t*((t>>12|t>>8)&63&t>>4)
#define ALGO_02 (t*(t>>5|t>>8))>>(t>>16)
#define ALGO_03 t*((t>>9|t>>13)&25&t>>6)
#define ALGO_04 t*(t>>11&t>>8&123&t>>3)
#define ALGO_05 t*(t>>8*(t>>15|t>>8)&(20|(t>>19)*5>>t|t>>3))
#define ALGO_06 (int)(t/17*t*t+t)%127|t>>4|t>>5|t%127+(t>>16)|t
#define ALGO_07 (t>>6|t|t>>(t>>16))*10+((t>>11)&7)
#define ALGO_08 (t|(t>>9|t>>7))*t&(t>>11|t>>9)
#define ALGO_09 t*5&(t>>7)|t*3&(t*4>>10)
#define ALGO_10 (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6)
#define ALGO_11 ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t))
#define ALGO_12 ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6)
#define ALGO_13 (t*5&t>>7)|(t*3&t>>10)
#define ALGO_14 (int)(t/1e7*t*t+t)%127|t>>4|t>>5|t%127+(t>>16)|t
#define ALGO_15 ((t/2*(15&(0x234568a0>>(t>>8&28))))|t/2>>(t>>11)^t>>12)+(t/16&t&24)
#define ALGO_16 (t&t%255)-(t*3&t>>13&t>>6)
#define ALGO_17 t>>4|t&(t>>5)/((t>>7-(t>>15)&-t>>7-(t>>15))!=0?(t>>7-(t>>15)&-t>>7-(t>>15)):1)
#define ALGO_18 ((t*("36364689"[t>>13&7]&15))/12&128)+(((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127)
#define ALGO_19 (t*9&t>>4|t*5&t>>7|t*3&t/1024)-1
#define ALGO_20 ((t*(t>>12)&(201*t/100)&(199*t/100))&(t*(t>>14)&(t*301/100)&(t*399/100)))+((t*(t>>16)&(t*202/100)&(t*198/100))-(t*(t>>17)&(t*302/100)&(t*298/100)))
#define ALGO_21 t*(t^t+(t>>15|1)^(t-1280^t)>>10)
//ALGO_22

//Set pin numbers
const short speakerPin = 11;
const short buttonPin = 1;
const short encoderPinA = 2;
const short encoderPinB = 3;

//Display settings
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
bool drawn = false;
bool drawing = false;

//Encoder
volatile unsigned int encoderPos = 0;
unsigned int lastReportedPos = 1;
static boolean rotating = false;
bool aSet = false;
bool bSet = false;
bool pressed = false;
int select = 1;

//Math
short count = 0;
unsigned long t = 0;
int num = 4;
uint8_t array[128];

void checkEncoder()
{
  rotating = true;  //reset debouncer
  
  if(lastReportedPos != encoderPos)
  {
    lastReportedPos = encoderPos;
    //tone(speakerPin, 100, 25);
  }
  
  if(digitalRead(buttonPin) == LOW)
  {
    pressed = true;
    delay(500);
  }
}

void doEncoderA()
{
 
  //debounce
  if(rotating) delay(1);

  //test transition
  if(digitalRead(encoderPinA) != aSet)
  {
    aSet = !aSet;

    if(aSet && !bSet)
    {
      encoderPos ++;
      if(select<NUMBER_OF_ALGORITHMS)
      {
        select++;
        t = 0;
        drawn = false;
      }
    }

    rotating = false; //no more debouncing until loop repeats
  }
}

void doEncoderB()
{
 
  if(rotating)
    delay(1);
  if(digitalRead(encoderPinB) != bSet)
  {
    bSet = !bSet;

    if(bSet && !aSet)
    {
      encoderPos--;
      if(select>1)
      {
        select--;
        t = 0;
        drawn = false;
      }
    }

    rotating = false;
  }
}

void setup()
{
  
  //Serial communication
  //Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  //Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Initialize with I2C addr 0x3C
  display.clearDisplay();
  display.display();

  //Set pin modes
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  //encoder interrupts
  attachInterrupt(digitalPinToInterrupt(2), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), doEncoderB, CHANGE);

  //Sound setup
  soundSetup();
  
}

void loop()
{   
  checkEncoder();

  if(!drawn)
  {
    display.clearDisplay();
    showAlgorithm();
    display.display();
    drawn = true;
  }

  if(count == 128)
  {    
    drawing = true;
    //Serial.print("Drew dot ");
    //Serial.println(t);
    display.fillRect(0,16,128,48,0);
    drawDot();
    count = 0;
    display.display();
    drawing = false;
  }
  
  /*if(pressed)
  {
    drawDisplay = !drawDisplay;
    pressed = false;
  }*/
}

unsigned long doAlgorithm()
{
  unsigned long x, y = 0;
  switch(select)
  {
    case 1:
      return ALGO_01;
    case 2:
      return ALGO_02;
    case 3:
      return ALGO_03;
    case 4:
      return ALGO_04;
    case 5:
      return ALGO_05;
    case 6:
      return ALGO_06;
    case 7:
      return ALGO_07;
    case 8:
      return ALGO_08;
    case 9:
      return ALGO_09;
    case 10:
      return ALGO_10;
    case 11:
      return ALGO_11;
    case 12:
      return ALGO_12;
    case 13:
      return ALGO_13;
    case 14:
      return ALGO_14;
    case 15:
      return ALGO_15;
    case 16:
      return ALGO_16;
    case 17:
      return ALGO_17;
    case 18:
      return ALGO_18;
    case 19:
      return ALGO_19;
    case 20:
      return ALGO_20;
    case 21:
      return ALGO_21;
    case 22:
      y = t&16383;
      x = t*"6689"[t>>16&3]/24&127;
      return (((unsigned long)3e3/y)&1)*35+x*y/4e4+((t>>8^t>>10|t>>14|x)&63);
    default:
      break;
  }
}

ISR(TIMER1_COMPA_vect)
{
  OCR2A = doAlgorithm();
  if(drawing == false && count < 128)
  {
    array[count] = doAlgorithm();
    count++;
  }
  ++t;
}

void showAlgorithm()
{
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0,0);
  display.print("Algorithm: ");
  display.print(select);
}

void drawDot()
{
  for(short i = 0; i < 128; ++i)
  {
    for(short j = 0; j < 8; ++j)
    {
      //display.drawPixel(i, 16 + (47 - ((array[i] / 255.0) * 47)), 1);
      display.drawFastVLine(i, 16 + (47 - ((array[i] / 255.0) * 47)), 47, 1);
    }
  }
}

void soundSetup()
{
  pinMode(speakerPin, OUTPUT);
  
  // Set up Timer 2 to do pulse width modulation on the speaker
  // pin.
  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  
  // Set fast PWM mode  (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);
  
  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  
  // No prescaler (p.158)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  
  // Set initial pulse width to the first sample.
  OCR2A = B00000000;
  
  // Set up Timer 1 to send a sample every interrupt.
  cli();
  
  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  
  // No prescaler (p.134)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  
  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000
  
  // Enable interrupt when TCNT1 == OCR1A (p.136)
  TIMSK1 |= _BV(OCIE1A);

  sei();
}
