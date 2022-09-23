#include "display.h"
#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include "debug.h"
#include "bluetooth.h"
#include "battery.h"

// 'battery-full', 8x8px
const unsigned char epd_bitmap_battery_full [] PROGMEM = {
	0x18, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c
};
// 'battery-empty', 8x8px
const unsigned char epd_bitmap_battery_empty [] PROGMEM = {
	0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c
};
// 'battery-1-3', 8x8px
const unsigned char epd_bitmap_battery_1_3 [] PROGMEM = {
	0x18, 0x24, 0x42, 0x42, 0x42, 0x7e, 0x7e, 0x3c
};
// 'battery-2-3', 8x8px
const unsigned char epd_bitmap_battery_2_3 [] PROGMEM = {
	0x18, 0x24, 0x42, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c
};
// 'charging', 8x8px
const unsigned char epd_bitmap_charging [] PROGMEM = {
	0x00, 0x04, 0x0f, 0x3c, 0x4f, 0x84, 0x40, 0x30
};
// 'bluetooth-disconnected', 8x8px
const unsigned char epd_bitmap_bluetooth_disconnected [] PROGMEM = {
	0x3c, 0x16, 0x82, 0x46, 0x62, 0x51, 0x78, 0x3c
};
// 'bluetooth', 8x8px
const unsigned char epd_bitmap_bluetooth [] PROGMEM = {
	0x3c, 0x76, 0x52, 0x66, 0x66, 0x52, 0x76, 0x3c
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 224)
const int epd_bitmap_allArray_LEN = 7;
const unsigned char* epd_bitmap_allArray[7] = {
	epd_bitmap_battery_1_3,
	epd_bitmap_battery_2_3,
	epd_bitmap_battery_empty,
	epd_bitmap_battery_full,
	epd_bitmap_bluetooth,
	epd_bitmap_bluetooth_disconnected,
	epd_bitmap_charging
};


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
    display.println("timer primed");
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

  void showBrewStats(double mass, unsigned long duration) {
    display.printf("%-3.1fg in %02dm:%02ds,", mass, duration / 60, duration % 60);
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

  void showCountdown(int seconds) {
    clear();
    display.println(seconds);
    show();
  }

  void showStatusBar() {
    display.setCursor(0, 0);
    display.fillRect(0, 0, 128, 8, SH110X_BLACK);
    display.print(Battery::getVoltage());
    if (BLE::isDeviceConnected()) {
      display.drawBitmap(120, 0, epd_bitmap_bluetooth, 8, 8, SH110X_WHITE);
    } else {
      display.drawBitmap(120, 0, epd_bitmap_bluetooth_disconnected, 8, 8, SH110X_WHITE);
    }
  }

  void clear() {
    display.clearDisplay();
    display.setCursor(0,0);
  }

  void show() {
    showStatusBar();
    display.display();
  }

  Adafruit_SH1106G getDisplay() {
    return display;
  }
}