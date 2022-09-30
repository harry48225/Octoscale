#include "HX711.h"
#include "scale.h"
#include "graph.h"
#include "bluetooth.h"
#include "led.h"
#include "buttons.h"
#include "display.h"
#include "animations.h"
#include "debug.h"
#include "timer.h"
#include "speaker.h"
#include "battery.h"

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
  CALIBRATION,
  SLEEP
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
  Speaker::init();
}

void gatherBrewStats() {
  brewDuration = Timer::getSecondsElapsed();
  brewMass = scale.getLastSettledReading();
  Graph::stop();
}

void sleepLoop() {
  if (Battery::isCharging()) {
    Animations::Charging::update();
  } else {
    Animations::Charged::update();
  }
  delay(10);
}

void awakeLoop() {
  Speaker::clear();
  Leds::clear();
  Display::clear();
  Buttons::loop();
  double mass = -1;
  if (state == TIMING_STOPPED) {
    mass = brewMass;
  } else {
    scale.updateReading();
    mass = scale.getReading();
  }

  BLE::update(mass);

  if (state == IDLE) {
    Display::showMass(mass);

    if(Buttons::a()) {
      if (state == IDLE) state = TIMER_PRIMING;
    }

    if(Buttons::b()) {
      if (Buttons::getDurationPressed() > 10000) {
        state = CALIBRATION;
      }
        
      scale.tare();
    }

    if (BLE::isPendingTare()) {
      scale.tare();
      BLE::clearPendingTare();
    }
  } 
  else if (state == TIMER_PRIMING) {
    Graph::reset();
    Graph::stop();
    Display::showTimerPriming();
    scale.tare();
    state = TIMER_WAITING_FOR_START;
  }
  else if (state == TIMER_WAITING_FOR_START) {
    Display::showTimerWaitingForStart(mass);
    if (Buttons::b()) state = IDLE;

    if (!scale.hasSettled || Buttons::a()) {
      auto start = millis();
      while (!Buttons::a() && millis() - start < 200) {
        Buttons::loop();
        delay(50);
      }

      if (Buttons::a()) {
        scale.tare();
        Timer::startWithCountDown(5);
        state = TIMING; 
      } else {
        Timer::start();
        state = TIMING;
      }
    }
  }
  else if (state == TIMING) {
    long seconds = Timer::getSecondsElapsed();
    Display::showTimer(seconds, mass);
    BLE::updateTimerDuration(seconds);

    if (scale.getLastSettledReading() - scale.getReading() > STOP_TIMER_REMOVAL_MASS) {
      if (!brewStatsGathered) gatherBrewStats();
      if (scale.hasSettled) {
        Timer::stop();
        state = TIMING_STOPPED;
        BLE::stopTiming();
      }
    } else {
      brewStatsGathered = false;
      Graph::resume();
    }

    if (Buttons::a()) {
      gatherBrewStats();
      Timer::stop();
      BLE::stopTiming();
      state = TIMING_STOPPED;
    }

    if (Buttons::b()) state = IDLE;
  }
  else if (state == TIMING_STOPPED) {
    Display::showBrewStats(brewMass, brewDuration);

    if (Buttons::b()) state = IDLE;
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
      Buttons::loop();
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
  Speaker::sound();
}

void loop() {
  if (Battery::isPowerSwitchOn()) {
    if (state == SLEEP) state = IDLE;
    while (Battery::getVoltage() <= CUTOFF_VOLTAGE) {
      Display::showBatteryDepletedScreen();
      delay(100);
    } 
  } else {
    if (state != SLEEP) {
      state = SLEEP;
      Display::clear();
      Display::show(false);
      Leds::clear();
      Leds::show();
      Speaker::clear();
      Speaker::sound();
    }
  }

  if (state == SLEEP) {
    sleepLoop();
  } else {
    awakeLoop();
  }
}