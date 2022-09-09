#include <Arduino.h>
#include "speaker.h"

namespace Speaker {
  float volume = 0;
  int frequency = 0;

  void init() {
    ledcSetup(CHANNEL, 2000, RESOLUTION);
    ledcAttachPin(SPEAKER_PIN, CHANNEL);
    setVolume(0.1);
  }

  void clear() {
    frequency = 0;
  }

  void sound() {
    if (frequency > 0) {
      ledcWriteTone(CHANNEL, frequency);
      ledcWrite(CHANNEL, 128.0 * volume);
    } else {
      ledcWriteTone(CHANNEL, 0);
      ledcWrite(CHANNEL, 0);
    }
  }

  void setVolume(float newVolume) {
    volume = newVolume;
  }

  void buttonBeep(unsigned long duration) {
    frequency = 1000;
  }
}