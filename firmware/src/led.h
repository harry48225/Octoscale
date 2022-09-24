#include <Adafruit_NeoPixel.h>

#define PIN 1 
#define NUMPIXELS 9

namespace Leds {
  void init();
  void aTapped();
  void bTapped();

  void setPixelColourRgb(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void fillPixelsRgb(uint8_t r, uint8_t g, uint8_t b);

  uint8_t getBrightness();
  void setBrightness(uint8_t brightness);
  
  void clear();
  void show();
}