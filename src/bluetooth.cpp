#include "bluetooth.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "ade4af7e-f409-473c-ace4-c49d11393be3"
#define CHARACTERISTIC_UUID "4f00104b-12c2-40d7-b6b9-d3e654222b25"

namespace BLE {
  BLECharacteristic massCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
  );

  void init() {
    BLEDevice::init("octoscale");
    BLEServer *pServer = BLEDevice::createServer();

    BLEService *pService = pServer->createService(SERVICE_UUID);

    massCharacteristic.setValue("0");
    pService->addCharacteristic(&massCharacteristic);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
  }

  void update(float mass) {
    char massString[64];
    sprintf(massString, "%.f", mass);
    massCharacteristic.setValue(massString);
  }
}