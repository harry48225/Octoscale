import { getBluetoothInstance } from '@nativescript-community/ble';
import { request } from '@nativescript-community/perms';
import { writable } from 'svelte/store';

const SERVICE_UUID = "ade4af7e-f409-473c-ace4-c49d11393be3";
const MASS_CHARACTERISTIC_UUID = "4f00104b-12c2-40d7-b6b9-d3e654222b25";
const A_BUTTON_CHARACTERISTIC_UUID = "bee6b99c-8ba8-40b2-bda1-fbea0e23696d";
const B_BUTTON_CHARACTERISTIC_UUID = "e96bdf52-9eae-44dc-bcca-55ef50a8a924";
const TIMER_IS_TIMING_CHARACTERISTIC_UUID = "eae9e056-2747-403d-a4b1-f7e9543f3099";
const TIMER_DURATION_CHARACTERISTIC_UUID = "04fc7405-1900-415c-b5b7-7dcfdf55859f";

let scaleUUID: string | undefined = undefined;
export const isConnected = writable(false);
export const mass = writable(0.0);
let massValue = 0;

mass.subscribe((val) => massValue = val);

export const displayedMass = writable(0.0);

export const isTiming = writable(false);
export const timerDurationSeconds = writable(0);

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
    seconds: 60,
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
      mass.set(+massString);
    }
  })

  // Maybe should read these on first connect.
  ble.startNotifying({
    peripheralUUID: scaleUUID,
    serviceUUID: SERVICE_UUID,
    characteristicUUID: TIMER_DURATION_CHARACTERISTIC_UUID,
    onNotify: (result) => {
      const durationString = decodeResultValue(result.value);
      timerDurationSeconds.update(secs => +durationString);
    }
  })
}

const decodeResultValue = (val: ArrayBuffer) => {
  const data = new Uint8Array(val);
  return String.fromCharCode(...data);
}

let interpolateMassLoop = async () => {
  // Should probably use a 1euro filter
  displayedMass.update(dMass => {
    let newMass = dMass * 0.1 + massValue * 0.9;
    if (newMass > -0.05 && newMass < 0) {
      newMass = 0;
    }
    return newMass;
  });
  setTimeout(interpolateMassLoop, 20);
}

const writeCharacteristic = async (uuid: string, value: string) => {
  const ble = getBluetoothInstance();
  try {
    if (!scaleUUID) {
      throw new Error("No scale uuid");
    }
    await ble.write({
      peripheralUUID: scaleUUID,
      serviceUUID: SERVICE_UUID,
      characteristicUUID: uuid,
      value: value,
      encoding: 'utf-8',
    });
  } catch (err) {
    console.log(err);
  }
}

const writeButton = async (uuid: string) => writeCharacteristic(uuid, "1");

export const aButton = async () => writeButton(A_BUTTON_CHARACTERISTIC_UUID);
export const bButton = async () => writeButton(B_BUTTON_CHARACTERISTIC_UUID);

interpolateMassLoop();