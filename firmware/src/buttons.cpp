#include "buttons.h"
#include <Arduino.h>
#include "led.h"
#include "debug.h"
#include "speaker.h"
#include "bluetooth.h"

namespace Buttons {
  int aVal = 0;
  int bVal = 0;

  int lowestA = 40000;
  int lowestB = 40000;

  bool aPressed = true;
  bool bPressed = true;

  bool aPressedLatching = true;
  bool bPressedLatching = true;

  unsigned long durationPressed = 0;

  void init() {
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);

    delay(100);

    aVal = touchRead(BUTTON_A_PIN);
    bVal = touchRead(BUTTON_B_PIN);
    lowestA = min(lowestA, aVal);
    lowestB = min(lowestB, bVal);
  }

  void update() {
    int newA = 0.2 * touchRead(BUTTON_A_PIN) + 0.8 * aVal;
    int newB = 0.2 * touchRead(BUTTON_B_PIN) + 0.8 * bVal;

    DEBUG_SERIAL.print(newA);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(newB);

    aPressed = newA > lowestA + THRESHOLD; 
    bPressed = newB > lowestB + THRESHOLD;

    aVal = newA;
    bVal = newB;

    lowestA = min(lowestA, aVal);
    lowestB = min(lowestB, bVal);
  }

  void loop() {
    aPressedLatching = false;
    bPressedLatching = false;
    
    update();

    if (aPressed) {
      Leds::aTapped();
      Speaker::buttonBeep();
      aPressedLatching = true;
    }
    if (bPressed) {
      Leds::bTapped();
      Speaker::buttonBeep();
      bPressedLatching = true;
    }

    DEBUG_SERIAL.print(aPressed);
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(bPressed);


    if (aPressed || bPressed) {
      Leds::show();
      Speaker::sound();

      unsigned long start = millis();
      waitForRelease();
      durationPressed = millis() - start;

      Leds::clear();
      Speaker::clear();
      Leds::show();
      Speaker::sound();
    }

    if (BLE::isPendingAButton()) {
      aPressedLatching = true;
      BLE::clearPendingAButton();
    }

    if (BLE::isPendingBButton()) {
      bPressedLatching = true;
      BLE::clearPendingBButton();
    }
  }

  bool a() {
    return aPressedLatching;
  }

  bool b() {
    return bPressedLatching;
  }

  unsigned long getDurationPressed() {
    return durationPressed;
  }

  void waitForRelease() {
    while (aPressed || bPressed) {
      update();
      delay(10);
    }
  }
}