# Showcase Builds Field Notes

This is the cheat sheet for the sketches that consistently wow visitors at the bench. Treat it like the instructor copy of the zine: tight overviews, parts lists, wiring callouts, and pointers to future media drops.

> Novelty is fun, but repeatable magic is better. Each section below documents the most reliable, audience-ready version of the sketch it references.

## Mozzi 5-Knob Lead (folder: `mozzi_5knob_L`)

**Intent**: Expressive FM lead with five simultaneous control surfaces for sweeps, vibrato depth, detune, and filter weirdness.

**Bill of Materials**
- Arduino Uno (or any ATmega328P-based board)
- 5 × 10k linear potentiometers
- 1 × 1/4" or 3.5 mm audio jack breakout
- 1 × MCP4725 breakout (optional for CV experiments)
- Breadboard + jumper wires
- Powered speakers or headphones with isolation transformer

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| Audio Out | D9 (PWM) | Route through RC low-pass (e.g., 10k & 0.1 µF) before the jack |
| Pot 1 | A0 | Primary carrier frequency control |
| Pot 2 | A1 | Modulator frequency |
| Pot 3 | A2 | Mod index |
| Pot 4 | A3 | Filter/resonance mix |
| Pot 5 | A4 | Delay feedback |
| Audio Ground | GND | Tie to sleeve of output jack |
| Pot Grounds | GND | Common ground rail |
| Pot +5V | 5V | Shared positive rail |

```
Top view (breadboard-ish):
[5 × pots]
 wipers → A0–A4
 outer legs → +5V / GND buses
PWM D9 → RC filter → audio jack tip
GND bus → audio jack sleeve
```

**Performance Notes**
- Keep the Mozzi update rate at 16384 Hz for the smoothest sweeps.
- The PWM output likes a passive low-pass before meeting any amps—keep it mellow.
- Record every take; happy accidents are common when riding Pot 4 + Pot 5 together.

**Media TODOs**
- TODO: Drop final wiring diagram render at `docs/images/mozzi_5knob_L_wiring.png`.
- TODO: Record a 30 s riff and stash it at `docs/audio/mozzi_5knob_L_demo.wav`.
- TODO: Capture a knob-riding GIF for modulation interplay at `docs/gifs/mozzi_5knob_L_controls.gif`.

---

## WAV Shield Storyteller (folder: `WAVshield`)

**Intent**: Reliable SD-card playback rig for sound installations that need crisp 16-bit samples.

**Bill of Materials**
- Arduino Uno or Mega
- Adafruit Wave Shield (assembled)
- microSD card (FAT formatted)
- 3 × momentary push buttons
- 3 × 10k resistors for pull-downs
- Powered speakers or mixer input

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| Button 1 | D2 | Connect to ground via 10k pull-down |
| Button 2 | D3 | Same layout as Button 1 |
| Button 3 | D4 | Same layout as Button 1 |
| Audio L/R | Wave Shield terminals | Use shield’s DAC outputs |
| Power | 5V + GND | Shared between Arduino and shield |

```
Front panel idea:
[Btn1] [Btn2] [Btn3]
Pins D2–D4 ←→ buttons → GND
Pull-down resistors from pins to ground keep things quiet between triggers.
```

**Performance Notes**
- Normalize WAV files to -1 dBFS to avoid clipping in the onboard mixer.
- Keep filenames 8.3 characters and pre-buffer in `setup()` for faster first-trigger response.
- For installations, add a watchdog reset to auto-recover from SD hiccups.

**Media TODOs**
- TODO: Render shield + panel wiring diagram to `docs/images/wavshield_frontpanel.svg`.
- TODO: Record button-mashing performance at `docs/audio/wavshield_three_shots.wav`.

---

## DrawBot Relay Driver (folder: `drawBots`)

**Intent**: Low-fi plotter controller that slaps relays instead of steppers to animate hacked appliances or contact mics.

**Bill of Materials**
- Arduino Uno
- 2 × SPDT 5V relays with driver transistors (or a relay shield)
- 2 × flyback diodes (1N4001)
- 2 × NPN transistors (e.g., 2N2222) + base resistors (1k)
- External 5V supply for the relays
- Alligator clips or screw terminals for your kinetic load

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| Relay 1 control | D8 | Goes through 1k to transistor base |
| Relay 2 control | D9 | Same as above |
| Transistor emitters | GND | Tie to Arduino ground |
| Transistor collectors | Relay coil negative | Coil positive → external +5V |
| Flyback diodes | Across relay coils | Stripe toward +5V |
| Load | Relay common + NO/NC | Pick the side that matches your vibe |

