#define SPEAKER_PIN 3
#define CHANNEL 0
#define RESOLUTION 8

namespace Speaker {
  void init();
  void clear();
  void sound();
  void setVolume(float volume);
  void buttonBeep();

  void beep(long frequency, long duration = 200);
}