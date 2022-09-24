#include <led.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

namespace Leds {
  Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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

  void setPixelColourRgb(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    pixels.setPixelColor(n, pixels.Color(r, g, b));
  }

  void fillPixelsRgb(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < NUMPIXELS; i++) {
      setPixelColourRgb(i, r, g, b);
    } 
  }

  uint8_t getBrightness() {
    return pixels.getBrightness();
  }

  void setBrightness(uint8_t brightness) {
    pixels.setBrightness(brightness);
  }

  void clear() {
    pixels.clear();
  }

  void show() {
    pixels.show();
  }
}
