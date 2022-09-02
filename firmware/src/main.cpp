#include "HX711.h"
#include "scale.h"
#include "graph.h"
#include "bluetooth.h"
#include "led.h"
#include "buttons.h"
#include "display.h"

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
bool autotareEnabled = false;
unsigned long startTime = 0;
bool brewStatsGathered = false;
unsigned long brewDuration = 0;
float brewMass = 0;


void setup() {
  Serial.begin(9600);

  Display::init();
  BLE::init();
  Leds::init();
  Buttons::init();
}

void startTimer() {
  startTime = millis();
  Graph::reset();
  autotareEnabled = false;
  state = TIMING;
}

void stopTimer() {
  state = TIMING_STOPPED;
  Display::showBrewCompleteAnimation();
  delay(500);
}

void loop() {
  Leds::clear();
  Buttons::loop();
  //Serial.println(BLE::isDeviceConnected());
  // Serial.print(Buttons::a());
  // Serial.print(", ");
  // Serial.println(Buttons::b());
  //Serial.printf("\r a: %d, b: %d                                               ", touchRead(BUTTON_A_PIN), touchRead(BUTTON_B_PIN));
  if(Buttons::a()) state = TIMER_WAITING_FOR_START;

  if(Buttons::b()) {

    while (Buttons::b())
    {
      Buttons::loop();
      Leds::show();
      delay(10);
    }
    Leds::clear();
    Leds::show();
    while (!scale.hasSettled)
    {
      scale.updateReading();
      delay(10);
    }
    
    scale.tare();
  }

  if (BLE::isPendingTare()) {
    scale.tare();
    BLE::clearPendingTare();
  }

  Display::clear();

  double mass = -1;
  if (state == TIMING_STOPPED) {
    mass = brewMass;
  } else {
    scale.updateReading();
    mass = scale.getReading();
  }
  
  Display::showMass(mass);
  BLE::update(mass);
  
  // handle timing states
  if (state == TIMER_WAITING_FOR_START) {
    Graph::reset();
    Graph::stop();
    Display::showTimerPrimed();
  if (!scale.hasSettled) {
      startTimer();
      BLE::startTiming();
   }
  }

  if (state == TIMING) {
    long seconds = (millis() - startTime)/1000;
    Display::showTimer(seconds);
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
    Display::showBrewStats(brewDuration);
  }

  // Do auto tare
  if (autotareEnabled && scale.hasSettled && abs(scale.getReading() - scale.getLastSettledReading()) > 100 && scale.millisBetweenSettledReadings < 2000) {
    Display::showAutoTare();
    // Take some more readings
    for (int i = 0; i < 40; i++) {
      scale.updateReading();
      delay(20);
    }
    scale.tare();
    delay(200);
  }
    
  // if (state == CALIBRATION) {
  //   display.clearDisplay();
  //   display.println("Calibration mode");
  //   display.println("Place a 100g mass");
  //   display.println("Then press A");
  //   display.display();
  //   scale.setScale();
  //   scale.tareLoadCell();
  //   scale.tare();

  //   while(digitalRead(BUTTON_A)) delay(10);
    
  //   float factor = scale.getUnits(255) / 100.f;

  //   scale.setScale(factor);

  //   display.clearDisplay();
  //   display.setCursor(0,0);
  //   display.println("Calibrated!");
  //   display.print("Factor: ");
  //   display.print(factor);
  //   display.display();
  //   delay(2000);

  //   state = IDLE;
  // }

  Graph::update(scale.getReading());
  //Graph::draw(Display::getDisplay(), 0, 41, 128, 22);
  Display::show();
  Leds::show();
}