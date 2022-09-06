#include <Arduino.h>
#include "timer.h"
#include "bluetooth.h"

namespace Timer {
  bool timing = false;
  unsigned long startTime = 0;
  unsigned long duration = 0;

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