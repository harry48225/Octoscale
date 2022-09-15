#define BUTTON_A_PIN 2
#define BUTTON_B_PIN 9

#define THRESHOLD_A 200
#define THRESHOLD_B 150

namespace Buttons {
  void init();
  void loop();
  bool a();
  bool b();
  unsigned long getDurationPressed();
  
  void waitForRelease();
}