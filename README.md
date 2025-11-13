# Arduino Sketchbook Field Guide

Welcome to the synth lab slash robot garage. This repo is a studio notebook of quick-and-dirty experiments, but now it has a map. Use this README like a zine—skim the spreads, pick a neighborhood, and dive in with intent.

## How this sketchbook is organized
Each folder is an Arduino sketch directory. Drop one into your `sketchbook/` and the IDE will treat it as its own project. Libraries such as **Mozzi** or **Adafruit WaveHC** must already be installed in your Arduino environment.

The collections below are loose crews, not rigid taxonomies; if you discover better alignments, remix away.

### Synthesis & Sound Design
Projects that cook tones directly on the microcontroller.
- `Mozzi/`, `mozzi_5knob/`, `mozzi_5knob_L/`, `mozzi_5knob_L_GB/`, `mozziSynth_glitch/`, `MozziSynth_example1/`
  - Mozzi-powered sketches covering FM, glitchy filters, multi-knob control, and the cleaned-up FM showcase in this repo.
- `attiny85_random_burst_drone/`, `attiny85_dual_clock_clash/`, `attiny85_quad_xor_drone/`,
  `attiny85_pitch_ramp_randomizer/`, `attiny85_three_voice_gate/`, `attiny85_scale_sequencer/`,
  `attiny85_random_pwm_noise/`, `attiny85_pitch_mutator/`, `attiny85_drum_trax/`
  - Bare-metal ATTiny85 tone experiments rescued from hash-folders; expect raw timer hacking and button-driven chaos.
- `auduino/`, `auduino_v5/`, `AuduinoSequencer/`
  - Classic granular/noise textures and sequencer riffs.
- `granularSynth/`, `pdVCO/`, `avrPitchSampler/`
  - More experimental oscillators and pitch shifters.

### Samplers, Players & Shields
Boards that sling prerecorded audio or lean on helper hardware.
- `WAVshield/`, `wavehc/`, `PING_WAV/`, `Ping_baby/`, `PIR_soundie/`
  - SD-card playback patterns, button routing, and motion-triggered samples.
- `SamplerUnoPlayer/`, `SamplerUnoPlayer_test/`, `SampleRateMod/`, `sampleLauncher_neotrellis/`
  - Quick-loading sample players, rate changers, and the NeoTrellis launcher.
- `AttinySound-master/`, `tiny85audioplayer/`
  - Tiny microcontroller players when you’re short on pins and patience.

### Rhythm, Sequencers & Generative Gizmos
Keep time, break time, or let the machine improvise.
- `ArduinoPunk/`, `drum8/`, `drumkid-master/`, `Beat707-Projects-master/`
  - Drum patterns, step sequencing, and tempo juggling.
- `generativeAlgoSound/`, `sixteen_step_sequencer/`, `Musicator/`, `_2noise/`
  - Algorithmic pieces for when you want the synth to dream.
- `player_simple_shieldVersion/`, `player_interrupts_ShieldVersion/`
  - IRQ-friendly players useful for tempo-locked sample chops.

### Robotics, Kinetics & Control Surfaces
Anything that moves, senses, or bosses motors around.
- `drawBots/`, `drawBots_1/`
  - Button handling patterns and motion control for pen-plotter style bots.
- `dc_motor.ino` inside `drawBots/`, `realDRobot/`, `greyBox_Growser/`*
  - Reversible H-bridge driving and relay logic for more industrial builds.
- `OurRobot_1/`, `Our_Robot_01/`, `ENERGY_1/`, `Energy_FADE/`
  - Lighting and motion mashups for stage pieces.

### Reference Dumps & Vendor Libraries
- `AdafruitAudio-master/`, `Adafruit_NeoTrellisM4-master/`, `Arduinoboy-master/`, `jan_ostmans_synths-master/`
  - Mostly upstream copies; treat them as reference manuals.
- `DuinoTune-master/`, `PJRCAudio-master/`, `AttinySound-master/`
  - Utility toolkits that underpin several sketches.

### Notes, Docs & Ephemera
- `docs/`
  - Former stray PDFs now corralled into a single folder.
  - See `docs/showcase.md` for wiring guides + BOMs on the crowd-pleasers.
- `Notes_keke_siren/`, `ReferanceSketch/`, `SKY_PHONE_2/`
  - Mixed notes and iterations. Expect loose ends.

\* Many of these builds assume external motor drivers, relays, or sensors. Read the header comments in each sketch for wiring specifics before soldering anything permanent.

## Quick-start checklists
1. **Pick a theme above** and copy that folder to your Arduino sketchbook.
2. **Install dependencies** (Mozzi, WaveHC, Sabertooth, etc.) via the Arduino Library Manager or from the vendor repos here.
3. **Check pin assignments** in the `.ino` file comments and adjust for your hardware.
4. **Prototype fast**. This is a playground—fork the sketch, twist a knob, record what happens.

## Where to explore next
- Tighten the best-performing sketches into showcase-ready demos with photos or wiring diagrams. `docs/showcase.md` has TODO slots waiting for media drops.
- Document unexpected tricks (timer sharing, ISR gotchas) directly in the sketches.
- Trade polished recordings for commit history: drop audio snippets or GIFs into `docs/` to complement the code. Call your shot with TODO notes until the binary files land.

Questions? Crack open the serial monitor, read the comments, and don’t be afraid to get weird—but keep it useful.
