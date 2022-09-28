#define BATTERY_SENSE_PIN 17
#define CHARGE_SENSE_PIN 11
#define POWER_SWITCH_SENSE_PIN 47
#define CUTOFF_VOLTAGE 3.0

namespace Battery {
  void init();

  double getVoltage();

  bool isCharging();
  bool isPowerSwitchOn();
}