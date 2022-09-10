#include <Arduino.h>
#include "timer.h"
#include "bluetooth.h"
#include "display.h"
#include "speaker.h"
#include "led.h"

namespace Timer {
  bool timing = false;
  unsigned long startTime = 0;
  unsigned long duration = 0;

  void startWithCountDown(int countdownDurationSeconds) {
    int secondsRemaining = countdownDurationSeconds;

    while (secondsRemaining > 0) {
      Display::showCountdown(secondsRemaining);

      if (secondsRemaining == 1) {
        Leds::fillPixelsRgb(255, 85, 0);
        Leds::show();
        Speaker::beep(750, 200);
      } else if (secondsRemaining == 2) {
        Leds::fillPixelsRgb(255, 0, 0);
        Leds::show();
        Speaker::beep(500, 200);
      } else {
        Leds::fillPixelsRgb(255, 0, 0);
        Leds::show();
        delay(200);
      }
      delay(800);
      secondsRemaining--;    
    }

    Leds::fillPixelsRgb(0, 255, 0);
    Leds::show();
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