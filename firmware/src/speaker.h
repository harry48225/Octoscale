#define SPEAKER_PIN 3
#define BUTTON_TONE 4

namespace Speaker {
  void init();
  void buttonBeep(unsigned long duration = 100);
}