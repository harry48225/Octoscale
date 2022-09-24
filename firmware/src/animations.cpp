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

  namespace Charging {
    unsigned long lastUpdated = 0;
    int brightness = 0;
    bool increasing = true;
    
    void reset() {
      lastUpdated = millis();
      brightness = 0;
      increasing = true;
    }

    void update() {
      unsigned long deltaTime = millis() - lastUpdated;
      lastUpdated = millis();
      auto delta = deltaTime / 10;
      if (increasing) {
        brightness += delta;
        if (brightness > 255) {
          brightness = 255;
          increasing = false;
        }
      } else {
        brightness -= delta;
        if (brightness <= 0) {
          brightness = 0;
          increasing = true;
        }
      }

      auto oldBrightness = Leds::getBrightness();
      Leds::clear();
      Leds::fillPixelsRgb(230, 76, 0);
      Leds::setBrightness(brightness);
      Leds::show();
      Leds::setBrightness(oldBrightness);
    }
  }
}