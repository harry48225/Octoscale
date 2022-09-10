#include <Arduino.h>
#include "timer.h"
#include "bluetooth.h"
#include "display.h"
#include "speaker.h"

namespace Timer {
  bool timing = false;
  unsigned long startTime = 0;
  unsigned long duration = 0;

  void startWithCountDown(int countdownDurationSeconds) {
    int secondsRemaining = countdownDurationSeconds;

    while (secondsRemaining > 0) {
      Display::showCountdown(secondsRemaining);
      Speaker::beep(
        200 + 200 * ((countdownDurationSeconds - secondsRemaining))/countdownDurationSeconds,
        200
      );
      delay(800);
      secondsRemaining--;    
    }

    Speaker::beep(1000, 400);
    start();
  }

  void start() {
    startTime = millis();
    timing = true;
    BLE::startTiming();
  }

  void stop() {
    timing = false;
    BLE::stopTiming();
  }

  unsigned long getSecondsElapsed() {
    if (timing) duration = millis() - startTime;
    return duration / 1000;
  }
}