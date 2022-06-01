import { getBluetoothInstance } from '@nativescript-community/ble';
import { request } from '@nativescript-community/perms';
import { writable } from 'svelte/store';

const SERVICE_UUID = "ade4af7e-f409-473c-ace4-c49d11393be3";
const MASS_CHARACTERISTIC_UUID = "4f00104b-12c2-40d7-b6b9-d3e654222b25";
const TARE_CHARACTERISTIC_UUID = "17769036-e46f-494f-921c-0a545be290ea";
const TIMER_IS_TIMING_CHARACTERISTIC_UUID = "eae9e056-2747-403d-a4b1-f7e9543f3099";
const TIMER_DURATION_CHARACTERISTIC_UUID = "04fc7405-1900-415c-b5b7-7dcfdf55859f";

let scaleUUID: string | undefined = undefined;
export const isConnected = writable(false);
let mass: number = 0.0;
export const displayedMass = writable(0.0);

export const connectToScale = async () => {
  const ble = getBluetoothInstance();
  
  // Request permissions
  for (const perm of ['bluetooth', 'bluetoothConnect', 'bluetoothScan']) {
    await request(perm);
  }

  ble.clear(); // Might be unnessesary
  const bleEnabled = await ble.enable();

  if (!bleEnabled) {
    throw new Error('bluetooth not enabled');
  }

  await ble.startScanning({
    filters:[{serviceUUID: 'ade4af7e-f409-473c-ace4-c49d11393be3'}],
    seconds: 4,
    onDiscovered: (peripheral) => {
        console.log(`found ${peripheral.name}`)
        console.log(peripheral);
        scaleUUID = peripheral.UUID;
        ble.stopScanning();
    }
  });

  if (!scaleUUID) {
    throw new Error('scale not found');
  }

  await ble.stopScanning();

  await ble.connect({
    UUID: scaleUUID,
    onConnected:  (connectedPeripheral) => {
      isConnected.update(val => true);
      console.log("connected");
      },
    onDisconnected: (connectedPeripheral) => {
        isConnected.update(val => false);
      }
  });

  const services = await ble.discoverAll({peripheralUUID: scaleUUID});

  readMassLoop();
}

const readMass = async () => {
  let ble = getBluetoothInstance();
  let readValue = await ble.read({
      peripheralUUID: scaleUUID!,
      serviceUUID: "ade4af7e-f409-473c-ace4-c49d11393be3",
      characteristicUUID: "4f00104b-12c2-40d7-b6b9-d3e654222b25",
      timeout: 1000,
  });
  let data = new Uint8Array(readValue.value);
  mass = +String.fromCharCode(...data);
}

const readMassLoop = async () => {
  if (!isConnected) {
    return;
  }
  await readMass();
  setTimeout(readMassLoop, 100);
}

let interpolateMassLoop = async () => {
  // Should probably use a 1euro filter
  displayedMass.update(dMass => dMass * 0.8 + mass * 0.2);
  setTimeout(interpolateMassLoop, 20);
}

export const tare = async () => {
  const ble = getBluetoothInstance();
  try {
    if (!scaleUUID) {
      throw new Error("No scale uuid");
    }
    await ble.write({
      peripheralUUID: scaleUUID,
      serviceUUID: "ade4af7e-f409-473c-ace4-c49d11393be3",
      characteristicUUID: TARE_CHARACTERISTIC_UUID,
      value: "1",
      encoding: 'utf-8',
    });
  } catch (err) {
    console.log(err);
  }
}

interpolateMassLoop();