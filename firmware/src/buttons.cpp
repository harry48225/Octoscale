#include "buttons.h"
#include <Arduino.h>
#include "led.h"
#include "debug.h"

namespace Buttons {
  int aVal = 0;
  int bVal = 0;

  bool aPressed = true;
  bool bPressed = true; 

  void init() {
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);
    aVal = touchRead(BUTTON_A_PIN);
    bVal = touchRead(BUTTON_B_PIN);

    while (aPressed || bPressed)
    {
      loop();
      delay(10);
    }
    
  }

  void loop() {
    int newA = 0.3 * touchRead(BUTTON_A_PIN) + 0.7 * aVal;
    int newB = 0.3 * touchRead(BUTTON_B_PIN) + 0.7 * bVal;

    DEBUG_SERIAL.print(newA);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(newB);

    aPressed = newA > A_THRESHOLD; 
    bPressed = newB > B_THRESHOLD;

    if (aPressed) Leds::aTapped();
    if (bPressed) Leds::bTapped();

    DEBUG_SERIAL.print(aPressed);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(bPressed);

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