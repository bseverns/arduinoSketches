// DUST PRESS preset loader demo — Teensy 4.x + Audio Shield
// Reads presets/presets.json off SD or SerialFlash, clamps each field to the
// control map ranges in the canonical table (docs/USAGE.md), and pushes the values into AudioDustPress setters.
// Wiring cliff notes (Teensy Audio Shield): SD CS = pin 10 (or BUILTIN_SDCARD on 4.1),
// SerialFlash CS = pin 6. Both use SPI and want 3.3V logic. Keep the traces short.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ArduinoJson.h>

#include "DustPress.h"

AudioInputI2S        i2sIn;
AudioDustPress       dustpress;
AudioOutputI2S       i2sOut;
AudioConnection      patchCord1(i2sIn, 0, dustpress, 0);
AudioConnection      patchCord2(i2sIn, 1, dustpress, 1);
AudioConnection      patchCord3(dustpress, 0, i2sOut, 0);
AudioConnection      patchCord4(dustpress, 1, i2sOut, 1);
AudioControlSGTL5000 codec;

#ifndef BUILTIN_SDCARD
constexpr int SD_CS_PIN = 10;
#else
constexpr int SD_CS_PIN = BUILTIN_SDCARD;
#endif
constexpr int SERIALFLASH_CS_PIN = 6;

template <typename T>
T clampValue(T v, T lo, T hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

struct Preset {
  int curveIndex = 0;
  float driveDb = 12.0f;
  float bias = 0.0f;
  float envToDriveDb = 6.0f;
  float gateComp = 0.2f;
  float preTiltDbPerOct = 0.0f;
  float postAirDb = 0.0f;
  float mix = 0.5f;
  float chaos = 0.0f;
  float dirt = 0.1f;
  float ceilingDb = -1.0f;
  float outTrimDb = 0.0f;
  String name = "Untitled";
};

Preset currentPreset;

Preset buildPreset(const JsonObject& obj) {
  Preset p;
  p.name = obj["name"] | "Untitled";
  p.curveIndex = clampValue<int>(obj["curve"] | 0, 0, 3);
  p.driveDb = clampValue<float>(obj["drive_db"] | 12.0f, 0.0f, 36.0f);
  p.bias = clampValue<float>(obj["bias"] | 0.0f, -1.0f, 1.0f);
  p.envToDriveDb = clampValue<float>(obj["env_to_drive_db"] | 6.0f, -12.0f, 12.0f);
  p.gateComp = clampValue<float>(obj["gate_comp"] | 0.2f, 0.0f, 1.0f);
  p.preTiltDbPerOct = clampValue<float>(obj["pre_tilt_db_per_oct"] | 0.0f, -6.0f, 6.0f);
  p.postAirDb = clampValue<float>(obj["post_air_gain_db"] | 0.0f, -6.0f, 6.0f);
  p.mix = clampValue<float>(obj["mix"] | 0.5f, 0.0f, 1.0f);
  p.chaos = clampValue<float>(obj["chaos"] | 0.0f, 0.0f, 7.0f);
  p.dirt = clampValue<float>(obj["dirt"] | 0.1f, 0.0f, 1.0f);
  p.ceilingDb = clampValue<float>(obj["limiter_ceiling_db"] | -1.0f, -6.0f, 0.0f);
  p.outTrimDb = clampValue<float>(obj["out_trim_db"] | 0.0f, -12.0f, 6.0f);
  return p;
}

void applyPreset(const Preset& p) {
  dustpress.setCurveIndex(p.curveIndex);
  dustpress.setDriveDb(p.driveDb);
  dustpress.setBias(p.bias);
  dustpress.setEnvToDriveDb(p.envToDriveDb);
  dustpress.setGateComp(p.gateComp);
  dustpress.setPreTilt(p.preTiltDbPerOct);
  dustpress.setPostAir(p.postAirDb);
  dustpress.setMix(p.mix);
  dustpress.setChaos(p.chaos);
  dustpress.setDirt(p.dirt);
  dustpress.setCeiling(p.ceilingDb);
  dustpress.setOutputTrimDb(p.outTrimDb);
}

bool readFileInto(String& buffer, File& file) {
  if (!file) return false;
  while (file.available()) {
    buffer += (char)file.read();
  }
  file.close();
  return buffer.length() > 0;
}

bool readFileInto(String& buffer, SerialFlashFile& file) {
  if (!file) return false;
  while (file.available()) {
    char chunk[64];
    size_t n = file.read(chunk, sizeof(chunk));
    buffer.concat(chunk, n);
  }
  file.close();
  return buffer.length() > 0;
}

bool loadPresetFromText(const String& jsonText) {
  StaticJsonDocument<4096> doc;
  DeserializationError err = deserializeJson(doc, jsonText);
  if (err) {
    Serial.print("JSON parse failed: ");
    Serial.println(err.c_str());
    return false;
  }

  JsonArray arr = doc.as<JsonArray>();
  if (arr.isNull() || arr.size() == 0) {
    Serial.println("No presets found in array.");
    return false;
  }

  currentPreset = buildPreset(arr[0]);
  applyPreset(currentPreset);
  Serial.print("Loaded preset: ");
  Serial.println(currentPreset.name);
  return true;
}

bool loadFromSD() {
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD not found");
    return false;
  }

  File file = SD.open("/presets/presets.json");
  String jsonText;
  if (!readFileInto(jsonText, file)) {
    Serial.println("SD read failed");
    return false;
  }
  Serial.println("Read presets.json from SD");
  return loadPresetFromText(jsonText);
}

bool loadFromSerialFlash() {
  if (!SerialFlash.begin(SERIALFLASH_CS_PIN)) {
    Serial.println("SerialFlash not found");
    return false;
  }
  if (!SerialFlash.exists("presets/presets.json")) {
    Serial.println("presets/presets.json missing on SerialFlash");
    return false;
  }

  SerialFlashFile file = SerialFlash.open("presets/presets.json");
  String jsonText;
  if (!readFileInto(jsonText, file)) {
    Serial.println("SerialFlash read failed");
    return false;
  }
  Serial.println("Read presets.json from SerialFlash");
  return loadPresetFromText(jsonText);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Dust Press preset loader demo");

  AudioMemory(64);
  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.6);

  if (!loadFromSD()) {
    loadFromSerialFlash();
  }
}

void loop() {
  // In a real build, switch presets on button/encoder and re-run loadPresetFromText
  // with the indexed entry. For now, the first preset is live and the DSP is running.
}
