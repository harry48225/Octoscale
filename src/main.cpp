#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include "scale.h"
#include "graph.h"

#define BUTTON_A  1
#define BUTTON_B  38
#define BUTTON_C  33

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 17
#define LOADCELL_SCK_PIN 18

Scale scale = Scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

enum State {
  IDLE,
  TIMER_WAITING_FOR_START,
  TIMING,
  TIMING_STOPPED,
  CALIBRATION
};

State state = IDLE;
bool autotareEnabled = true;
unsigned long startTime = 0;
unsigned long duration = 0;


void setup() {
  Serial.begin(115200);
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);

  // setup buttons
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

void startTimer() {
  
}

void loop() {
  if(!digitalRead(BUTTON_A)) state = TIMER_WAITING_FOR_START;

  if(!digitalRead(BUTTON_B)) scale.tare();

  if(!digitalRead(BUTTON_C)) state=CALIBRATION;

  display.clearDisplay();
  display.setCursor(0,0);

  // Display mass
  scale.updateReading();
  double mass = scale.getReading();
  display.setCursor(0,16);
  display.setFont(&FreeMono18pt7b);
  display.printf("%.1f\n",mass);
  display.setFont();
  display.setCursor(0,16+10);
  //display.printf("sttl: %.1f, dT: %.f\n", scale.getLastSettledReading(), scale.millisBetweenSettledReadings);
  
  // handle timing states
  if (state == TIMER_WAITING_FOR_START) {
   display.println("timer primed");

   if (!scale.hasSettled) {
      startTime = millis();
      Graph::reset();
      autotareEnabled = false;
      state = TIMING;
   }
  }

  if (state == TIMING) {
    display.print("timing: ");
    long seconds = (millis() - startTime)/1000;
    display.printf("%02d:%02d", seconds / 60, seconds % 60);

    // Something greater than 50g must have been taken off the scale
    if (scale.getLastSettledReading() - scale.getReading() > 50) {
      state = TIMING_STOPPED;
      duration = (millis() - startTime)/1000;
    }
  }

  if (state == TIMING_STOPPED) {
    display.print("Brew Time: ");
    display.printf("%02d:%02d", duration / 60, duration % 60);
  }

  // Do auto tare
  if (autotareEnabled && scale.hasSettled && abs(scale.getReading() - scale.getLastSettledReading()) > 100 && scale.millisBetweenSettledReadings < 2000) {
    display.clearDisplay();
    display.setCursor(0,12);
    display.setFont(&FreeMono12pt7b);
    display.println("AUTO");
    display.println("TARE");
    display.display();
    // Take some more readings
    for (int i = 0; i < 40; i++) {
      scale.updateReading();
      delay(20);
    }
    scale.tare();
    delay(200);
  }
    
  if (state == CALIBRATION) {
    display.clearDisplay();
    display.println("Calibration mode");
    display.println("Place a 100g mass");
    display.println("Then press A");
    display.display();
    scale.setScale();
    scale.tare();

    while(digitalRead(BUTTON_A)) delay(10);
    
    float factor = scale.getUnits(255) / 100.f;

    scale.setScale(factor);

    display.clearDisplay();
    display.println("Calibrated!");
    display.print("Factor: ");
    display.print(factor);
    delay(2000);
  }

  Graph::update(scale.getReading());
  Graph::draw(display, 0, 41, 128, 22);
  display.display();
}