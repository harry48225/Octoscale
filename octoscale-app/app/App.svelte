<frame>
    <page>
        <stackLayout>
            <button on:tap="{() => connectToScale()}">Refresh</button>
            <button on:tap="{readChar}">Read</button>
            <label text="{connected}"/>
            <label text="{Math.round(displayedMass*10)/10}"/>
        </stackLayout>
    </page>
</frame>

<script lang="ts">
    import Home from './components/Home.svelte';
    import { Trace } from '@nativescript/core';
    import { BleTraceCategory } from '@nativescript-community/ble';
    import { Bluetooth, getBluetoothInstance } from '@nativescript-community/ble';
    import { request } from '@nativescript-community/perms'
    import { onMount } from 'svelte';

    Trace.addCategories(BleTraceCategory);
    Trace.addCategories(Trace.categories.All);
    Trace.enable();

    let scaleUUID: string | null = null;
    // IF YOU'RE HAVING ISSUES DISABLE AND THEN REENABLE BLUETOOTH
    let connectToScale = async () => {
        let ble = getBluetoothInstance();
        console.log("connecting to scale");
        await request('bluetooth');
        await request('bluetoothConnect');
        await request('bluetoothScan');
        ble.clear();
        let bleEnabled = await ble.enable();

        console.log(bleEnabled ? "ble enabled" : "no ble");

        console.log("starting scan");

        await ble.startScanning({
            filters:[{serviceUUID: 'ade4af7e-f409-473c-ace4-c49d11393be3'}],
            seconds: 4,
            onDiscovered: (peripheral) => {
                console.log(`found ${peripheral.name}`)
                console.log(peripheral);
                scaleUUID = peripheral.UUID;
                ble.stopScanning();
                
            }
        })
        if (!scaleUUID) {
            return;
        }
        console.log(`found scale has uuid ${scaleUUID}`);

        await ble.stopScanning();


        console.log("_connection_")
        let connection = await ble.connect({
            UUID: scaleUUID,
            onConnected:  async (connectedPeripheral) => {
            console.log("Periperhal connected with UUID: " + connectedPeripheral.UUID);
            console.log(connectedPeripheral);
            connected = true;
            },
            onDisconnected: function (connectedPeripheral) {
                console.log("disconnected with UUID: " + connectedPeripheral.UUID);
                connected = false;
                //connectToScale();
            }
        })  
        ble.clearAdvertismentCache();
        let services = await ble.discoverAll({peripheralUUID: scaleUUID!});
        for (let s of services.services) {
            console.log(s);
        }
        // Trace.write(JSON.stringify(services), Trace.categories.Debug);
        // //await ble.connect({UUID: scaleUUID});
        // //console.log("reconnect");
        console.log("connect to scale finished");

        setTimeout(readCharLoop, 20);
    } 

    //let interpolatingAmount: number | null = null;

    // let adjustMass = (adjustment: number, count: number, limit: number) => {
    //     displayedMass += adjustment;
    //     count++;
    //     if (count != limit) {
    //         setTimeout(() => adjustMass(adjustment, count+1, limit), 10);
    //     }
    // }

    let interpolateMassLoop = async () => {
        // Should probably use a 1euro filter
        displayedMass = displayedMass * 0.6 + mass * 0.4;
        setTimeout(interpolateMassLoop, 10);
    }

    setTimeout(interpolateMassLoop, 100);

    let readCharLoop = async () => {
        await readChar()
        setTimeout(readCharLoop, 100);
    }

    let readChar = async () => {
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

    onMount(async () => {
        await connectToScale();
    })

    export let connected = false;
    export let mass = 0;
    export let displayedMass = 0.0;
</script>