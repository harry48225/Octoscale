<frame>
    <page>
        <stackLayout>
            <button on:tap="{() => connectToScale()}">Refresh</button>
            <label text="{connected}"/>
            <label text="{Math.round(mass*10)/10}"/>
        </stackLayout>
    </page>
</frame>

<script lang="ts">
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