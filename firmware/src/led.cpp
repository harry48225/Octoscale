#include <led.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

namespace LEDS {
  void init() {
    pixels.begin();
    pixels.rainbow();
    pixels.show();
  }
}
