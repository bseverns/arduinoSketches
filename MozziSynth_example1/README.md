# Grey Box FM Rig Field Notes

Welcome to the Grey Box stack: a punky little FM laboratory built around MozziSynth_example1. This is the rig that anchors the Grey Box performances—five knobs, one audio pin, and a pile of modulation mischief. Use this README like a zine: skim the vibe, wire the thing, then scribble your own discoveries in the margins.

## What this sketch is doing
- Runs the Mozzi audio engine at audio/control rates on an Arduino Uno or Nano.
- Dials in classic two-oscillator phase modulation (carrier + modulator) with an LFO breathing through the FM depth.
- Streams raw knob data over Serial so you can watch the control surface move while you patch.

## Kit of parts (Grey Box stack)
| Piece | Spec | Notes |
| ----- | ---- | ----- |
| MCU | Arduino Uno / Nano (ATmega328P @ 16 MHz) | Any 5 V board that Mozzi supports will do, but Grey Box was voiced on an Uno. |
| Audio out | Digital **pin 9** → RC low-pass (1 kΩ in series, 100 nF to ground) → mixer/amp | Keep grounds common. Treat pin 9 as line-level; add a volume pot or isolation transformer if your PA is fussy. |
| Pots | Five 10 kΩ linear pots wired as voltage dividers | 5 V on one outer leg, GND on the other, wiper to the analog pin listed below. |
| Power | 5 V DC shared between Arduino and control surface | Grey Box stack prefers USB or a regulated 5 V brick. Avoid saggy phone chargers; they chatter the ADC. |
| Monitoring | USB serial at **115200 baud** | Open the serial monitor to confirm the controls are alive and to debug mappings. |

## Wire it up
1. Solder or breadboard the five potentiometers. From left to right in the Grey Box chassis, we route them to A0–A4.
2. Patch audio: pin 9 → 1 kΩ resistor → output jack tip. Hang 100 nF from jack tip to ground for the low-pass filter.
3. Tie all grounds together (Arduino GND, pot rails, audio jack sleeve).
4. Optional: run the USB cable to a laptop for serial diagnostics or to log knob moves during rehearsals.

```
A0  Carrier pitch      → "FREQ" knob
A1  FM intensity       → "BRIGHT" knob (inverted response)
A2  LFO speed          → "RATE" knob (clockwise = slower)
A3  Texture scaler     → "TEXTURE" knob (multiplies FM depth)
A4  Harmonic ratio     → "HARM" knob (multiplies mod/carrier ratio)
```

## Control cheat sheet
| Control | Internal mapping | Sonic effect |
| ------- | ---------------- | ------------ |
| `A0` – FREQ | AutoMaps 0–1023 → **22–440 Hz** | Sweeps the carrier from droney floorboards to bright bell tones. |
| `A1` – BRIGHT | AutoMaps 0–1023 → **700 → 10** (inverted) | Counter-clockwise slams the mod depth for clangy FM; clockwise relaxes into gentle shimmer. |
| `A2` – RATE | AutoMaps 0–1023 → **10,000 → 1**, then `/1000` | Acts like a wobble brake: CW gives syrupy swells (~0.001 Hz), CCW gives trem-stutter (~10 Hz). |
| `A3` – TEXTURE | AutoMaps 0–1023 → **×1 → ×15** | Scales the FM depth before smoothing, thickening the sound without touching the base BRIGHT setting. |
| `A4` – HARM | AutoMaps 0–1023 → **×1 → ×10** | Multiplies the fixed mod ratio (default 5×), so the modulator swings from 5× to 50× the carrier. |

Behind the scenes, `Smooth<long>` keeps FM depth changes from zippering, and an audio-rate cosine table drives both carrier and modulator. The LFO (`kIntensityMod`) is a control-rate oscillator that feeds into the FM depth math each frame.

## Play moves to try
- **Glass gong**: Set FREQ midrange (~12 o'clock), BRIGHT at 9 o'clock, TEXTURE around 2 o'clock, HARM near full CW. Ride RATE slowly CW to let the gong bloom.
- **Alien trem-bass**: FREQ down low, BRIGHT barely cracked open, RATE all the way CCW (fast), TEXTURE mid, HARM around noon. The LFO turns into a growling tremolo.
- **Grey Box drone pad**: FREQ ~10 o'clock, BRIGHT ~1 o'clock, RATE ~3 o'clock, TEXTURE high, HARM mid. Let it hang and twist BRIGHT for evolving harmonics.

Write down your own winning combos—Grey Box thrives on curated settings you can recall during a performance.

## Troubleshooting + maintenance
| Symptom | Likely culprit | Quick fix |
| ------- | -------------- | --------- |
| Dead silent | Pin 9 not reaching the filter/output, `startMozzi()` not executing, or volume downstream is muted | Re-seat the RC filter, confirm `loop()` is calling `audioHook()` (it is in stock code), and crank your amp. Check the serial stream for knob movement to prove the MCU is alive. |
| Static / fizz | Missing RC filter, floating pot leg, noisy power supply | Verify the 1 kΩ/100 nF filter, tighten the 5 V/GND rails on every pot, and power from a stable USB port or regulated 5 V. |
| Controls feel backwards | AutoMap uses inverted ranges for BRIGHT and RATE | Swap the outer legs on that pot or edit `MIN/MAX` constants in the sketch. |
| Serial spam overwhelms logs | Debug prints run every control tick | Only enable Serial when you need it, or comment out the `Serial.print` lines once the hardware is trusted. |
| LFO stalls or screams | RATE pot at an extreme, or Mozzi clock/timer conflict with other shields | Keep RATE off the stops for mid-speed wobble, and avoid stacking timer-hungry shields without adjusting Mozzi’s config. |

## Extend the rig
- Add a footswitch on a spare digital pin to freeze the LFO (`kIntensityMod.setFreq(0)` when pressed).
- Swap `cos2048` for other wave tables (sine, triangle) to reshape the FM voice.
- Capture the Serial stream in a DAW/Max patch to automate knob sweeps during a set.

Patch hard, play loud, and keep adding to the Grey Box lore.
