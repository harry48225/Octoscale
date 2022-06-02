<script lang="ts">
    import MassDisplay from './components/MassDisplay.svelte';
    import Timer from './components/Timer.svelte';

    import { Trace } from '@nativescript/core';
    import { BleTraceCategory } from '@nativescript-community/ble';
    import { onMount } from 'svelte';
    import { connectToScale, isConnected, displayedMass, tare, isTiming, timerDurationSeconds } from  './bleManager';
    
    //Trace.addCategories(BleTraceCategory);
    //Trace.addCategories(Trace.categories.All);
    //Trace.enable();

    let mass: number;
    displayedMass.subscribe(val => mass = val);

    let connected: boolean;
    isConnected.subscribe(val => connected = val);

    let timing: boolean;
    isTiming.subscribe(val => timing = !!val);

    let duration: number;
    timerDurationSeconds.subscribe(val => duration = val);

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
            <button on:tap="{tare}">Tare</button>
            <Timer isTiming={timing} durationSeconds={duration}/>
        </stackLayout>
    </page>
</frame>