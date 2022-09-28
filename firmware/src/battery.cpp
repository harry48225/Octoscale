#include "battery.h"

#include <Arduino.h>

namespace Battery {
  double m_v = (4.19 - 3.64)/(1277 - 1107);
  double c_v = 3.64;

  void init() {
    pinMode(BATTERY_SENSE_PIN, INPUT);
    pinMode(CHARGE_SENSE_PIN, INPUT);
    pinMode(POWER_SWITCH_SENSE_PIN, INPUT);
  }

  double getVoltage() {
    return  m_v * (analogRead(BATTERY_SENSE_PIN) - 1107) + c_v;
  }

  bool isCharging() {
    return analogRead(CHARGE_SENSE_PIN) < 200;
  }

  bool isPowerSwitchOn() {
    return digitalRead(POWER_SWITCH_SENSE_PIN);
  }
}