# Repo Progression Map

This repo is easiest to clean up if it gets organized by **stage of work** first, not by perfect topic. The goal is to answer:

1. What is an early experiment?
2. What is a later variation of the same idea?
3. What is a stable project worth keeping easy to find?
4. What is just upstream reference material?

## Stage Labels

- `seed`: first-pass sketch, rough proof of concept, often single-purpose
- `variant`: a later branch of the same idea with one or two major changes
- `project`: more complete build or a sketch tied to a specific device/install
- `reference`: imported library, vendor repo, or external example set
- `artifact`: generated output that should not drive repo structure

## Likely Progression Families

These are the clusters that already read like evolving lines of work.

| Family | Likely progression | Notes |
| --- | --- | --- |
| GreyBox | `experiments/greybox/greyBox_instructableEx` -> `experiments/greybox/greyBox_Growser` -> `experiments/greybox/greyBox_GrowserSwitch` -> `experiments/greybox/greyBox_Growser_delay` | First family already moved out of the repo root. Use it as the pattern for similar lineages. |
| Mozzi controls | `experiments/synthesis/mozzi-controls/5knob_MozziSynth` -> `experiments/synthesis/mozzi-controls/mozzi_5knob` -> `experiments/synthesis/mozzi-controls/mozzi_5knob_L` -> `experiments/synthesis/mozzi-controls/mozzi_5knob_L_GB` | Now grouped under the synthesis domain. |
| Auduino | `experiments/synthesis/auduino/auduino` -> `experiments/synthesis/auduino/auduino_v5` -> `experiments/synthesis/auduino/AuduinoSequencer` | Straightforward lineage, now grouped as one family. |
| Sampler player | `experiments/playback/sample-players/SamplerUnoPlayer` -> `experiments/playback/sample-players/SamplerUnoPlayer_test` | Now grouped under playback experiments. |
| Shield player | `experiments/playback/shield-players/player_simple_shieldVersion` -> `experiments/playback/shield-players/player_interrupts_ShieldVersion` | Same hardware direction, later ISR-aware variant. |
| Draw bots | `drawBots` -> `drawBots_1` | Probably a revision, not a separate category. |
| Robot builds | `experiments/robotics/robot-builds/OurRobot_1` -> `experiments/robotics/robot-builds/Our_Robot_01` -> `experiments/robotics/robot-builds/realDRobot` | Naming suggests evolving robot/control work. |
| Relay control | `experiments/robotics/relay-control/dbMSr3` -> `experiments/robotics/relay-control/dbSabertoothshield_1` -> `experiments/robotics/relay-control/dbrough` plus `experiments/robotics/relay-control/sP_relay1-3` | Hardware-control sketches grouped as one robotics subdomain. |
| Energy / light | `projects/interactive-installs/energy-light/ENERGY_1` -> `projects/interactive-installs/energy-light/Energy_TEST` -> `projects/interactive-installs/energy-light/Energy_FADE` | Promoted into projects because the names read like staged install variants. |
| Stage-light control | `projects/interactive-installs/thesis-light/thesisLight` -> `projects/interactive-installs/thesis-light/thesisLightCTRL` | Promoted into projects because the names read more like installation builds than experiments. |
| Sky phone | `projects/interactive-installs/sky-phone/SKY_PHONE_2` -> `projects/interactive-installs/sky-phone/SKY_PHONE_FIN` | Reads like iteration to a more complete install build. |
| MiM | `projects/personal-builds/mim/MiM_1` | User-identified prior project; keep it in the project layer. |
| Sproject | `projects/personal-builds/sproject/Sproject_1` | User-identified prior project; keep it in the project layer. |
| Son build | `archive/legacy-finals/son-build/The_Son_final` -> `archive/legacy-finals/son-build/The_Son_final_a` | Archived final-branch snapshots kept for history, not active development. |
| Roller | `Roller` -> `ROLLER_f` | Early concept plus later PlatformIO-style project. |

## Standalone Experiment Clusters

These do not yet show a clear lineage, but they already belong together conceptually.

### ATTiny sound experiments

`experiments/synthesis/attiny85-sound/attiny85_drum_trax`, `experiments/synthesis/attiny85-sound/attiny85_dual_clock_clash`, `experiments/synthesis/attiny85-sound/attiny85_pitch_mutator`, `experiments/synthesis/attiny85-sound/attiny85_pitch_ramp_randomizer`, `experiments/synthesis/attiny85-sound/attiny85_quad_xor_drone`, `experiments/synthesis/attiny85-sound/attiny85_random_burst_drone`, `experiments/synthesis/attiny85-sound/attiny85_random_pwm_noise`, `experiments/synthesis/attiny85-sound/attiny85_scale_sequencer`, `experiments/synthesis/attiny85-sound/attiny85_three_voice_gate`, `tiny85audioplayer`

### One-off synth sketches

`pdVCO`, `granularSynth`, `avrPitchSampler`, `mozziSynth_glitch`

### Algorithm-first studies

`experiments/algorithms/generativeAlgoSound`, `experiments/algorithms/oneLineAlgo`, `experiments/algorithms/_2noise`

### Sequencer studies

