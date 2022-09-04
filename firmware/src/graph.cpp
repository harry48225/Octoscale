#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
#include "graph.h"

namespace Graph {
  float readings[160][2];
  long nextReading = 0;
  int readingInterval = INITIAL_READING_INTERVAL;
  bool running = true;

  unsigned long lastReadingMillis = millis();

  void stop() {
    running = false;
  }

  void resume() {
    running = true;
  }

  void reset() {
    readingInterval = INITIAL_READING_INTERVAL;
    nextReading = 0;
  }

  void update(float newReading) {
    if (!running) return;
    // update reading history for graph
    if (millis() - lastReadingMillis > readingInterval) {
      readings[nextReading][0] = millis();
      readings[nextReading][1] = newReading;
      nextReading++;
      lastReadingMillis = millis();

      // Display is only 160 pixels across so it doesn't really make sense
      // to store more than 160 readings,
      if (nextReading == 160) {
        // Mean all the readings
        for (int i = 0; i<nextReading - 1; i+=2) {
          readings[i/2][0] = (readings[i][0] + readings[i+1][0])/2;
          readings[i/2][1] = (readings[i][1] + readings[i+1][1])/2;
        }

        nextReading=80;
        readingInterval*=2;
      }
    }
  }

  int linearIterpolate(float x, float min, float max, int range) {
    if (max == min) return -1;
    float prop = (x - min)/(max - min);
    //DEBUG_SERIAL.printf("prop: %f, range: %d, prop*range: %f \n", prop, range, prop * (float) range);
    return (int) (prop * (float)range);
  }

  void draw(Adafruit_SH1106G display, int x0, int y0, int width, int height) {
    // Draw the axis
    display.fillRect(x0, y0, width, height, SH110X_BLACK);
    display.drawLine(x0, y0, x0, y0+height, SH110X_WHITE);

    // Preprocess to work out the largest reading
    float smallestX = infinityf();
    float largestX = -1;
    float smallestY = infinityf();
    float largestY = -1;
    for (int i = 0; i < nextReading; i++) {
      smallestX = min(smallestX, readings[i][0]);
      largestX = max(largestX, readings[i][0]);
      smallestY = min(smallestY, readings[i][1]);
      largestY = max(largestY, readings[i][1]);
    }

    //DEBUG_SERIAL.printf("smol x: %f, big x: %f, smol y: %f, big y: %f", smallestX, largestX, smallestY, largestY);
    int x_prev = -1;
    int y_prev = -1;
    // Now scale and plot the points
    for (int i = 0; i < nextReading; i++) {
      int x = linearIterpolate(readings[i][0], smallestX, largestX, width);
      int y = y0 + height - linearIterpolate(readings[i][1], smallestY, largestY, height);
      
      if (x_prev != -1 && y_prev != -1) {
        display.drawLine(x_prev, y_prev, x, y, SH110X_WHITE);
        //DEBUG_SERIAL.printf("plotting at %d, %d \n", x, y);
      }

      x_prev = x;
      y_prev = y;
    }
  }
}