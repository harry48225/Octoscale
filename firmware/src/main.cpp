#include "HX711.h"
#include "scale.h"
#include "graph.h"
#include "bluetooth.h"
#include "led.h"
#include "buttons.h"
#include "display.h"
#include "animations.h"
#include "debug.h"

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 48
#define LOADCELL_SCK_PIN 45

Scale scale = Scale(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

enum State {
  IDLE,
  TIMER_PRIMING,
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
  DEBUG_SERIAL.begin(9600);

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
  Display::clear();

  double mass = -1;
  if (state == TIMING_STOPPED) {
    mass = brewMass;
  } else {
    scale.updateReading();
    mass = scale.getReading();
  }

  BLE::update(mass);
  //DEBUG_SERIAL.println(BLE::isDeviceConnected());
  // DEBUG_SERIAL.print(Buttons::a());
  // DEBUG_SERIAL.print(", ");
  // DEBUG_SERIAL.println(Buttons::b());
  //DEBUG_SERIAL.printf("\r a: %d, b: %d                                               ", touchRead(BUTTON_A_PIN), touchRead(BUTTON_B_PIN));
  
  if (state == IDLE) {
    Display::showMass(mass);

    if(Buttons::a()) {
      if (state == IDLE) {
        state = TIMER_PRIMING;
      }
    }

    if(Buttons::b()) {
      unsigned long firstPressed = millis();

      while (Buttons::b()) {
        Buttons::loop();
        Leds::show();
        delay(10);
      }

      unsigned long durationPressed = millis() - firstPressed;

      if (durationPressed > 10000) {
        state = CALIBRATION;
      }
        
      scale.tare();
    }

    if (BLE::isPendingTare()) {
      scale.tare();
      BLE::clearPendingTare();
    }
  }

  // handle timing states
  if (state == TIMER_PRIMING) {
    Graph::reset();
    Graph::stop();
    Display::showTimerPriming();
    scale.tare();
    state = TIMER_WAITING_FOR_START;
  }

  if (state == TIMER_WAITING_FOR_START) {
    Display::showTimerWaitingForStart(mass);
    if (Buttons::b()) {
      scale.tare();
    }

    if (!scale.hasSettled) {
      startTimer();
      BLE::startTiming();
    }

    while (Buttons::b()) {
      Buttons::loop();
      state = IDLE;
    }    
  }

  if (state == TIMING) {
    long seconds = (millis() - startTime)/1000;
    Display::showTimer(seconds, mass);
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

    while (Buttons::b()) {
      state = IDLE;
      delay(10);
      Buttons::loop();
    }
  }

  if (state == TIMING_STOPPED) {
    Display::showBrewStats(brewDuration);

    if (Buttons::b()) {
      state = IDLE;
      delay(10);
    }
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
    
  if (state == CALIBRATION) {
    Display::showCalibrationScreen();

    scale.setScale();
    scale.tareLoadCell();

    scale.unsettle();
    while (!scale.hasSettled) {
      scale.updateReading();
    }

    // wait for b to be pressed
    while(!Buttons::b()) {
      delay(10);
      Leds::clear();
      Buttons::loop();
      Leds::show();
    }

    // wait for b to be released
    while(Buttons::b()) {
      delay(10);
      Leds::clear();
      Buttons::loop();
      Leds::show();
      scale.updateReading();
    }

    Leds::clear();
    Leds::show();
    delay(5000);
    
    scale.unsettle();
    while (!scale.hasSettled) {
      scale.updateReading();
    }
    
    float factor = scale.getReading() / 100.f;
    scale.setScale(factor);

    Display::showCalibrationCompleteScreen(factor);
    delay(2000);

    state = IDLE;
  }

  Graph::update(scale.getReading());
  //Graph::draw(Display::getDisplay(), 0, 41, 128, 22);
  Display::show();
  Leds::show();
}