`experiments/sequencers/ArduinoPunk`, `experiments/sequencers/SimpleZAR`, `experiments/sequencers/sixteen_step_sequencer`

### Rhythm-box studies

`experiments/rhythm-boxes/drum8`, `experiments/rhythm-boxes/ANDI-master`

### Sample / trigger installations

`PING_WAV`, `Ping_baby`, `PIR_soundie`, `sampleLauncher_neotrellis`, `WAVshield`, `SampleRateMod`

### Sensor / actuator studies

`experiments/sensors-actuators/Blink_baby`, `experiments/sensors-actuators/trinketInstall_baby`, `experiments/sensors-actuators/lineIn`, `experiments/sensors-actuators/baby_switch`

### Holding area

`holding/uncategorized/glitchamin`, `holding/uncategorized/Musicator`, `holding/uncategorized/mP1`, `holding/uncategorized/IllutronBDemo0_2`, `holding/uncategorized/Jingle_Hell__Saturnalia_`, `holding/uncategorized/MozziSynth_example1`

### Modern PlatformIO demo snapshots

`holding/modern-platformio-demos/moarknobs_button_ef_usb_midi_demo`, `holding/modern-platformio-demos/diceloop_rough_prototype`, `holding/modern-platformio-demos/hallway_reactor_2m`, `holding/modern-platformio-demos/linelight_band_lamp`, `holding/modern-platformio-demos/motorlightsound_lane_controller`, `holding/modern-platformio-demos/dust_press_minimal`, `holding/modern-platformio-demos/horizon_minimal`, `holding/modern-platformio-demos/rhythm_weave_minimal`, `holding/modern-platformio-demos/tide_engine_minimal`, `holding/modern-platformio-demos/orbit_looper_minimal`, `holding/modern-platformio-demos/silt_minimal`, `holding/modern-platformio-demos/dust_press_presets_demo`, `holding/modern-platformio-demos/horizon_preset_morph`, `holding/modern-platformio-demos/horizon_scope_demo`, `holding/modern-platformio-demos/tide_engine_teaching_rig`, `holding/modern-platformio-demos/tide_engine_humanize_demo`

### Motion / relay / mechanism control

`experiments/robotics/relay-control/dbSabertoothshield_1`, `experiments/robotics/relay-control/dbMSr3`, `experiments/robotics/relay-control/dbrough`, `experiments/robotics/relay-control/sP_relay1`, `experiments/robotics/relay-control/sP_relay2`, `experiments/robotics/relay-control/sP_relay3`

## Reference and Vendor Material

These folders should eventually live together because they are support material, not your main sketch history.

`third_party/reference/AdafruitAudio-master`, `third_party/reference/Adafruit_NeoTrellisM4-master`, `third_party/reference/Arduinoboy-master`, `third_party/reference/AttinySound-master`, `third_party/reference/Beat707-Projects-master`, `third_party/reference/DuinoTune-master`, `third_party/reference/Lil_Soundie_Audio_Player-master`, `third_party/reference/Mozzi`, `third_party/reference/PJRCAudio-master`, `third_party/reference/arpeggiator-master`, `third_party/reference/drumkid-master`, `third_party/reference/jan_ostmans_synths-master`, `third_party/reference/mosquito_one-master`, `third_party/reference/wavehc`

## Cleanup Targets

These are structure-noise, not meaningful project history.

- `*/applet/`
- `.DS_Store`
- `.vscode/`
- `*.o`, `*.eep`, `*.elf`, `*.hex`
- local zip snapshots like the ones inside `Roller/`

## Recommended First Physical Reorg

Do this in small passes, not one giant move:

1. Completed: move vendor/reference folders into `third_party/reference/`.
2. Completed: group the GreyBox lineage under `experiments/greybox/`.
3. Completed: group the Mozzi controls, Auduino sketches, ATTiny85 sound line, sampler/player variants, robot builds, and installation-style project lines into domain folders.
4. Completed: group relay-control sketches, promote the energy/light line into projects, and archive the legacy `The_Son_final*` branch.
5. Completed: move the clearest algorithm-first sketches into `experiments/algorithms/`.
6. Completed: move the clearest sequencer-led sketches into `experiments/sequencers/`.
7. Completed: group the clearest drum-machine and loop-box sketches into `experiments/rhythm-boxes/`.
8. Completed: move the remaining low-ambiguity sensor/input/output oddities into `experiments/sensors-actuators/`.
9. Completed: move `MiM_1` and `Sproject_1` into the project layer after user clarification that they are prior projects.
10. Completed: move the remaining hard-to-classify sketches into `holding/uncategorized/` so they stop cluttering the repo root.
11. Next: only split items back out of `holding/` when a better domain is actually clear.
12. Later: leave ambiguous one-offs in `holding/` until there is enough context to classify them cleanly.
13. Last: move generated output and old exports into `archive/` only after confirming nothing still depends on them.

## Naming Rule Going Forward

For new work, prefer:

- `family/seed-name`
- `family/v2-name`
- `family/final-name`

or, if you want to stay flat for now:

- `family_01_seed`
- `family_02_variant`
- `family_90_final`

That gives you sortable history without needing to guess chronology from names later.
