#include "battery.h"

#include <Arduino.h>

namespace Battery {
  double m_v = (4.19 - 4.11)/(1277 - 1250);
  double c_v = 4.11;

  double m = (1.00)/(4.10 - 3.5);

  void init() {
    pinMode(BATTERY_SENSE_PIN, INPUT);
    pinMode(CHARGE_SENSE_PIN, INPUT);
  }

  double getVoltage() {
    return  m_v * (analogRead(BATTERY_SENSE_PIN) - 1250) + c_v;
  }

  double getValue() {
    return m*(getVoltage() - 3.5);
  }

  bool isCharging() {
    return !digitalRead(CHARGE_SENSE_PIN);
  }
}