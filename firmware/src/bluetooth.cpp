#include "bluetooth.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h> 

#define SERVICE_UUID "ade4af7e-f409-473c-ace4-c49d11393be3"
#define MASS_CHARACTERISTIC_UUID "4f00104b-12c2-40d7-b6b9-d3e654222b25"
#define TARE_CHARACTERISTIC_UUID "17769036-e46f-494f-921c-0a545be290ea"
#define TIMER_IS_TIMING_CHARACTERISTIC_UUID "eae9e056-2747-403d-a4b1-f7e9543f3099"
#define TIMER_DURATION_CHARACTERISTIC_UUID "04fc7405-1900-415c-b5b7-7dcfdf55859f"
#define A_BUTTON_CHARACTERISTIC_UUID "bee6b99c-8ba8-40b2-bda1-fbea0e23696d"
#define B_BUTTON_CHARACTERISTIC_UUID "e96bdf52-9eae-44dc-bcca-55ef50a8a924"


// Should we be able to start the timer from the app
// or just be able to prime it?

namespace BLE {
  bool deviceConnected = false;
  BLECharacteristic massCharacteristic(
    MASS_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor massDescriptor(BLEUUID((uint16_t)0x2902));

  BLECharacteristic tareCharacteristic(
    TARE_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  BLECharacteristic aButtonCharacteristic(
    A_BUTTON_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  BLECharacteristic bButtonCharacteristic(
    B_BUTTON_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  BLECharacteristic isTimingCharacteristic(
    TIMER_IS_TIMING_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor isTimingDescriptor(BLEUUID((uint16_t)0x2902)); // Needed to advertise the notify property

  BLECharacteristic timerDurationCharacteristic(
    TIMER_DURATION_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor timerDurationDescriptor(BLEUUID((uint16_t)0x2902));

  class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
  };

  bool isDeviceConnected() {
    return deviceConnected;
  }

  void init() {
    //esp_bt_controller_init();
    //esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    cfg.bluetooth_mode = ESP_BT_MODE_BLE;
    cfg.sleep_mode = ESP_BT_SLEEP_MODE_1;
    cfg.sleep_clock = ESP_BT_SLEEP_CLOCK_MAIN_XTAL;
    cfg.txpwr_dft = ESP_PWR_LVL_N27;
    esp_bt_controller_init(&cfg);

    BLEDevice::init("octoscale");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    massCharacteristic.setValue("0");
    massCharacteristic.addDescriptor(&massDescriptor);
    pService->addCharacteristic(&massCharacteristic);

    tareCharacteristic.setValue("0");
    pService->addCharacteristic(&tareCharacteristic);

    aButtonCharacteristic.setValue("0");
    pService->addCharacteristic(&aButtonCharacteristic);

    bButtonCharacteristic.setValue("0");
    pService->addCharacteristic(&bButtonCharacteristic);

    isTimingCharacteristic.setValue("0");
    isTimingCharacteristic.addDescriptor(&isTimingDescriptor);
    pService->addCharacteristic(&isTimingCharacteristic);

    timerDurationCharacteristic.setValue("0");
    timerDurationCharacteristic.addDescriptor(&timerDurationDescriptor);
    pService->addCharacteristic(&timerDurationCharacteristic);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
  }

  void update(float mass) {
    char massString[16];
    sprintf(massString, "%.1f", mass);
    massCharacteristic.setValue(massString);
    massCharacteristic.notify();
  }

  bool isPendingTare() {
    String tare = tareCharacteristic.getValue().c_str();
    return tare.equals("1");
  }

  void clearPendingTare() {
    tareCharacteristic.setValue("0");
  }

  bool isPendingAButton() {
    String a = aButtonCharacteristic.getValue().c_str();
    return a.equals("1"); 
  }

  void clearPendingAButton() {
    aButtonCharacteristic.setValue("0");
  }

  bool isPendingBButton() {
    String b = bButtonCharacteristic.getValue().c_str();
    return b.equals("1"); 
  }

  void clearPendingBButton() {
    bButtonCharacteristic.setValue("0");
  }

  void startTiming() {
    isTimingCharacteristic.setValue("1");
    isTimingCharacteristic.notify();
  }

  void stopTiming() {
    isTimingCharacteristic.setValue("0");
    isTimingCharacteristic.notify();
  }

  void updateTimerDuration(long seconds) {
    char secondsString[16];
    sprintf(secondsString, "%d", seconds);
    timerDurationCharacteristic.setValue(secondsString);
    timerDurationCharacteristic.notify();
  }
}