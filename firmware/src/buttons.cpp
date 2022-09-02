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
    int newA = 0.3 * touchRead(BUTTON_A_PIN) + 0.7 * aVal;
    int newB = 0.3 * touchRead(BUTTON_B_PIN) + 0.7 * bVal;

    Serial.print(newA);
    Serial.print(", ");
    Serial.println(newB);

    aPressed = newA > A_THRESHOLD; 
    bPressed = newB > B_THRESHOLD;

    Serial.print(aPressed);
    Serial.print(", ");
    Serial.println(bPressed);

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