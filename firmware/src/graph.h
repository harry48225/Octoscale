#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>

#define INITIAL_READING_INTERVAL 20
// Might be more appropriate to make this a class rather than
// a namespace...
namespace Graph {
  void reset();
  void update(float newReading);
  void stop();
  void resume();
  void draw(Adafruit_SH1106G &display, int x0, int y0, int width, int height);
}