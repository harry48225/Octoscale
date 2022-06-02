<script lang="ts">
    import MassDisplay from './components/MassDisplay.svelte';
    import Timer from './components/Timer.svelte';
    import TareButon from './components/ActionButtons.svelte';

    import { Trace } from '@nativescript/core';
    import { BleTraceCategory } from '@nativescript-community/ble';
    import { onMount } from 'svelte';
    import { connectToScale, isConnected, displayedMass, tare, isTiming, timerDurationSeconds } from  './bleManager';
import ActionButtons from './components/ActionButtons.svelte';
import ConnectionBanner from './components/ConnectionBanner.svelte';
import Graph from './components/Graph.svelte';
    
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
        <flexboxLayout>
            <ConnectionBanner isConnected={connected}/>
            <MassDisplay mass={mass}/>
            <Timer isTiming={timing} durationSeconds={duration}/>
            <Graph/>
            <button style={"fle"} on:tap="{() => connectToScale()}">Refresh</button>
            <ActionButtons tareCallback={tare}/>
        </flexboxLayout>
    </page>
</frame>

<style lang="scss">
    page {
        background-color: whitesmoke;
        padding-bottom: 100px;
    }

    FlexboxLayout {
        flex-direction: column;
        justify-items: center;
        justify-content: space-between;
    }
</style>