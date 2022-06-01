<script lang="ts">
    import MassDisplay from './components/MassDisplay.svelte';

    import { Trace } from '@nativescript/core';
    import { BleTraceCategory } from '@nativescript-community/ble';
    import { onMount } from 'svelte';
    import { connectToScale, isConnected, displayedMass } from  './bleManager';
    
    Trace.addCategories(BleTraceCategory);
    Trace.addCategories(Trace.categories.All);
    Trace.enable();

    let mass: number;
    displayedMass.subscribe(val => mass = val);

    let connected: boolean;
    isConnected.subscribe(val => connected = val);

    onMount(async () => {
        await connectToScale();
    })
</script>

<frame>
    <page>
        <stackLayout>
            <MassDisplay mass={mass}/>
            <button on:tap="{() => connectToScale()}">Refresh</button>
            <label text="{connected}"/>
        </stackLayout>
    </page>
</frame>