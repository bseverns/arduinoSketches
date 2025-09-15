/* This section of code will let you add sound to your project

We are giving the notes names (like C, D, E, etc...) and values
This lets us decide what the notes will be ahead of time.

The values can be any whole number between 100 and 4000
(high number here = high note later)

We've given you an example set up, but we want you to play around
with it and to get comfortable changing the code as you see fit */

int C = 262; 
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int Buzz = 2;
int scale = 2;

//The "void setup() {...." only happens once, when you start your program
void setup() {  
  pinMode(2, OUTPUT);     //initialize the digital pin as an output.  
                          //Pin 13 has an LED connected on most Arduino 
                          //(the kind we're using) boards:

  for (int R=0; R < 3; R++) //this is the code for repeating I chose "r" for 
                            //the number of times for repeating
                            
                            //That means that this next part inside of the { and } will repeat
  {
    tone(2, C);
    delay(1000);
    tone(2, D);
    delay(500);
    tone(2, E);
    delay(250);
  }
  
  //This part only happens once, because there is no repeating "for(..." loop
  tone(2, F);
  delay(100);
  tone(2, G);
  delay(100);
  tone(2, A);
  delay(100);
  tone(2, B);
  delay(100);
  noTone(2);

}

//The "void loop() {...." part of your program happens as long as 
//the program is turned on
void loop()
{
  for (int bunch = 500; bunch > 202; bunch-=10) // no parens because condition
  {
    tone(2, bunch);
    delay(1000);
  } 
  for (int bunch1 = 202; bunch1 < 500; bunch1+=10) // no parens because condition
  {
    tone(2, bunch1);
    delay(1000);
  }
}

