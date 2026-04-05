// LineLight-1: Single-lamp, band-limited LED level from line audio
// Target: SparkFun Pro Mini (ATmega328P, 5V/16MHz) flashed via FTDI Basic
//
// This file intentionally reads like a tour guide. Every stage is annotated so you can hand it to a
// manufacturing test tech, a firmware intern, or future-you and they all get the "why" as well as the "what".
// Nothing here is "magic"—the ethos is transparency.

#include <Arduino.h>
#include <arduinoFFT.h>

// -----------------------------------------------------------------------------
// Pin map — keep the numbers here so harness diagrams and firmware match.
// -----------------------------------------------------------------------------
static const uint8_t PIN_AUDIO = A0;   // Audio feed, AC-coupled and biased around VCC/2.
static const uint8_t PIN_POT1  = A1;   // Potentiometer selecting the low edge of the FFT band.
static const uint8_t PIN_POT2  = A2;   // Potentiometer selecting the high edge of the FFT band.
static const uint8_t PIN_PWM   = 9;    // Timer1 OC1A output → MOSFET gate driving the lamp.

// -----------------------------------------------------------------------------
// FFT / DSP constants — tuned for a 9.6 kHz sample rate which plays nicely with the loop timing.
// -----------------------------------------------------------------------------
static const uint16_t FFT_BIN_COUNT = 128;            // 128 keeps the double buffers under 2 KB SRAM on the Pro Mini.
static const float    SAMPLE_RATE_HZ = 9600.0f;       // Set by our crude spin-wait sampler.
static const uint32_t SAMPLE_PERIOD_US =
    static_cast<uint32_t>(1000000.0f / SAMPLE_RATE_HZ + 0.5f);
static const uint16_t ADC_BIAS = 512;                 // 10-bit ADC mid-point after bias network.

// -----------------------------------------------------------------------------
// AGC / smoothing constants — values chosen by ear for a smooth, musical response.
// -----------------------------------------------------------------------------
static const double EMA_ALPHA     = 0.20;
static const double TARGET_LEVEL  = 0.35;
static const double AGC_STEP      = 0.015;
static const double AGC_MIN_GAIN  = 0.05;
static const double AGC_MAX_GAIN  = 200.0;
static const double PWM_GAMMA     = 2.0;

// -----------------------------------------------------------------------------
// Working buffers — global to avoid stack thrash.
// -----------------------------------------------------------------------------
static arduinoFFT FFT;
static double vReal[FFT_BIN_COUNT];
static double vImag[FFT_BIN_COUNT];
static double hannWindow[FFT_BIN_COUNT];

// -----------------------------------------------------------------------------
// State for the slow-control loops.
// -----------------------------------------------------------------------------
static double emaEnergy = 0.0;
static double agcGain   = 1.0;

void setupFastPWM31kHz();
inline void pwmWrite(uint8_t duty) { OCR1A = duty; }

void primeHannWindow();
void acquireWindowedSamples();
void performFft();

struct BandSelection {
  uint16_t binLo;
  uint16_t binHi;
};

BandSelection readBandSelection();
double computeBandEnergy(uint16_t binLo, uint16_t binHi);
double normalizeEnergy(double rawEnergy);
uint8_t renderDutyFromLevel(double level);
void logDebugOncePerSecond(const BandSelection& band, double level);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PWM, OUTPUT);

#if defined(analogReadResolution)
  analogReadResolution(10);
#endif
  analogReference(DEFAULT);

  primeHannWindow();
  setupFastPWM31kHz();
  pwmWrite(0);

  delay(50);
  Serial.println(F("LineLight-1 boot"));
  Serial.print(F("Fs=")); Serial.println(SAMPLE_RATE_HZ);
  Serial.print(F("N="));  Serial.println(FFT_BIN_COUNT);
}

void loop() {
  acquireWindowedSamples();
  performFft();
  BandSelection band = readBandSelection();
  double bandEnergy = computeBandEnergy(band.binLo, band.binHi);
  double normalized = normalizeEnergy(bandEnergy);
  uint8_t duty = renderDutyFromLevel(normalized);
  pwmWrite(duty);
  logDebugOncePerSecond(band, normalized);
}

