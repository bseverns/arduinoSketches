# drawBots Motor + Button Control Cheat-Sheet

This sketch pair is basically a lo-fi robot notebook: `dc_motor.ino` spins a pair of DC motors through an H-bridge, while `drawBots.ino` watches two buttons and reports **short** vs **long** presses. Merge them and you've got a slow-and-steady drawbot that can drive, back up, and pivot when you slap a button.

## Pin map + roles

| Signal | Pin | Direction | Notes |
|--------|-----|-----------|-------|
| `Enable1` | D10 | Output | PWM-capable enable for Motor A channel on the H-bridge. Held HIGH in the sample sketch, but you can PWM it for speed control. |
| `Enable2` | D11 | Output | PWM-capable enable for Motor B channel on the H-bridge. Same deal as `Enable1`. |
| `M1` | D2 | Output | Motor A input 1. Drives first motor direction (HIGH for forward in stock code). |
| `M2` | D3 | Output | Motor A input 2. Drives first motor reverse (HIGH for reverse). |
| `M3` | D4 | Output | Motor B input 1. |
| `M4` | D5 | Output | Motor B input 2. |
| `Switch` / `BUTTON1_PIN` | D6 (motor sketch) / D2 (button sketch) | Input | Momentary switch that triggers the "back up + turn" routine. When combining sketches, pick one pin—either keep the motor sketch's D6 or rewire the button sketch to match. |
| `BUTTON2_PIN` | D3 | Input | Optional second button for extra behaviors (long press detection is twice as long as button 1). |

The `ButtonHandler` class in `drawBots.ino` sets each button's pin as an input with the internal pull-up resistor. That means your buttons should short to **GND** when pressed. If you reuse the motor pins (D2/D3) for buttons, remember that they're also motor direction outputs in `dc_motor.ino`; reassign either the button pins or the motor pins so they aren't fighting each other.

## H-bridge + wiring sketch

Think classic dual-channel modules such as **L293D**, **SN754410**, or any 2-channel MOSFET breakout that expects four logic inputs and two enable pins.

```
Arduino D10  ─── Enable A (PWM)      Motor A  ──┐
Arduino D11  ─── Enable B (PWM)      Motor A  ──┘
Arduino D2   ─── Input 1A ───────────────┐
Arduino D3   ─── Input 2A ───────────────┘ (direction for motor A)
Arduino D4   ─── Input 1B ───────────────┐
Arduino D5   ─── Input 2B ───────────────┘ (direction for motor B)
5–9 V supply ── Vmotor (H-bridge) → motors (share ground with Arduino!)
Arduino 5 V  ── Vlogic (if needed)
GNDs all tied together.
```

Buttons: wire from the chosen button pin to one terminal of a momentary switch, other terminal to **GND**. The internal pull-up keeps it HIGH; press drags it LOW.

## How the two sketches dance together

1. **Copy the button handler** from `drawBots.ino` into `dc_motor.ino`.
2. **Pick your pins** so the buttons don't collide with the motor directions. A clean option: keep the motor pins as-is (D2–D5 for direction, D10/D11 for enable) and redefine the button pins to D6 and D7. Update the `ButtonHandler` constructors accordingly.
3. In `loop()`, call `button1.handle()` each cycle. On a **short press**, trigger `backwards()` and `turn()` just like the `if(digitalRead(Switch))` branch used to. Reserve **long presses** or the second button for extra moves—spin, wiggle, pen lift, whatever.
4. Keep the serial prints while prototyping so you can watch the event stream. Kill them once the robot behaves; the motors don't care, but your sanity might.

## Supply + safety notes

* **Voltage:** Those exhibition-friendly gear motors usually like 5–9 V. Feed the H-bridge motor supply with whatever the motors want (check the datasheet) and keep the Arduino on a clean 5 V USB/regulator line.
* **Current:** Cheap dual H-bridges cap out around 600–1 A per channel. If your motors stall harder than that, the chip will thermal-shutdown—or worse, magic smoke. Add fuses or polyfuses if you're feeling responsible.
* **Grounding:** One ground to rule them all. Tie Arduino GND, H-bridge logic GND, motor supply negative, and button ground together or the controls will ghost.

## Punk extensions

* **Speed control:** Swap the constant `digitalWrite(EnableX, HIGH)` with `analogWrite(EnableX, pwmValue)` to throttle the drive. Pair it with long-press actions for slow/fast modes.
* **Multiple behaviors:** Use `button2` (long press threshold is doubled in code) to trigger alternative movement routines—draw spirals, wiggle pens, or pause for dramatic effect.
* **Sensor mashups:** Drop in a light sensor or gyro and blend readings with the button events to auto-trigger turns when hitting a boundary.
* **Fail-safe timeout:** Add a timer that kills the motors if no button presses happen for N seconds—saves you when the bot tries to eat the wall.

Document your chaos, tweak delays (`DELAY` in the button sketch controls poll rate), and keep iterating until the drawing looks like your favorite zine cover.
