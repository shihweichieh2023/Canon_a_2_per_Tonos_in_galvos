#include <Audio.h>

// Audio objects for audio wave output
AudioSynthWaveform sineX; // Sine wave for X-axis
AudioSynthWaveform sineY; // Sine wave for Y-axis
AudioOutputI2S i2sOutput; // Outputs to the earphone plug
AudioConnection patchCord1(sineX, 0, i2sOutput, 0); // X to Left channel
AudioConnection patchCord2(sineY, 0, i2sOutput, 1); // Y to Right channel

// Define the main motif of the canon (C4 = 261.63 Hz)
float notesX[] = {
  261.63, 293.66, 329.63, 261.63, 329.63, 392.00, 
  349.23, 329.63, 293.66, 261.63, 329.63, 261.63
};

// Define corresponding Y-axis notes for harmony or static projection
float notesY[] = {
  196.00, 220.00, 261.63, 196.00, 261.63, 293.66, 
  261.63, 220.00, 196.00, 220.00, 261.63, 196.00
};

// Define note durations in milliseconds
int durations[] = {
  500, 500, 500, 500, 500, 500, 
  500, 500, 500, 500, 500, 500
};

// Number of notes in the theme
const int numNotes = sizeof(notesX) / sizeof(notesX[0]);

// Modulation step in Hz
float modulationStep = 1.05946; // Corresponds to a semitone increase

void setup() {
  // Initialize the Audio library
  AudioMemory(10); // Allocate memory for audio processing

  // Start with the first note
  playNote(notesX[0], notesY[0]);
}

void loop() {
  // Sequentially play the notes of the theme
  static int currentNote = 0;
  static unsigned long noteStartTime = millis();
  static float modulationFactor = 1.0;

  if (millis() - noteStartTime >= durations[currentNote]) {
    // Move to the next note
    currentNote++;
    if (currentNote >= numNotes) {
      currentNote = 0; // Loop back to the first note

      // Modulate the frequencies upwards by a semitone
      modulationFactor *= modulationStep;
      if (modulationFactor > 4.0) {
        modulationFactor = 1.0; // Reset modulation factor to prevent excessive high frequencies
      }
    }

    // Play the next note with modulation
    playNote(notesX[currentNote] * modulationFactor, notesY[currentNote] * modulationFactor);

    // Record the start time of the new note
    noteStartTime = millis();
  }
}

// Function to set the frequencies for X and Y axes
void playNote(float freqX, float freqY) {
  sineX.begin(0.7f, freqX, WAVEFORM_SINE); // Amplitude 0.5, X-axis frequency
  sineY.begin(0.7f, freqY, WAVEFORM_SINE); // Amplitude 0.5, Y-axis frequency
}
