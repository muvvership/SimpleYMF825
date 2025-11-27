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

void setup()
{
    YMF825.begin(IOVDD_5V);     // for 5V MCU
    //YMF825.begin(IOVDD_3V3); // for 3.3V MCU

    // Use SAW_LEAD for a classic synthesizer sound
    YMF825.setTone(0, SAW_LEAD);
    YMF825.setMasterVolume(32);

    pinMode(POT_PITCH, INPUT);
    pinMode(POT_VOLUME, INPUT);

    // Start with a note
    YMF825.keyon(0, 4, KEY_C, 15);
}

void loop()
{
    // Read potentiometer values
    int potPitchValue = analogRead(POT_PITCH);
    int potVolumeValue = analogRead(POT_VOLUME);

    // Map pitch pot to cover 3 octaves (36 semitones: 3 octaves × 12 notes)
    // Octaves 3, 4, 5 for a good playable range
    int semitone = map(potPitchValue, 0, 1023, 0, 35); // 36 semitones (0-35)

    // Calculate octave and key from semitone
    int newOctave = 3 + (semitone / 12);  // Octave 3, 4, or 5
    int newKey = semitone % 12;            // Key 0-11

    // Map volume pot to volume range
    int newVolume = map(potVolumeValue, 0, 1023, 0, 31); // Volume 0-31

    // Update pitch if changed
    if (newOctave != currentOctave || newKey != currentKey) {
        currentOctave = newOctave;
        currentKey = newKey;

        // Restart the note with new pitch
        YMF825.keyoff(0);
        delay(5); // Shorter delay for smoother transitions
        YMF825.keyon(0, currentOctave, currentKey, currentVolume >= 0 ? currentVolume : 15);
    }

    // Update volume if changed
    if (newVolume != currentVolume) {
        currentVolume = newVolume;
        YMF825.setVolume(0, currentVolume);
    }

    delay(30); // Shorter delay for more responsive control
}
