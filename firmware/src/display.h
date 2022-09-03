#include <Adafruit_SH110X.h>

#define SDA 12
#define SCL 13
#define ADDR 0x3C

namespace Display {
  void init();

  void showMass(double mass);
  void showTimer(long seconds);
  void showTimerPrimed();
  void showBrewCompleteAnimation();
  void showBrewStats(unsigned long duration);
  void showAutoTare();
  
  void showCalibrationScreen();
  void showCalibrationCompleteScreen(float factor);

  void clear();
  void show();

  Adafruit_SH1106G getDisplay();
}
