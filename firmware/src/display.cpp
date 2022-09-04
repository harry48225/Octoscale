#include "display.h"
#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include "debug.h"

namespace Display {
  Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, 14);

  void init() {
    Wire.begin(12, 13);
    delay(250); // wait for the OLED to power up
    display.begin(0x3C, true);

    // Clear the buffer.
    display.clearDisplay();
    display.display();
    display.setRotation(0);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0,0);
  }

  void showMass(double rawMass, int x, int y) {
    double mass = rawMass;

    if (mass > -0.1 && mass < 0) {
      mass = 0;
    }

    char massChars[16];
    sprintf(massChars, "%-3.1f", mass);

    String massString(massChars);

    while (massString.length() < 5) {
      massString = " " + massString;
    }

    DEBUG_SERIAL.println(massString);
    
    display.setFont(&FreeMono18pt7b);

    display.setCursor(x, y);
    display.printf(massString.c_str());
    display.setFont();
    display.setCursor(0,16+10);
    //display.printf("sttl: %.1f, dT: %.f\n", scale.getLastSettledReading(), scale.millisBetweenSettledReadings);
  }

  void showTimer(long seconds, double mass) {
    clear();
    display.print("timing: ");
    display.printf("%02d:%02d", seconds / 60, seconds % 60);
    showMass(mass);
    show();
  }

  void showTimerPriming() {
    clear();    
    display.println("timer priming");
    show();    
  }

  void showTimerWaitingForStart(double mass) {
    clear();
    showMass(mass);
    show();
  }

  void showBrewCompleteAnimation() {
    for (int i = 0; i < 5; i++) {
      display.clearDisplay();
      display.setCursor(20,30);
      display.println("<<BREW COMPLETE>>");
      display.display();
      delay(500);
      display.clearDisplay();
      display.display();
      delay(500);
    }
  }

  void showBrewStats(unsigned long duration) {
    display.printf("in %02dm:%02ds,", duration / 60, duration % 60);
  }

  void showAutoTare() {
    display.clearDisplay();
    display.setCursor(0,12);
    display.setFont(&FreeMono12pt7b);
    display.println("AUTO");
    display.println("TARE");
    display.display();
  }

  void showCalibrationScreen() {
    clear();
    display.println("Calibration mode");
    display.println("Place a 100g mass");
    display.println("Then press B");
    show();
  }

  void showCalibrationCompleteScreen(float factor) {
    clear();
    display.println("Calibrated!");
    display.println("Factor: ");
    display.println(factor);
    show();
  }

  void clear() {
    display.clearDisplay();
    display.setCursor(0,0);
  }

  void show() {
    display.display();
  }

  Adafruit_SH1106G getDisplay() {
    return display;
  }
}