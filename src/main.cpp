#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
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
}

void startTimer() {
  
}

enum State {
  IDLE,
  TIMER_WAITING_FOR_START,
  TIMING,
  TIMING_STOPPED,
  CALIBRATION
};

State state = IDLE;
unsigned long startTime = 0;
unsigned long duration = 0;

//float readings[][2] = {{0, 0}, {10, 10}, {20, 20}, {30, 30}, {40, 31}, {50, 32}, {55,35}};
float readings[160][2];
long nextReading = 0;
int readingInterval = 20;

unsigned long lastReadingMillis = millis();

void loop() {
  display.clearDisplay();
  if(!digitalRead(BUTTON_A)) state = TIMER_WAITING_FOR_START;

  if(!digitalRead(BUTTON_B)) scale.tare();

  if(!digitalRead(BUTTON_C)) state=CALIBRATION;

  display.fillRect(0,0,127,32, SH110X_BLACK);
  display.setCursor(0,0);

  scale.updateReading();
  double mass = scale.getReading();
  display.printf("%.1f %.2f\n",mass, mass);
  display.printf("sttl: %.1f, dT: %.f\n", scale.getLastSettledReading(), scale.millisBetweenSettledReadings);
  if (scale.hasSettled) {
    display.println("settled");
  } else {
    display.println("UNSETTLED");
  }
  if (state == TIMER_WAITING_FOR_START) {
   display.println("Waiting for start"); 
  }
  if (state == TIMING) {
    display.print("Duration: ");
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

  if (state == TIMER_WAITING_FOR_START && !scale.hasSettled) {
    startTime = millis();
    state = TIMING;
    nextReading = 0;
  }

  // update reading history for graph
  if (millis() - lastReadingMillis > readingInterval) {
    readings[nextReading][0] = millis();
    readings[nextReading][1] = scale.getReading();
    nextReading++;
    lastReadingMillis = millis();

    // Display is only 160 pixels across so it doesn't really make sense
    // to store more than 160 readings,
    if (nextReading == 160) {
      // Mean all the readings
      for (int i = 0; i<nextReading - 1; i+=2) {
        readings[i/2][0] = (readings[i][0] + readings[i+1][0])/2;
        readings[i/2][1] = (readings[i][1] + readings[i+1][1])/2;
      }

      nextReading=80;
      readingInterval*=2;
    }
  }

  // Do auto tare
  if (scale.hasSettled && abs(scale.getReading() - scale.getLastSettledReading()) > 100 && scale.millisBetweenSettledReadings < 2000) {
    display.println("AUTO TARE");
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

  drawGraph(display, readings, nextReading, 0, 31, 128, 32);
  display.display();
}