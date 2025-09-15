/****************************************************************
* * Arduino Controlled Servo Robot (SERB) - Whisker Avoidance   
* * For more details visit: http://www.oomlout.com/serb           
* *                                                               
*   * Behaviour: Uses two wire whiskers, to bump into obstacles and 
* *            change direction accordingly. It will poll the     
* *            whiskers and when one is found to be tripped it    
* *            will reverse for 500 milliseconds then             
* *            turn randomly left or right for a random time      
* *            between 300 and 1500 milliseconds                  
*  *                                                               
* * Wiring: --Arduino Controlled Servo Robot--                    
* *             Right Servo Signal - pin 9                        
*   *             Left Servo Signal  - pin 10                       
* *         --Whisker Wiring--                                    
*  *             right whisker  - pin 6                            
* *             left whisker   - pin 7                            
* *             whisker common - ground                           
* *                                                               
* * License: This work is licenced under the Creative Commons     
* *          Attribution-Share Alike 3.0 Unported License. To     
* *          view a copy of this licence, visit                   
* *          http://creativecommons.org/licenses/by-sa/3.0/       
* *          or send a letter to Creative Commons, 171 Second     
* *          Street, Suite 300, San Francisco, California 94105,  
* *          USA.                                                 
* *                                                                        
****************************************************************/
//----------------------------------------------------------------
// START OF WHISKER PREAMBLE
#define RIGHTWHISKER 6  
//the pin the right whisker is attached to
#define LEFTWHISKER 7   
//the pin the left whisker is attached to
//----------------------------------------------------------------
// START OF ARDUINO CONTROLLED SERVO ROBOT (SERB) PREAMBLE
#include <Servo.h>
#define LEFTSERVOPIN  10  //The pin the left Servo is connected to
#define RIGHTSERVOPIN  9  //The pin the right Servo is connected to#
define MAXSPEED 30  //due to the way continuous rotation servos                      
//work maximum speed is reached at a much                      
//lower value than 90 (this value will change                     
//depending on your servos) (for Parallax servos)                     
//20 will give you full range 10 makes it very                     
//controllable but a little slow
Servo leftServo; 
Servo rightServo; 
int leftSpeed;        //sets the speed of the robot (left servos)                       
//a percentage between -MAXSPEED and MAXSPEED
int rightSpeed;       //sets the speed of the robot (right servos)                       
//a percentage between -MAXSPEED and MAXSPEED
int speed = 100;      //used for simple control (goForward etc.)                      
//a percentage between 0 and MAXSPEE
D// END OF THIS PROGRAMS PREAMBLE
//----------------------------------------------------------------

void setup() 
{  beginSerial (9600);   //Starts the serial port   
serbSetup();          //adds the servos and prepares all                         
//SERB related variables  
whiskerSetup();                        
}
/* * The main loop 
* Sends the robot forward then checks it's whiskers, if one is pressed 
* the robot will reverse then turn and head forward once more   */
void loop()
{  goForward();      //sends the robot forward  
checkWhiskers();  //checks to see if a whisker is pressed
}
//------------------------------------------------------------------------

//START WHISKER ROUTINES
void whiskerSetup()
{  pinMode(RIGHTWHISKER, INPUT);    //Sets the right whisker's pin to be an input  
pinMode(LEFTWHISKER, INPUT);     //Sets the left whisker's pin to be an input  
digitalWrite(RIGHTWHISKER, HIGH);//Sets the right whisker pin's internal pullup                                    
//resistor (this means the pin is high when                                    
//there is no signal attached and reads low                                   
//when a negative signal is attached  
digitalWrite(LEFTWHISKER, HIGH);//Sets the left whisker pin's internal pullup                                   //resistor
}

void checkWhiskers()
{  if(!digitalRead(RIGHTWHISKER))
{reverseAndTurn(); } //if the right whisker is pressed                                                    
//then reverse and turn  
if(!digitalRead(LEFTWHISKER))
{reverseAndTurn();}  //if the left whisker is pressed                                                    
//then reverse and turn
}

