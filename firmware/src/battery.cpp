#include "battery.h"

#include <Arduino.h>

namespace Battery {
  double m = (4.19 - 4.11)/(1277 - 1250);
  double c = 4.11;

  void init() {
    pinMode(BATTERY_SENSE_PIN, INPUT);
  }

  double getVoltage() {
    return  m * (analogRead(BATTERY_SENSE_PIN) - 1250) + c;
  }
}