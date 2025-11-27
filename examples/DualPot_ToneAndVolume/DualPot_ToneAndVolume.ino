/*
 * DualPot_ToneAndVolume
 *
 * This example demonstrates using two potentiometers to control:
 * - POT 1 (A0): Instrument/Tone selection (16 different instruments)
 * - POT 2 (A1): Volume control (0-31)
 *
 * The sketch plays a C note continuously while you adjust the tone and volume.
 *
 * Hardware connections:
 * - Potentiometer 1: Connect to A0 (for tone selection)
 * - Potentiometer 2: Connect to A1 (for volume control)
 */

#include "SimpleYMF825.h"

SimpleYMF825 YMF825;

const int POT_TONE = A0;    // Potentiometer for tone selection
const int POT_VOLUME = A1;  // Potentiometer for volume control

int currentTone = -1;
int currentVolume = -1;

void setup()
{
    YMF825.begin(IOVDD_5V);     // for 5V MCU
    //YMF825.begin(IOVDD_3V3); // for 3.3V MCU

    // Initialize all 16 tones
    YMF825.setTone( 0, GRAND_PIANO );
    YMF825.setTone( 1, E_PIANO );
    YMF825.setTone( 2, TENOR_SAX );
    YMF825.setTone( 3, PICK_BASS );
    YMF825.setTone( 4, TNKL_BELL );
    YMF825.setTone( 5, NEW_AGE_PD );
    YMF825.setTone( 6, BRIGHT_PIANO );
    YMF825.setTone( 7, VIBES );
    YMF825.setTone( 8, CHURCH_ORGAN );
    YMF825.setTone( 9, FLUTE );
    YMF825.setTone(10, ROCK_ORGAN );
    YMF825.setTone(11, NYLON_GUITER );
    YMF825.setTone(12, SQUARE_LEAD );
    YMF825.setTone(13, SAW_LEAD );
    YMF825.setTone(14, HARPSICHORD );
    YMF825.setTone(15, HARMONICA );

    YMF825.setMasterVolume(32);

    pinMode(POT_TONE, INPUT);
    pinMode(POT_VOLUME, INPUT);

    // Start playing a note on channel 0
    YMF825.keyon(0, 4, KEY_C, 15);
}

void loop()
{
    // Read potentiometer values
    int potToneValue = analogRead(POT_TONE);
    int potVolumeValue = analogRead(POT_VOLUME);

    // Map pot values to ranges
    int newTone = map(potToneValue, 0, 1023, 0, 15);     // 16 tones (0-15)
    int newVolume = map(potVolumeValue, 0, 1023, 0, 31); // Volume range (0-31)

    // Update tone if changed
    if (newTone != currentTone) {
        currentTone = newTone;
        // Need to restart the note with new tone
        YMF825.keyoff(0);
        delay(10);
        YMF825.setTone(0, currentTone);
        YMF825.keyon(0, 4, KEY_C, currentVolume >= 0 ? currentVolume : 15);
    }

    // Update volume if changed
    if (newVolume != currentVolume) {
        currentVolume = newVolume;
        YMF825.setVolume(0, currentVolume);
    }

    delay(50); // Small delay to debounce pot readings
}