void reverseAndTurn()
{ goBackward();         //goes backward 
delay(500);           //for half a second 
turnRandom(300,1500); //turns randomly for a time between 300 and 1500 milliseconds
}
//END OF WHISKER ROUTINES
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//START OF ARDUINO CONTROLLED SERVO ROBOT (SERB) ROUTINES
/* * sets up your arduino to address your SERB using the included  * routines */

void serbSetup()
{  setSpeed(speed);  
pinMode(LEFTSERVOPIN, OUTPUT);     //sets the left servo signal pin   //to output 
pinMode(RIGHTSERVOPIN, OUTPUT);    //sets the right servo signal pin   //to output  
leftServo.attach(LEFTSERVOPIN);    //attaches left servo  
rightServo.attach(RIGHTSERVOPIN);  //attaches right servo  
goStop();}/* * sets the speed of the robot between 0-(stopped) and 100-(full speed) 
* NOTE: speed will not change the current speed you must change speed  
* then call one of the go methods before changes occur. */

void setSpeed(int newSpeed) {
  if(newSpeed >= 100) 
{    newSpeed = 100;  }     //if speed is greater than 100         
//make it 100  
if(newSpeed <= 0) {
  newSpeed = 0;  }         //if speed is less than 0 make             
//it 0   
speed = newSpeed * MAXSPEED / 100;        //scales the speed to be                                             
//between 0 and MAXSPEED
}

/* * sets the speed of the robots rightServo between -100-(reversed) and 100-(forward) 
* NOTE: calls to this routine will take effect imediatly */
void setSpeedRight(int newSpeed){  
if(newSpeed >= 100) {    
newSpeed = 100;  }     //if speed is greater than 100         
//make it 100  
if(newSpeed <= -100) {    
newSpeed = -100;  }   //if speed is less than -100 make       
//it -100   
rightSpeed = newSpeed * MAXSPEED / 100;   //scales the speed to be                                             
//between -MAXSPEED and MAXSPEED  
rightServo.write(90 - rightSpeed);        //sends the new value to the servo}

/* * sets the speed of the robots leftServo between -100-(reversed) and 100-(forward) * NOTE: calls to this routine will take effect immediatly */

void setSpeedLeft(int newSpeed){  
if(newSpeed >= 100) {
newSpeed = 100;} //if speed is greater than 100                                         
//make it 100  
if(newSpeed <= -100){
newSpeed = -100;}//if speed is less than -100                                         
//make it -100   
leftSpeed = newSpeed * MAXSPEED / 100;//scales the speed to be                                         
//between -MAXSPEED and MAXSPEED  
leftServo.write(90 + leftSpeed);      //sends the new value to the servo}
/* * turns the robot randomly left or right for a random time period between * minTime (milliseconds) and maxTime (milliseconds) */

void turnRandom(int minTime, int maxTime){  
int choice = random(2);                     //Random number to decide                                               
//between left (1) and right (0)  
int turnTime = random(minTime,maxTime);     //Random number for the pause                                               
//time  
if(choice == 1){ 
goLeft();}                 //If random number = 1 then turn                                               
//left  
else {goRight();}                           
//If random number = 0 then turn                                               
//right  
delay(turnTime);                            //delay for random time                         
}

/* * sends the robot forwards */
void goForward(){  
leftServo.write(90 + speed);  
rightServo.write(90 - speed);
}

/* * sends the robot backwards */
void goBackward(){  
leftServo.write(90 - speed);  
rightServo.write(90 + speed);
}

/* * sends the robot right */
void goRight(){  
leftServo.write(90 + speed);  
rightServo.write(90 + speed);
}

/* * sends the robot left */
void goLeft(){  
leftServo.write(90 - speed);  
rightServo.write(90 - speed);
}

/* * stops the robot */
void goStop(){  
leftServo.write(90);  
rightServo.write(90);}
//END OF ARDUINO CONTROLLED SERVO ROBOT (SERB) ROUTINES
//---------------------------------------------------------------------------
