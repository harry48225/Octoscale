#include <led.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

namespace Leds {
  void init() {
    pixels.begin();
    pixels.show();
  }

  void aTapped() {
    pixels.setPixelColor(1, pixels.Color(230, 76, 0));
  }

  void bTapped() {
    pixels.setPixelColor(2, pixels.Color(230, 76, 0));
  }

  void clear() {
    pixels.clear();
  }

  void show() {
    pixels.show();
  }
}
