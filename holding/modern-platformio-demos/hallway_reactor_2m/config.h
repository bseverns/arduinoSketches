#pragma once

// ====== Physical layout ======
#define NUM_LEDS        120   // 2 m @ 60 px/m
#define LED_PIN         6
#define LED_BRIGHTNESS_NORMAL  80    // 0-255 (~31%)
#define LED_BRIGHTNESS_QUIET   8     // ~3% for Quiet Hours

// ====== VL53L1X addressing pins ======
#define XSHUT_E 2
#define XSHUT_M 3
#define XSHUT_W 4

// ====== Sensing thresholds (millimeters) ======
#define NEAR_EW_MM     900    // end sensors: "close"
#define PRES_EW_MM     1500   // end sensors: broad presence
#define NEAR_MID_MM    700    // mid sensor: "crossing"

// ====== Timing (milliseconds) ======
#define DEBOUNCE_MS    100
#define DIR_WINDOW_MS  700
#define DWELL_MS       4000

// ====== Quiet Hours physical toggle (SPST) ======
// Wire: PIN ---[switch]--- GND ; enable internal pull-up.
// When the switch is CLOSED (to GND), quiet mode is ACTIVE.
#define QUIET_SWITCH_PIN   7
#define QUIET_ACTIVE_LOW   1   // 1 = active when pin reads LOW (with INPUT_PULLUP)
