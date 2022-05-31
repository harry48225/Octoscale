<frame>
    <page>
        <button on:tap="{() => connectToScale()}">Refresh</button>
    </page>
</frame>

<script lang="ts">
    import Home from './components/Home.svelte';

    import { Bluetooth, getBluetoothInstance } from '@nativescript-community/ble';
    import { request } from '@nativescript-community/perms'
    import { onMount } from 'svelte';

    let connectToScale = async () => {
        let ble = new Bluetooth();
        console.log("connecting to scale");
        await request('bluetooth');
        await request('bluetoothConnect');
        await request('bluetoothScan');
        let bleEnabled = await ble.enable();

        console.log(bleEnabled ? "ble enabled" : "no ble");

        console.log("starting scan");
        let scaleUUID = null;
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

        await ble.connect({
            UUID: scaleUUID,
            onConnected: function (connectedPeripheral) {
            console.log("Periperhal connected with UUID: " + connectedPeripheral.UUID);
            console.log(connectedPeripheral);
            // the peripheral object now has a list of available services:
            for (let service of connectedPeripheral.services!) {
                console.log("service found: " + JSON.stringify(service) + "\n");
            }
            },
            onDisconnected: function (connectedPeripheral) {
                console.log("disconnected with UUID: " + connectedPeripheral.UUID);
            }
        })

        await ble.isConnected({UUID:scaleUUID});

    } 

    onMount(async () => {
        await connectToScale();
        console.log("connect to scale finished");
    })
</script>