#include "bluetooth.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "ade4af7e-f409-473c-ace4-c49d11393be3"
#define MASS_CHARACTERISTIC_UUID "4f00104b-12c2-40d7-b6b9-d3e654222b25"
#define TARE_CHARACTERISTIC_UUID "17769036-e46f-494f-921c-0a545be290ea"
#define TIMER_CHARACTERISTIC_UUID "04fc7405-1900-415c-b5b7-7dcfdf55859f"
#define TIMER_IS_TIMING_CHARACTERISTIC_UUID "eae9e056-2747-403d-a4b1-f7e9543f3099"


namespace BLE {
  bool deviceConnected = false;
  BLECharacteristic massCharacteristic(
    MASS_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
  );

  BLECharacteristic tareCharacteristic(
    TARE_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      BLEDevice::startAdvertising();
    }
  };

  bool isDeviceConnected() {
    return deviceConnected;
  }

  void init() {
    BLEDevice::init("octoscale");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    massCharacteristic.setValue("0");
    pService->addCharacteristic(&massCharacteristic);

    tareCharacteristic.setValue("0");
    pService->addCharacteristic(&tareCharacteristic);

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
  }

  bool isPendingTare() {
    String tare = tareCharacteristic.getValue().c_str();
    return tare.equals("1");
  }

  void clearPendingTare() {
    tareCharacteristic.setValue("0");
  }
}