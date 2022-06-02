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

export const isTiming = writable(false);

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
    filters:[{serviceUUID: SERVICE_UUID}],
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

  // For some strange reason need to discover the services before the connection works...
  const services = await ble.discoverAll({peripheralUUID: scaleUUID});

  ble.startNotifying({
    peripheralUUID: scaleUUID,
    serviceUUID: SERVICE_UUID,
    characteristicUUID: TIMER_IS_TIMING_CHARACTERISTIC_UUID,
    onNotify: (result) => {
      const timing = decodeResultValue(result.value);
      if (timing == "1") {
        console.log("Started timing");
        isTiming.update(() => true);
      } else {
        isTiming.update(() => false);
        console.log("stopped timing");
      }
    } 
  });

  ble.startNotifying({
    peripheralUUID: scaleUUID,
    serviceUUID: SERVICE_UUID,
    characteristicUUID: MASS_CHARACTERISTIC_UUID,
    onNotify: (result) => {
      const massString = decodeResultValue(result.value);
      mass = +massString;
    }
  })
}

const decodeResultValue = (val: ArrayBuffer) => {
  const data = new Uint8Array(val);
  return String.fromCharCode(...data);
}

let interpolateMassLoop = async () => {
  // Should probably use a 1euro filter
  displayedMass.update(dMass => dMass * 0.2 + mass * 0.8);
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
      serviceUUID: SERVICE_UUID,
      characteristicUUID: TARE_CHARACTERISTIC_UUID,
      value: "1",
      encoding: 'utf-8',
    });
  } catch (err) {
    console.log(err);
  }
}

interpolateMassLoop();