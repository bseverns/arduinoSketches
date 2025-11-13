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

### Next Steps
- As you tighten each build, commit the missing media to the placeholder paths above.
- If a build graduates to a full installation, spin up a dedicated folder under `docs/` with process shots, BOM sourcing links, and troubleshooting logs.
- Keep chasing utility: document the choices that helped the sketch behave in the wild.

