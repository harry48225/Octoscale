#include <Arduino.h>
#include "speaker.h"

namespace Speaker {
  void init() {
    pinMode(SPEAKER_PIN, OUTPUT);
  }

  void buttonBeep(unsigned long duration) {
    unsigned long start = millis();

    while (millis() < start + duration) {
      digitalWrite(SPEAKER_PIN, HIGH);
      delay(BUTTON_TONE);
      digitalWrite(SPEAKER_PIN, LOW);
      delay(BUTTON_TONE);
    }
  }
}