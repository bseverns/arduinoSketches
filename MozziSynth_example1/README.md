# MozziSynth_example1 Field Notes

Welcome to the scrappy-lush corner of the studio where phase modulation sings. This sketch is a tutorial companion piece for Mozzi, and the notes below should help you wire it, twist it, and keep it hollering without frying anything.

## Hardware hookup
- **Board**: Designed with Arduino Uno/Nano-style boards in mind (ATmega328P running at 16 MHz).
- **Audio out**: Mozzi pushes its signal on **digital pin 9**. Route that pin through the usual Mozzi-friendly RC low-pass (e.g., 1 kΩ in series, 100 nF to ground) or into the Mozzi Audio output circuit before hitting headphones, a mixer, or an amp. Keep grounds common.
- **Control voltage playground**: Five potentiometers, all wired as voltage dividers (5 V ↔ wiper ↔ GND) with their wipers patched to analog inputs:
  - `A0` – carrier pitch
  - `A1` – FM intensity (aka brightness/feedback)
  - `A2` – LFO speed for the intensity modulator
  - `A3` – texture scaler that multiplies the FM depth in real time
  - `A4` – harmonic ratio multiplier nudging the modulator/carrier relationship

## Knob behaviors & ranges
This sketch leans on `AutoMap` to squeeze every knob turn into useful territory.

| Control | Internal mapping | Sonic effect |
| ------- | ---------------- | ------------ |
| Carrier pitch (`A0`) | 22 Hz → 440 Hz | Sweeps the base note from sub drone to bright audio tone. |
| FM intensity (`A1`) | Raw 0–1023 mapped to 700 → 10 (yes, inverted) | Higher knob positions mellow the modulation; lower positions drive clangorous FM. |
| LFO speed (`A2`) | Raw 0–1023 mapped to 10,000 → 1, then scaled to Hz (`/1000`) | Counterintuitively, clockwise slows the wobble; counter-clockwise cranks the trem/alias storm. |
| Texture scaler (`A3`) | 1 → 15 multiplier | Acts like a parallel timbre fader, thickening or starving the modulation depth before smoothing. |
| Harmonic ratio (`A4`) | 1 → 10 multiplier | Pushes the modulator frequency to high-order sidebands or lets it chill near the carrier. |

Internally, `A4` multiplies the `mod_ratio` (default 5), so the modulator can range from 5× to 50× the carrier frequency. `A3` and the LFO team up to swing the final FM depth, with `Smooth<long>` taming zipper noise before it hits the audio-rate oscillator.

## Expected soundscape
- **Bass cave to bright bell**: Carrier sweeps 22–440 Hz while the modulator goes 5–50× higher, so expect everything from wobbly sub pulses to metallic chimes.
- **Dynamic FM grit**: FM intensity flips its response—full clockwise yields softer textures, counter-clockwise drives harsher clangs.
- **Slow-to-strobe tremolo**: LFO rate spans roughly 0.001–10 Hz after scaling, letting you drift through syrupy swells or frantic shimmers.

## Mozzi dependency check
1. Install the **Mozzi** library (grab it via the Arduino Library Manager or from [Mozzi on GitHub](https://github.com/sensorium/Mozzi)).
2. Restart the IDE so the headers resolve cleanly (`MozziGuts.h`, `Oscil.h`, `Smooth.h`, `AutoMap.h`, and the `cos2048` table all come from Mozzi).
3. For Unos, set the sketch to use the default `STANDARD_PLUS` audio mode unless you’ve modded `mozziConfig.h`.

## Quick troubleshooting
- **Silence?** Double-check pin 9 wiring, confirm `startMozzi()` is running (it is in `setup()`), and watch the serial monitor at 115200 baud—if the pot readouts move, the control loop lives; if not, your power/ground to the pots might be floating.
- **Noisy or crusty output?** Make sure the RC filter is present and the grounds are tight. Also, Mozzi hates floating analog inputs—verify each pot has its 5 V and GND legs secured. If the LFO knob is maxed counter-clockwise, the extreme modulation can alias; back it off for a tamer tone.
- **Ratty FM depth response?** Remember that intensity is inverted; if it feels backwards, swap the pot outer legs or flip the mapping constants.

Solder bravely, listen loudly, and document your own happy accidents right next to these notes.
