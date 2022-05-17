#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>

int linearIterpolate(float x, float min, float max, int range) {
  if (max == min) return -1;
  float prop = (x - min)/(max - min);
  Serial.printf("prop: %f, range: %d, prop*range: %f \n", prop, range, prop * (float) range);
  return (int) (prop * (float)range);
}

void drawGraph(Adafruit_SH1107 &display, float points[][2], int numberOfPoints, int x0, int y0, int width, int height) {
  // Draw the axis
  display.fillRect(x0, y0, width, height, SH110X_BLACK);
  display.drawLine(x0, y0, x0, y0+height, SH110X_WHITE);

  // Preprocess to work out the largest reading
  float smallestX = infinityf();
  float largestX = -1;
  float smallestY = infinityf();
  float largestY = -1;
  for (int i = 0; i < numberOfPoints; i++) {
    smallestX = min(smallestX, points[i][0]);
    largestX = max(largestX, points[i][0]);
    smallestY = min(smallestY, points[i][1]);
    largestY = max(largestY, points[i][1]);
  }

  Serial.printf("smol x: %f, big x: %f, smol y: %f, big y: %f", smallestX, largestX, smallestY, largestY);
  int x_prev = -1;
  int y_prev = -1;
  // Now scale and plot the points
  for (int i = 0; i < numberOfPoints; i++) {
    int x = linearIterpolate(points[i][0], smallestX, largestX, width);
    int y = y0 + height - linearIterpolate(points[i][1], smallestY, largestY, height);
    
    if (x_prev != -1 && y_prev != -1) {
      display.drawLine(x_prev, y_prev, x, y, SH110X_WHITE);
      Serial.printf("plotting at %d, %d \n", x, y);
    }

    x_prev = x;
    y_prev = y;
  }
}