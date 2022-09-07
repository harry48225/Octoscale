#include "buttons.h"
#include <Arduino.h>
#include "led.h"
#include "debug.h"

namespace Buttons {
  int aVal = 0;
  int bVal = 0;

  int lowestA = 40000;
  int lowestB = 40000;

  bool aPressed = true;
  bool bPressed = true; 

  void init() {
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);
    aVal = touchRead(BUTTON_A_PIN);
    bVal = touchRead(BUTTON_B_PIN);
    lowestA = aVal;
    lowestB = bVal;    
  }

  void loop() {
    int newA = 0.2 * touchRead(BUTTON_A_PIN) + 0.8 * aVal;
    int newB = 0.2 * touchRead(BUTTON_B_PIN) + 0.8 * bVal;

    DEBUG_SERIAL.print(newA);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(newB);

    aPressed = newA > lowestA + THRESHOLD; 
    bPressed = newB > lowestB + THRESHOLD;

    if (aPressed) Leds::aTapped();
    if (bPressed) Leds::bTapped();

    DEBUG_SERIAL.print(aPressed);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(bPressed);

    aVal = newA;
    bVal = newB;

    lowestA = min(lowestA, aVal);
    lowestB = min(lowestB, bVal);
  }

  bool a() {
    return aPressed;
  }

  bool b() {
    return bPressed;
  }

  void waitForRelease() {
    while (a() || b()) {
      loop();
      delay(10);
    }
  }
}