#define BATTERY_SENSE_PIN 17
#define CHARGE_SENSE_PIN 11

namespace Battery {
  void init();

  double getVoltage();
  // Battery fill level from 0 to 1
  double getValue();

  bool isCharging();
}