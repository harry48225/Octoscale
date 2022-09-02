#define BUTTON_A_PIN 2
#define BUTTON_B_PIN 9

#define A_THRESHOLD 24900
#define B_THRESHOLD 30700

namespace Buttons {
  void init();
  void loop();
  bool a();
  bool b();
}