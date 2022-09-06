#include <Adafruit_SH110X.h>

#define SDA 12
#define SCL 13
#define ADDR 0x3C

namespace Display {
  void init();

  void showMass(double mass, int x = 0, int y = 40);
  void showTimer(long seconds, double mass);
  void showTimerPriming();
  void showTimerWaitingForStart(double mass);
  void showBrewCompleteAnimation();
  void showBrewStats(double mass, unsigned long duration);
  void showAutoTare();
  
  void showCalibrationScreen();
  void showCalibrationCompleteScreen(float factor);

  void clear();
  void show();

  Adafruit_SH1106G getDisplay();
}
