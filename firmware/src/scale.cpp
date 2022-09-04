#include "scale.h"
#include "Arduino.h"
#include "HX711.h"
#include "led.h"
#include "animations.h"

Scale::Scale(int doutPin, int sckPin) {
  loadCell.set_scale(4411.92);
  loadCell.begin(doutPin, sckPin);
  loadCell.tare();
}

double Scale::getReading() {
  return smoothedReading - offset;
}

double Scale::getLastSettledReading() {
  return lastSettledReading - offset;
}

void Scale::tare() {
  Leds::clear();
  Leds::show();
  Animations::Tare::reset();

  unsettle();
  unsigned long start = millis();
  while (!hasSettled || millis() - start < 2000) {
    updateReading();
    Animations::Tare::update();
    delay(10);
  }
  
  offset = smoothedReading;
  lastSettledReading = offset;
}

void Scale::tareLoadCell() {
  offset = 0;
  loadCell.tare();
}

void Scale::setScale() {
  loadCell.set_scale();
}

void Scale::setScale(float factor) {
  loadCell.set_scale(factor);
}

float Scale::getUnits(int samples) {
  return loadCell.get_units(samples);
}

void Scale::unsettle() {
  hasSettled = false;
}

double Scale::smoothingFactor(double t_e, double cutoff) {
  double r = 2 * PI * cutoff * t_e;
  return r / (r+1);
}

double Scale::exponentialSmoothing(double a, double x, double x_prev) {
  return a * x + (1 - a) * x_prev;
}

void Scale::updateReading() {
  long t = micros();
  double x = getUnits(1);
  double t_e = (t - t_prev)/1e6;
  // Filtered derivative
  double a_d = smoothingFactor(t_e, D_CUTOFF);
  double dx = (x - x_prev) / t_e;
  double dx_hat = exponentialSmoothing(a_d, dx, dx_prev);

  // Filtered signal
  double cutoff = (MIN_CUTOFF) + (BETA * abs(dx_hat));
  double a = smoothingFactor(t_e, cutoff);
  double x_hat = exponentialSmoothing(a, x, x_prev);

  Serial.printf("\r x: %.1f x_hat: %.1f, dx_hat: %.1f, last_settled: %.1f                                         ", x, x_hat, dx_hat, lastSettledReading);
  if (abs(dx_hat) < SETTLED_TOLERANCE) {
    if (!hasSettled) {
      millisBetweenSettledReadings = (millis() - lastSettledMillis);
      lastSettledMillis = millis();
    }
    
    hasSettled = true;
  } else {
    if (abs(dx_hat) > UNSETTLED_TOLERANCE && hasSettled) {
      lastSettledReading = smoothedReading;
      hasSettled = false;
      lastSettledMillis = millis();
    }
  }

  x_prev = x_hat;
  dx_prev = dx_hat;
  t_prev = t;
  smoothedReading = x_hat;
}