```
Remember: share grounds between Arduino and the external 5V relay supply, otherwise the relays ghost-trigger.
```

**Performance Notes**
- Use `delayMicroseconds()` for short rhythmic pulses; relays have inertia but love staccato hits.
- Route power-hungry loads through the relay common/NO terminals, not straight from the Arduino.
- Capture the mechanical chatter with a contact mic for tasty percussive sampling.

**Media TODOs**
- TODO: Draft a connection diagram showing relay wiring at `docs/images/drawbot_relays.png`.
- TODO: Record a contact-mic loop of the relays at `docs/audio/drawbot_clicks.wav`.
- TODO: Film a 10 s GIF of the relay dancing with brushes at `docs/gifs/drawbot_brushdance.gif`.

---

## Ping-Baby Motion Sampler (folder: `Ping_baby`)

**Intent**: Motion-triggered sampler using an HC-SR04 ultrasonic sensor to fire off responses as people walk by.

**Bill of Materials**
- Arduino Uno
- HC-SR04 ultrasonic sensor
- Small powered speaker
- MicroSD breakout (if using external samples) or onboard tone via `tone()`
- Optional: WS2812 strip for reactive lighting

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| HC-SR04 Trig | D7 | |
| HC-SR04 Echo | D6 | Add a 10k/4.7k voltage divider if using 3.3V board |
| HC-SR04 VCC | 5V | |
| HC-SR04 GND | GND | |
| Speaker | D9 (PWM) | Add RC filter + cap |
| WS2812 Data (optional) | D5 | Power from 5V with large cap |

**Performance Notes**
- Debounce motion triggers so you don’t spam the same clip when someone lingers.
- Map distance bands to different samples for more playful interactivity.
- Pair with soft lighting so folks know it’s armed but don’t feel like they’re under surveillance.

**Media TODOs**
- TODO: Capture sensor mounting photo at `docs/images/pingbaby_sensor_mount.jpg`.
- TODO: Record ambient-reactive loop at `docs/audio/pingbaby_walkby.wav`.

---

## Hallway Reactor 2 m (folder: `holding/modern-platformio-demos/hallway_reactor_2m`)

**Intent**: Three ToF sensors infer entry direction across a hallway and turn a 2 m LED strip into comet, bloom, and dwell states.

**Bill of Materials**
- Arduino Uno or Nano
- 3 × VL53L1X distance sensors
- 1 × 2 m WS2812B strip, about 120 px total
- 1 × 5V power supply sized for the strip
- 1 × quiet-hours toggle switch
- Diffuser channel or smoked acrylic cover

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| LED Data | D6 | NeoPixel data line |
| XSHUT East | D2 | Used to assign sensor I2C address |
| XSHUT Mid | D3 | Used to assign sensor I2C address |
| XSHUT West | D4 | Used to assign sensor I2C address |
| Quiet Switch | D7 | Internal pull-up enabled, active LOW |
| I2C SDA | A4 | Shared across all three sensors |
| I2C SCL | A5 | Shared across all three sensors |
| LED Power | External 5V | Share ground with the Arduino |

```
East sensor ----\
Mid sensor  ----- I2C bus ---- Uno/Nano ---- D6 ---- 2 m LED strip
West sensor ----/

XSHUT pins on D2/D3/D4 let each VL53L1X boot one-by-one with a unique address.
```

**Performance Notes**
- The install works best when the sensors are slightly baffled so they read the hallway, not each other.
- Diffusion matters more than raw brightness; a soft comet read is what sells the piece.
- This is one of the clearest audience-facing sketches in the repo because the behavior is visible from across the room.

**Media TODOs**
- TODO: Add hallway mounting diagram at `docs/images/hallway_reactor_2m_layout.svg`.
- TODO: Capture an entry-direction demo clip at `docs/gifs/hallway_reactor_comet.gif`.
- TODO: Record a short install walkthrough at `docs/video/hallway_reactor_demo.mp4`.

---

## LineLight Band Lamp (folder: `holding/modern-platformio-demos/linelight_band_lamp`)

**Intent**: A single lamp listens to line-level audio, isolates a tunable FFT band, and glows with smoothed, gain-controlled intensity.

**Bill of Materials**
- SparkFun Pro Mini 5V/16 MHz or similar ATmega328P board
- 2 × 10k linear potentiometers
- 1 × lamp or LED fixture driven by a MOSFET
- 1 × audio input bias/filter front end
- FTDI Basic or similar serial uploader
- External lamp power supply sized for the load

