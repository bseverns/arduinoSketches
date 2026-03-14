# Arduino Sketchbook Field Guide

Welcome to the synth lab slash robot garage. This repo is a studio notebook of quick-and-dirty experiments, but now it has a map. Use this README like a zine—skim the spreads, pick a neighborhood, and dive in with intent.

## How this sketchbook is organized
Each folder is an Arduino sketch directory. Drop one into your `sketchbook/` and the IDE will treat it as its own project. Libraries such as **Mozzi** or **Adafruit WaveHC** must already be installed in your Arduino environment.

The collections below are loose crews, not rigid taxonomies; if you discover better alignments, remix away.

If you want to organize by **progression instead of genre**, use [`docs/progression-map.md`](docs/progression-map.md). That map groups folders into likely family lines such as `greyBox_*`, `mozzi_5knob*`, `SKY_PHONE_*`, and other sketch evolutions, which is the safest way to start cleaning a repo this flat.

## New Top-Level Structure
- `experiments/`
  - Families of sketches that clearly represent iterative work.
- `projects/`
  - More complete or installation-oriented builds that have progressed beyond sketch experiments.
- `holding/`
  - Intentionally unspecific buckets for sketches worth keeping together before a better taxonomy is obvious.
- `archive/`
  - Legacy branches and older finished variants that should stay available without dominating the main map.
- `third_party/reference/`
  - Imported libraries, vendor code, and upstream projects kept for reference.
- repo root
  - Legacy standalone sketches and projects that have not been grouped yet.

### Synthesis & Sound Design
Projects that cook tones directly on the microcontroller.
- `experiments/synthesis/mozzi-controls/5knob_MozziSynth/`, `experiments/synthesis/mozzi-controls/mozzi_5knob/`, `experiments/synthesis/mozzi-controls/mozzi_5knob_L/`, `experiments/synthesis/mozzi-controls/mozzi_5knob_L_GB/`, `mozziSynth_glitch/`
  - Mozzi-powered sketches covering FM, glitchy filters, multi-knob control, and the cleaned-up FM showcase in this repo.
- `experiments/synthesis/attiny85-sound/attiny85_random_burst_drone/`, `experiments/synthesis/attiny85-sound/attiny85_dual_clock_clash/`, `experiments/synthesis/attiny85-sound/attiny85_quad_xor_drone/`,
  `experiments/synthesis/attiny85-sound/attiny85_pitch_ramp_randomizer/`, `experiments/synthesis/attiny85-sound/attiny85_three_voice_gate/`, `experiments/synthesis/attiny85-sound/attiny85_scale_sequencer/`,
  `experiments/synthesis/attiny85-sound/attiny85_random_pwm_noise/`, `experiments/synthesis/attiny85-sound/attiny85_pitch_mutator/`, `experiments/synthesis/attiny85-sound/attiny85_drum_trax/`
  - Bare-metal ATTiny85 tone experiments rescued from hash-folders; expect raw timer hacking and button-driven chaos.
- `experiments/synthesis/auduino/auduino/`, `experiments/synthesis/auduino/auduino_v5/`, `experiments/synthesis/auduino/AuduinoSequencer/`
  - Classic granular/noise textures and sequencer riffs.
- `granularSynth/`, `pdVCO/`, `avrPitchSampler/`
  - More experimental oscillators and pitch shifters.

### Samplers, Players & Shields
Boards that sling prerecorded audio or lean on helper hardware.
- `WAVshield/`, `PING_WAV/`, `Ping_baby/`, `PIR_soundie/`
  - SD-card playback patterns, button routing, and motion-triggered samples.
- `experiments/playback/sample-players/SamplerUnoPlayer/`, `experiments/playback/sample-players/SamplerUnoPlayer_test/`, `SampleRateMod/`, `sampleLauncher_neotrellis/`
  - Quick-loading sample players, rate changers, and the NeoTrellis launcher.
- `tiny85audioplayer/`
  - Tiny microcontroller players when you’re short on pins and patience.

### Sensors, Triggers & I/O
- `experiments/sensors-actuators/Blink_baby/`, `experiments/sensors-actuators/trinketInstall_baby/`, `experiments/sensors-actuators/lineIn/`, `experiments/sensors-actuators/baby_switch/`
  - Small hardware-facing sketches for trigger tests, sensor input, actuator output, and early switch prototypes.

### Rhythm, Sequencers & Generative Gizmos
Keep time, break time, or let the machine improvise.
- `experiments/sequencers/ArduinoPunk/`, `experiments/sequencers/SimpleZAR/`, `experiments/sequencers/sixteen_step_sequencer/`
  - Step-sequencer builds with explicit pattern editing, clocking, or step-based control.
- `experiments/algorithms/generativeAlgoSound/`, `experiments/algorithms/oneLineAlgo/`, `experiments/algorithms/_2noise/`
  - Algorithm-first audio and visual studies.
- `experiments/rhythm-boxes/drum8/`, `experiments/rhythm-boxes/ANDI-master/`
  - Drum-machine and loop-box style builds that are rhythm-led but not primarily note sequencers.
