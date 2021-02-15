#include "Tunes.h"

#include "Pitches.h"

#include <Arduino.h>


void playTurnOnTune(const int pin) {
  tone(pin, NOTE_E4);
  delay(100);

  tone(pin, NOTE_C4);
  delay(100);

  tone(pin, NOTE_G4);
  delay(100);

  noTone(pin);
}

void playStartTimerTune(const int pin) {
  tone(pin, NOTE_C4);
  delay(100);

  tone(pin, NOTE_G4);
  delay(100);

  noTone(pin);
}

void playInterruptTimerTune(const int pin) {
  tone(pin, NOTE_G4);
  delay(100);

  tone(pin, NOTE_C4);
  delay(100);

  noTone(pin);
}
