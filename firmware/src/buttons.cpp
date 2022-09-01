#include "buttons.h"
#include <Arduino.h>

namespace Buttons {
  int aVal = 0;
  int bVal = 0;

  bool aPressed = false;
  bool bPressed = false; 

  void init() {
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);
    aVal = touchRead(BUTTON_A_PIN);
    bVal = touchRead(BUTTON_B_PIN);
  }

  void loop() {
    int newA = touchRead(BUTTON_A_PIN);
    int newB = touchRead(BUTTON_B_PIN);

    if (newA > aVal + THRESHOLD) {
      aPressed = true; 
    } else if (newA < aVal - THRESHOLD) {
      aPressed = false;
    }

    if (newB > bVal + THRESHOLD) {
      bPressed = true; 
    } else if (newB < bVal - THRESHOLD) {
      bPressed = false;
    }

    aVal = newA;
    bVal = newB;
  }

  bool a() {
    return aPressed;
  }

  bool b() {
    return bPressed;
  }
}
