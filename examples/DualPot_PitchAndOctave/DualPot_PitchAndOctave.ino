/*
 * DualPot_PitchAndOctave
 *
 * This example demonstrates using two potentiometers to control:
 * - POT 1 (A0): Note pitch (C, C#, D, D#, E, F, F#, G, G#, A, A#, B)
 * - POT 2 (A1): Octave (1-8)
 *
 * The sketch continuously plays the selected note at the selected octave
 * using the Grand Piano instrument.
 *
 * Hardware connections:
 * - Potentiometer 1: Connect to A0 (for pitch selection)
 * - Potentiometer 2: Connect to A1 (for octave selection)
 */

#include "SimpleYMF825.h"

SimpleYMF825 YMF825;

const int POT_PITCH = A0;   // Potentiometer for pitch selection
const int POT_OCTAVE = A1;  // Potentiometer for octave selection

int currentPitch = -1;
int currentOctave = -1;

void setup()
{
    YMF825.begin(IOVDD_5V);     // for 5V MCU
    //YMF825.begin(IOVDD_3V3); // for 3.3V MCU

    YMF825.setTone(0, GRAND_PIANO);
    YMF825.setMasterVolume(32);
    YMF825.setVolume(0, 31);

    // Note: No pinMode needed for analog inputs
}

void loop()
{
    // Read potentiometer values
    int potPitchValue = analogRead(POT_PITCH);
    int potOctaveValue = analogRead(POT_OCTAVE);

    // Map pot values to ranges
    int newPitch = map(potPitchValue, 0, 1023, 0, 11);  // 12 notes (0-11)
    int newOctave = map(potOctaveValue, 0, 1023, 1, 8); // Octaves 1-8

    // Initialize on first run
    if (currentPitch == -1) {
        currentPitch = newPitch;
        currentOctave = newOctave;
        YMF825.keyon(0, currentOctave, currentPitch);
    }

    // Update note if pitch or octave changed - use setKey to change smoothly
    if (newPitch != currentPitch || newOctave != currentOctave) {
        currentPitch = newPitch;
        currentOctave = newOctave;

        YMF825.setKey(0, currentOctave, currentPitch);
    }

    delay(50); // Small delay to debounce pot readings
}
