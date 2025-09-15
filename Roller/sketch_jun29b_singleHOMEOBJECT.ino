#include <AccelStepper.h>

// Pin definitions
#define DIR_PIN 11
#define STEP_PIN 12
#define SWITCH_PIN 38
#define LED_IDLE_PIN 3
#define LED_MOVING_TO_HOME_PIN 4
#define LED_BACKING_OFF_SWITCH_PIN 5

#define MOTOR_INTERFACE_TYPE 1

// Motor states
enum MotorState {
  IDLE,
  MOVING_TO_HOME,
  BACKING_OFF_SWITCH,
  REVERSING,
  AT_SOFT_HOME,
  MOVING_TO_POSITION,
  STOPPED
};

class StepperMotor {
private:
  AccelStepper stepper;
  int homeSwitchPin;
  int ledIdlePin;
  int ledMovingToHomePin;
  int ledBackingOffSwitchPin;
  MotorState state;
  bool previousSwitchState;

public:
  StepperMotor(int interface, int step, int dir, int homeSwitch, int ledIdle, int ledMovingToHome, int ledBackingOffSwitch)
    : stepper(interface, step, dir),
      homeSwitchPin(homeSwitch),
      ledIdlePin(ledIdle),
      ledMovingToHomePin(ledMovingToHome),
      ledBackingOffSwitchPin(ledBackingOffSwitch),
      state(IDLE),
      previousSwitchState(true) {
    pinMode(homeSwitchPin, INPUT_PULLUP);
    pinMode(ledIdlePin, OUTPUT);
    pinMode(ledMovingToHomePin, OUTPUT);
    pinMode(ledBackingOffSwitchPin, OUTPUT);
    stepper.setMaxSpeed(2000.0);      // Adjust the maximum speed (steps per second)
    stepper.setAcceleration(1000.0);  // Adjust the acceleration (steps per second per second)
    stepper.setSpeed(1000.0);         // Set the initial speed
    stepper.setCurrentPosition(0);    // Set the current position as START_ZERO
  }

  void moveToHome() {
    if (state == IDLE) {
      state = MOVING_TO_HOME;
      stepper.moveTo(40000);  // Move a fixed number of steps in the desired direction
    }
  }

  void moveToPosition() {
    if (state == AT_SOFT_HOME) {
      state = MOVING_TO_POSITION;
      stepper.moveTo(-14000);  // Move to the specific position
    }
  }

  void update() {
    bool currentSwitchState = digitalRead(homeSwitchPin);

    if (state == MOVING_TO_HOME) {
      digitalWrite(ledIdlePin, LOW);
      digitalWrite(ledMovingToHomePin, HIGH);
      digitalWrite(ledBackingOffSwitchPin, LOW);

      if (currentSwitchState != previousSwitchState) {
        // Home switch triggered
        state = BACKING_OFF_SWITCH;
        stepper.stop();     // Stop the motor
        stepper.move(150);  // Bounce back a bit
      }
    } else if (state == BACKING_OFF_SWITCH) {
      digitalWrite(ledIdlePin, LOW);
      digitalWrite(ledMovingToHomePin, LOW);
      digitalWrite(ledBackingOffSwitchPin, HIGH);

      if (!stepper.isRunning()) {
        // Movement in the opposite direction completed
        state = AT_SOFT_HOME;
        stepper.setCurrentPosition(0);  // Reset the current position as START_ZERO
      }
    } else if (state == AT_SOFT_HOME) {
      digitalWrite(ledIdlePin, HIGH);
      digitalWrite(ledMovingToHomePin, LOW);
      digitalWrite(ledBackingOffSwitchPin, LOW);

      moveToPosition();  // We move to the position after we are at soft home
    } 
    else if (state == MOVING_TO_POSITION) {
      digitalWrite(ledIdlePin, LOW);
      digitalWrite(ledMovingToHomePin, HIGH);
      digitalWrite(ledBackingOffSwitchPin, LOW);

      if (!stepper.isRunning()) {
        // Position reached
        state = MOVING_TO_HOME;
        stepper.setSpeed(1000);  // Set the speed for moving to home
        stepper.moveTo(14000);   // Move all the way back to home
      }
    }

    stepper.run();

    previousSwitchState = currentSwitchState;
  }
};

StepperMotor motor(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN, SWITCH_PIN, LED_IDLE_PIN, LED_MOVING_TO_HOME_PIN, LED_BACKING_OFF_SWITCH_PIN);

void setup() {
  delay(250);
  Serial.begin(9600);
  motor.moveToHome();
}

void loop() {
  motor.update();
}