void primeHannWindow() {
  for (uint16_t n = 0; n < FFT_BIN_COUNT; ++n) {
    const double phase = (2.0 * PI * n) / (FFT_BIN_COUNT - 1);
    hannWindow[n] = 0.5 * (1.0 - cos(phase));
  }
}

void acquireWindowedSamples() {
  uint32_t nextSampleMicros = micros();
  for (uint16_t i = 0; i < FFT_BIN_COUNT; ++i) {
    while (static_cast<int32_t>(micros() - nextSampleMicros) < 0) {
    }
    nextSampleMicros += SAMPLE_PERIOD_US;

    int raw = analogRead(PIN_AUDIO);
    double centered = static_cast<double>(raw) - ADC_BIAS;
    vReal[i] = centered * hannWindow[i];
    vImag[i] = 0.0;
  }
}

void performFft() {
  FFT.Windowing(vReal, FFT_BIN_COUNT, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, FFT_BIN_COUNT, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, FFT_BIN_COUNT);
}

BandSelection readBandSelection() {
  const uint16_t nyquistBin = (FFT_BIN_COUNT / 2) - 1;

  int pot1 = analogRead(PIN_POT1);
  int pot2 = analogRead(PIN_POT2);

  double loRatio = constrain(static_cast<double>(pot1) / 1023.0, 0.0, 1.0);
  double hiRatio = constrain(static_cast<double>(pot2) / 1023.0, 0.0, 1.0);

  uint16_t binLo = 1 + static_cast<uint16_t>(loRatio * (nyquistBin - 2));
  uint16_t binHi = binLo + 1 + static_cast<uint16_t>(hiRatio * (nyquistBin - binLo - 1));
  binHi = constrain(binHi, static_cast<uint16_t>(binLo + 1), nyquistBin);

  return {binLo, binHi};
}

double computeBandEnergy(uint16_t binLo, uint16_t binHi) {
  double accumulator = 0.0;
  for (uint16_t bin = binLo; bin <= binHi; ++bin) {
    double magnitude = vReal[bin];
    accumulator += magnitude * magnitude;
  }

  const uint16_t binCount = (binHi - binLo + 1);
  return accumulator / static_cast<double>(binCount);
}

double normalizeEnergy(double rawEnergy) {
  emaEnergy = (1.0 - EMA_ALPHA) * emaEnergy + EMA_ALPHA * rawEnergy;

  double normalizedMeasure = min(1.0, emaEnergy * agcGain);
  double error = TARGET_LEVEL - normalizedMeasure;
  agcGain *= (1.0 + AGC_STEP * error);
  agcGain = constrain(agcGain, AGC_MIN_GAIN, AGC_MAX_GAIN);

  double leveled = sqrt(emaEnergy * agcGain);
  return constrain(leveled, 0.0, 1.0);
}

uint8_t renderDutyFromLevel(double level) {
  double gammaCorrected = pow(level, PWM_GAMMA);
  gammaCorrected = constrain(gammaCorrected, 0.0, 1.0);
  return static_cast<uint8_t>(gammaCorrected * 255.0 + 0.5);
}

void logDebugOncePerSecond(const BandSelection& band, double level) {
  static uint32_t lastLogMs = 0;
  if (millis() - lastLogMs < 1000) {
    return;
  }
  lastLogMs = millis();

  const double binWidthHz = SAMPLE_RATE_HZ / static_cast<double>(FFT_BIN_COUNT);
  const double loHz = band.binLo * binWidthHz;
  const double hiHz = band.binHi * binWidthHz;

  Serial.print(F("bins "));
  Serial.print(band.binLo);
  Serial.print('-');
  Serial.print(band.binHi);
  Serial.print(F(" ("));
  Serial.print(loHz, 0);
  Serial.print('-');
  Serial.print(hiHz, 0);
  Serial.print(F(" Hz) "));
  Serial.print(F("level "));
  Serial.print(level, 3);
  Serial.print(F("  agc "));
  Serial.print(agcGain, 3);
  Serial.print(F("  ema "));
  Serial.println(emaEnergy, 3);
}

void setupFastPWM31kHz() {
  pinMode(PIN_PWM, OUTPUT);

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1A |= _BV(WGM10);
  TCCR1B |= _BV(CS10);
  TCCR1A |= _BV(COM1A1);

  OCR1A = 0;
}
