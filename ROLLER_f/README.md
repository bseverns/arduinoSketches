# ROLLER_f

PlatformIO-based Roller project targeting a Teensy 4.1.

## What It Appears To Do

This project coordinates a 4-lane roller game or kinetic installation with:

- four stepper-driven lanes
- one home sensor per lane
- lane status indicators
- NeoPixel lighting
- WAV-triggered game audio
- serial input from four external lane/sensor controllers

The main entry point is [src/main.cpp](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/src/main.cpp), which wires together the game, serial, LED, audio, and stepper modules.

## Build Target

- board: Teensy 4.1
- framework: Arduino
- build system: PlatformIO

Config lives in [platformIO.ini](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/platformIO.ini).

## Project Layout

- [src/main.cpp](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/src/main.cpp)
  - creates four `StepperControl` instances and the top-level managers
- [lib/GameControl](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/lib/GameControl/GameControl.cpp)
  - overall game state, homing, lane win detection, reset flow
- [lib/StepperControl](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/lib/StepperControl/StepperControl.cpp)
  - AccelStepper wrapper for homing and movement
- [lib/SerialManager](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/lib/SerialManager/SerialManager.cpp)
  - lane health/status input over `Serial1`, `Serial6`, `Serial7`, `Serial8`
- [lib/LEDManager](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/lib/LEDManager/LEDManager.cpp)
  - NeoPixel startup / chase / winner color handling
- [lib/AudioManager](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/lib/AudioManager/AudioManager.cpp)
  - WAV playback for start, game loop, and ball hit sounds

## Current Hardware Mapping

From [src/main.cpp](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/src/main.cpp):

- lane 1: step `12`, dir `11`, home `30`
- lane 2: step `14`, dir `13`, home `32`
- lane 3: step `16`, dir `15`, home `36`
- lane 4: step `18`, dir `17`, home `38`
- lane indicator pins: `3, 4, 5, 6`
- NeoPixel strip: pin `23`, count `42`

Serial ports:

- lane 1: `Serial1`
- lane 2: `Serial6`
- lane 3: `Serial7`
- lane 4: `Serial8`

## Dependencies

Declared in [platformIO.ini](/Users/bseverns/Documents/GitHub/arduinoSketches/ROLLER_f/platformIO.ini):

- `Adafruit NeoPixel`
- `Audio`
- `SD`
- `AccelStepper`

## Notes

- Audio playback expects `startGame.wav`, `rollerGame.wav`, and `ballHit.wav` to be available to the Teensy audio/SD setup.
- `SerialManager::handleCommand()` is still a stub, so the serial protocol looks only partially implemented.
- `Untitled-1.yaml` is present in this folder but does not appear to be referenced by the code yet.
