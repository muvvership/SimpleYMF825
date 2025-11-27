/*
 * DualPot_Theremin
 *
 * This example creates a theremin-like instrument using two potentiometers:
 * - POT 1 (A0): Pitch control (continuous across 3 octaves)
 * - POT 2 (A1): Volume control (0-31)
 *
 * The sketch provides smooth pitch transitions across octaves 3-5
 * and real-time volume control, mimicking a theremin's operation.
 * Uses the SAW_LEAD instrument for a classic synthesizer sound.
 *
 * Hardware connections:
 * - Potentiometer 1: Connect to A0 (for pitch control)
 * - Potentiometer 2: Connect to A1 (for volume control)
 */

#include "SimpleYMF825.h"

SimpleYMF825 YMF825;

const int POT_PITCH = A0;   // Potentiometer for pitch control
const int POT_VOLUME = A1;  // Potentiometer for volume control

int currentOctave = -1;
int currentKey = -1;
int currentVolume = -1;
int lastPotPitch = -1;
int lastPotVolume = -1;

void setup()
{
    Serial.begin(9600);

    YMF825.begin(IOVDD_5V);     // for 5V MCU
    //YMF825.begin(IOVDD_3V3); // for 3.3V MCU

    // Use SAW_LEAD for a classic synthesizer sound
    YMF825.setTone(0, SAW_LEAD);
    YMF825.setMasterVolume(32);

    // Note: No pinMode needed for analog inputs

    Serial.println("DualPot_Theremin started");
}

void loop()
{
    // Read potentiometer values
    int potPitchValue = analogRead(POT_PITCH);
    int potVolumeValue = analogRead(POT_VOLUME);

    // Add hysteresis - only process if pot moved significantly (reduces jitter)
    const int HYSTERESIS = 5;
    bool pitchChanged = (lastPotPitch == -1) || (abs(potPitchValue - lastPotPitch) > HYSTERESIS);
    bool volumeChanged = (lastPotVolume == -1) || (abs(potVolumeValue - lastPotVolume) > HYSTERESIS);

    if (!pitchChanged && !volumeChanged) {
        delay(10);
        return;
    }

    if (pitchChanged) lastPotPitch = potPitchValue;
    if (volumeChanged) lastPotVolume = potVolumeValue;

    // Map pitch pot to cover 3 octaves (36 semitones: 3 octaves × 12 notes)
    // Octaves 3-5 (both setKey and keyon use range 1-8, despite what README says)
    int semitone = map(potPitchValue, 0, 1023, 0, 35); // 36 semitones (0-35)

    // Calculate octave and key from semitone
    int newOctave = 3 + (semitone / 12);  // Octave 3, 4, or 5
    int newKey = semitone % 12;            // Key 0-11

    // Map volume pot to volume range
    int newVolume = map(potVolumeValue, 0, 1023, 0, 31); // Volume 0-31

    // Debug output
    Serial.print("Pitch pot: ");
    Serial.print(potPitchValue);
    Serial.print(" -> Oct: ");
    Serial.print(newOctave);
    Serial.print(" Key: ");
    Serial.print(newKey);
    Serial.print(" | Vol pot: ");
    Serial.print(potVolumeValue);
    Serial.print(" -> Vol: ");
    Serial.println(newVolume);

    // Initialize on first run
    if (currentOctave == -1) {
        currentOctave = newOctave;
        currentKey = newKey;
        currentVolume = newVolume;
        Serial.println("*** INIT: Starting note");
        YMF825.setVolume(0, currentVolume);
        YMF825.keyon(0, currentOctave, currentKey);
    }

    // Update pitch if changed - use setKey to change pitch without restarting note
    if (newOctave != currentOctave || newKey != currentKey) {
        currentOctave = newOctave;
        currentKey = newKey;

        Serial.println("*** PITCH CHANGE");
        YMF825.setKey(0, currentOctave, currentKey);
    }

    // Update volume if changed
    if (newVolume != currentVolume) {
        currentVolume = newVolume;
        Serial.println("*** VOLUME CHANGE");
        YMF825.setVolume(0, currentVolume);
    }

    delay(10);
}
