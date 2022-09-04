#include <Arduino.h>
#include "led.h"
#include "debug.h"

namespace Animations {
  namespace Tare {
    unsigned long lastUpdated = 0;
    double led = 2;
    
    void reset() {
      lastUpdated = millis();
      led = 2;
    }

    void update() {
      unsigned long delta = millis() - lastUpdated;
      lastUpdated = millis();
      led += delta / 100.0;
      DEBUG_SERIAL.println(led);

      while (led >= NUMPIXELS) {
        led -= NUMPIXELS;
      }

      Leds::clear();
      Leds::setPixelColourRgb(floor(led), 230, 76, 0);
      Leds::show();
    }
  }
}