import { getBluetoothInstance } from '@nativescript-community/ble';
import { request } from '@nativescript-community/perms';
import { writable } from 'svelte/store';

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
  displayedMass.update(dMass => dMass * 0.6 + mass * 0.4);
  setTimeout(interpolateMassLoop, 10);
}

interpolateMassLoop();