- `experiments/playback/shield-players/player_simple_shieldVersion/`, `experiments/playback/shield-players/player_interrupts_ShieldVersion/`
  - IRQ-friendly players useful for tempo-locked sample chops.

### Holding Area
- `holding/uncategorized/glitchamin/`, `holding/uncategorized/Musicator/`, `holding/uncategorized/mP1/`, `holding/uncategorized/IllutronBDemo0_2/`, `holding/uncategorized/Jingle_Hell__Saturnalia_/`, `holding/uncategorized/MozziSynth_example1/`
  - Real sketches worth keeping together, but not yet sorted into a more defensible family.

### Robotics, Kinetics & Control Surfaces
Anything that moves, senses, or bosses motors around.
- `drawBots/`, `drawBots_1/`
  - Button handling patterns and motion control for pen-plotter style bots.
- `dc_motor.ino` inside `drawBots/`, `experiments/robotics/robot-builds/realDRobot/`, `experiments/greybox/greyBox_Growser/`*
  - Reversible H-bridge driving and relay logic for more industrial builds.
- `experiments/robotics/robot-builds/OurRobot_1/`, `experiments/robotics/robot-builds/Our_Robot_01/`, `experiments/robotics/robot-builds/realDRobot/`, `experiments/robotics/relay-control/dbMSr3/`, `experiments/robotics/relay-control/dbSabertoothshield_1/`, `experiments/robotics/relay-control/dbrough/`, `experiments/robotics/relay-control/sP_relay1/`, `experiments/robotics/relay-control/sP_relay2/`, `experiments/robotics/relay-control/sP_relay3/`
  - Robot builds, relay control, and motor-driver experiments that fit better as families than one-offs.

### Named Projects & Installs
- `projects/interactive-installs/energy-light/ENERGY_1/`, `projects/interactive-installs/energy-light/Energy_TEST/`, `projects/interactive-installs/energy-light/Energy_FADE/`
  - A staged light/install line with clear progression in the names.
- `projects/interactive-installs/sky-phone/SKY_PHONE_2/`, `projects/interactive-installs/sky-phone/SKY_PHONE_FIN/`, `projects/interactive-installs/thesis-light/thesisLight/`, `projects/interactive-installs/thesis-light/thesisLightCTRL/`
  - Named installations or more complete interactive builds.
- `projects/personal-builds/mim/MiM_1/`, `projects/personal-builds/sproject/Sproject_1/`
  - Prior personal projects that belong in the project layer rather than the experiment layer.

### Reference Dumps & Vendor Libraries
- `third_party/reference/AdafruitAudio-master/`, `third_party/reference/Adafruit_NeoTrellisM4-master/`, `third_party/reference/Arduinoboy-master/`, `third_party/reference/jan_ostmans_synths-master/`
  - Mostly upstream copies; treat them as reference manuals.
- `third_party/reference/DuinoTune-master/`, `third_party/reference/PJRCAudio-master/`, `third_party/reference/AttinySound-master/`, `third_party/reference/Mozzi/`, `third_party/reference/wavehc/`
  - Utility toolkits that underpin several sketches.

### Notes, Docs & Ephemera
- `docs/`
  - Former stray PDFs now corralled into a single folder.
  - See `docs/showcase.md` for wiring guides + BOMs on the crowd-pleasers.
- `Notes_keke_siren/`, `ReferanceSketch/`, `projects/interactive-installs/sky-phone/SKY_PHONE_2/`, `projects/interactive-installs/sky-phone/SKY_PHONE_FIN/`
  - Mixed notes and iterations. Expect loose ends.
- `archive/legacy-finals/son-build/The_Son_final/`, `archive/legacy-finals/son-build/The_Son_final_a/`
  - Older final-branch snapshots kept around for history, not active browsing.

\* Many of these builds assume external motor drivers, relays, or sensors. Read the header comments in each sketch for wiring specifics before soldering anything permanent.

## Quick-start checklists
1. **Pick a theme above** and copy that folder to your Arduino sketchbook.
2. **Install dependencies** (Mozzi, WaveHC, Sabertooth, etc.) via the Arduino Library Manager or from the copies in `third_party/reference/`.
3. **Check pin assignments** in the `.ino` file comments and adjust for your hardware.
4. **Prototype fast**. This is a playground—fork the sketch, twist a knob, record what happens.

## Where to explore next
- Tighten the best-performing sketches into showcase-ready demos with photos or wiring diagrams. `docs/showcase.md` has TODO slots waiting for media drops.
- Document unexpected tricks (timer sharing, ISR gotchas) directly in the sketches.
- Trade polished recordings for commit history: drop audio snippets or GIFs into `docs/` to complement the code. Call your shot with TODO notes until the binary files land.
- Keep moving low-ambiguity families first; leave one-off sketches at the repo root until the domain or lineage is actually clear.

Questions? Crack open the serial monitor, read the comments, and don’t be afraid to get weird—but keep it useful.
