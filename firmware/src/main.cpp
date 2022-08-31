#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include "scale.h"
#include "graph.h"
#include "bluetooth.h"

#define BUTTON_A  1
#define BUTTON_B  38
#define BUTTON_C  33

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 48
#define LOADCELL_SCK_PIN 45

Scale scale = Scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

enum State {
  IDLE,
  TIMER_WAITING_FOR_START,
  TIMING,
  TIMING_STOPPED,
  CALIBRATION
};

#define STOP_TIMER_REMOVAL_MASS 50

State state = IDLE;
bool autotareEnabled = true;
unsigned long startTime = 0;
bool brewStatsGathered = false;
unsigned long brewDuration = 0;
float brewMass = 0;


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

  BLE::init();
}

void displayMass(double mass) {
  display.setCursor(0,16);
  display.setFont(&FreeMono18pt7b);
  display.printf("%-3.1f\n",mass);
  display.setFont();
  display.setCursor(0,16+10);
  //display.printf("sttl: %.1f, dT: %.f\n", scale.getLastSettledReading(), scale.millisBetweenSettledReadings);
}

void startTimer() {
  startTime = millis();
  Graph::reset();
  autotareEnabled = false;
  state = TIMING;
}

void stopTimer() {
  state = TIMING_STOPPED;
  display.clearDisplay();
  display.setCursor(8,30);
  display.println("<<brew complete>>");
  display.display();
  delay(500);
}

void displayTimer(long seconds) {
  display.print("timing: ");
  display.printf("%02d:%02d", seconds / 60, seconds % 60);
}

void displayBrewCompleteAnimation() {
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

void displayBrewStats() {
  display.printf("in %02dm:%02ds,", brewDuration / 60, brewDuration % 60);
}

void displayAutoTare() {
  display.clearDisplay();
  display.setCursor(0,12);
  display.setFont(&FreeMono12pt7b);
  display.println("AUTO");
  display.println("TARE");
  display.display();
}

void loop() {
  Serial.println(BLE::isDeviceConnected());
  if(!digitalRead(BUTTON_A)) state = TIMER_WAITING_FOR_START;

  if(!digitalRead(BUTTON_B)) scale.tare();

  if(!digitalRead(BUTTON_C)) state=CALIBRATION;

  if (BLE::isPendingTare()) {
    scale.tare();
    BLE::clearPendingTare();
  }

  display.clearDisplay();
  display.setCursor(0,0);

  double mass = -1;
  if (state == TIMING_STOPPED) {
    mass = brewMass;
  } else {
    scale.updateReading();
    mass = scale.getReading();
  }
  displayMass(mass);
  BLE::update(mass);
  
  // handle timing states
  if (state == TIMER_WAITING_FOR_START) {
    Graph::reset();
    Graph::stop();
   display.println("timer primed");
   if (!scale.hasSettled) {
      startTimer();
      BLE::startTiming();
   }
  }

  if (state == TIMING) {
    long seconds = (millis() - startTime)/1000;
    displayTimer(seconds);
    // This should probably be in millis ... 
    BLE::updateTimerDuration(seconds);
    // Something greater than 50g must have been taken off the scale
    if (scale.getLastSettledReading() - scale.getReading() > STOP_TIMER_REMOVAL_MASS) {
      if (!brewStatsGathered) {
        brewDuration = (millis() - startTime)/1000;
        brewMass = scale.getLastSettledReading();
        Graph::stop();
      }
      if (scale.hasSettled) {
        stopTimer();
        BLE::stopTiming();
      }
    } else {
      brewStatsGathered = false;
      Graph::resume();
    }
  }

  if (state == TIMING_STOPPED) {
    displayBrewStats();
  }

  // Do auto tare
  if (autotareEnabled && scale.hasSettled && abs(scale.getReading() - scale.getLastSettledReading()) > 100 && scale.millisBetweenSettledReadings < 2000) {
    displayAutoTare();
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
    scale.tareLoadCell();
    scale.tare();

    while(digitalRead(BUTTON_A)) delay(10);
    
    float factor = scale.getUnits(255) / 100.f;

    scale.setScale(factor);

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Calibrated!");
    display.print("Factor: ");
    display.print(factor);
    display.display();
    delay(2000);

    state = IDLE;
  }

  Graph::update(scale.getReading());
  Graph::draw(display, 0, 41, 128, 22);
  display.display();
}