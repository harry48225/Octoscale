#include "bluetooth.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "ade4af7e-f409-473c-ace4-c49d11393be3"
#define CHARACTERISTIC_UUID "4f00104b-12c2-40d7-b6b9-d3e654222b25"

namespace BLE {
  bool deviceConnected = false;
  BLECharacteristic massCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
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
}