**Wiring Cheat Sheet**
| Signal | Arduino Pin | Notes |
| --- | --- | --- |
| Audio In | A0 | AC-coupled and biased around mid-rail |
| Low Band Pot | A1 | Chooses low FFT edge |
| High Band Pot | A2 | Chooses high FFT edge |
| PWM Out | D9 | Timer1 OC1A high-frequency PWM |
| Lamp Ground | GND | Share with the external driver stage |

```
Audio source -> bias + filter front end -> A0
Pot 1 wiper -> A1
Pot 2 wiper -> A2
D9 -> MOSFET gate/driver -> lamp
```

**Performance Notes**
- This one is showcase material because the hardware is small but the response feels more sophisticated than “audio reactive lights.”
- The AGC and smoothing make it forgiving in live demos where source levels vary a lot.
- Keep the serial monitor handy when tuning thresholds; the bin and AGC printouts are part of the charm.

**Media TODOs**
- TODO: Add band-lamp wiring sheet at `docs/images/linelight_band_lamp_wiring.svg`.
- TODO: Record a frequency-sweep demo at `docs/audio/linelight_band_sweep.wav`.
- TODO: Capture a side-by-side pot interaction clip at `docs/gifs/linelight_pot_sweep.gif`.

---

## MOARkNOBS Button + Envelope Demo (folder: `holding/modern-platformio-demos/moarknobs_button_ef_usb_midi_demo`)

**Intent**: Tiny Teensy control-surface demo: one button fires a note and one envelope follower emits MIDI CC, enough to explain the bigger rig in miniature.

**Bill of Materials**
- Teensy 4.0
- 1 × momentary push button
- 1 × envelope follower output or control-voltage source
- USB connection to a laptop or synth host
- Breadboard + jumper wires

**Wiring Cheat Sheet**
| Signal | Teensy Pin | Notes |
| --- | --- | --- |
| Button | D12 | Active LOW, internal pull-up |
| Envelope Input | A0 | 0-3.3V analog source |
| USB MIDI | USB | Sends note and CC data |

```
Button -> D12 to GND
Envelope follower out -> A0
Teensy USB -> laptop running a MIDI monitor or synth
```

**Performance Notes**
- This is a great workshop sketch because the concept lands immediately and the whole rig fits on a breadboard.
- It is also a strong “explain the larger controller by shrinking it” demo.
- Keep the input quiet during boot so the baseline calibration settles cleanly.

**Media TODOs**
- TODO: Add breadboard layout at `docs/images/moarknobs_button_ef_demo_bb.png`.
- TODO: Record MIDI monitor capture at `docs/video/moarknobs_button_ef_demo.mp4`.
- TODO: Add a simple patch example at `docs/audio/moarknobs_button_ef_demo_usage.wav`.

---

## Horizon Preset Morph (folder: `holding/modern-platformio-demos/horizon_preset_morph`)

**Intent**: Self-running stereo widening and mastering demo that slowly morphs between two contrasting Horizon scenes so the audience can hear the processor breathe without touching a control.

**Bill of Materials**
- Teensy 4.0 or 4.1
- PJRC Audio Shield
- Stereo line-level source
- Headphones, powered speakers, or a mixer return
- USB cable for power and serial logging
- Breadboard or simple shield stack mounting

**Wiring Cheat Sheet**
| Signal | Teensy / Shield Pin | Notes |
| --- | --- | --- |
| Audio Input L/R | Audio Shield line in | Feed stereo program material |
| Audio Output L/R | Audio Shield headphone or line out | Monitor the morphing scenes |
| SGTL5000 control | Default I2C pins | Handled by the Audio Shield wiring |
| USB | USB | Power + optional serial preset logs |

```
Stereo source -> Audio Shield line in
Teensy + Horizon sketch
Audio Shield out -> headphones / mixer / powered speakers

No front-panel controls required for the demo surface.
```

**Performance Notes**
- This is a strong bench demo because it performs by itself once audio is running.
- The contrast between the wide "Cinema Morph" scene and the steadier "Bus Pillow" scene reads clearly even in a short listening test.
- Use dense stereo material, pads, or full mixes rather than dry mono signals; the effect is much easier to hear when the source has spatial content.

**Media TODOs**
- TODO: Add simple signal-flow card at `docs/images/horizon_preset_morph_signal_flow.svg`.
- TODO: Record before/after scene cycle at `docs/audio/horizon_preset_morph_cycle.wav`.
- TODO: Capture serial + audio bench clip at `docs/video/horizon_preset_morph_demo.mp4`.

---

### Next Steps
- As you tighten each build, commit the missing media to the placeholder paths above.
- If a build graduates to a full installation, spin up a dedicated folder under `docs/` with process shots, BOM sourcing links, and troubleshooting logs.
- Keep chasing utility: document the choices that helped the sketch behave in the wild.
