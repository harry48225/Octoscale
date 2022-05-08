#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  1
#define BUTTON_B  38
#define BUTTON_C  33

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 17
#define LOADCELL_SCK_PIN 18

HX711 scale;

void setup() {
  Serial.begin(115200);

  Serial.println("128x64 OLED FeatherWing test");
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(scale.get_units(40));
  display.display();
  if(!digitalRead(BUTTON_C)) {
    display.clearDisplay();
    display.println("Calibration mode");
    display.println("Place a 100g mass");
    display.println("Then press A");
    display.display();
    scale.set_scale();
    scale.tare();

    while(digitalRead(BUTTON_A)) delay(10);
    
    float factor = scale.get_units(100) / 100.f;

    scale.set_scale(factor);

    display.clearDisplay();
    display.println("Calibrated!");
    delay(500);
  }
}