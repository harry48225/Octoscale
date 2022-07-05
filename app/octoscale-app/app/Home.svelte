<script lang="ts">
    import MassDisplay from './components/MassDisplay.svelte';
    import Timer from './components/Timer.svelte';
    import { onMount } from 'svelte';
    import { connectToScale, isConnected, displayedMass, tare, isTiming, timerDurationSeconds } from  './bleManager';
    import ActionButtons from './components/ActionButtons.svelte';
    import ConnectionBanner from './components/ConnectionBanner.svelte';
    import Graph from './components/Graph.svelte';
    import { navigate } from 'svelte-native';
    import Settings from './Settings.svelte';
        
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


<page>
    <actionBar title="octoscale">
        <actionItem text="settings" on:tap={() => navigate({page: Settings})}/>
    </actionBar>
    <flexboxLayout>
        <ConnectionBanner isConnected={connected}/>
        <MassDisplay mass={mass}/>
        <Timer isTiming={timing} durationSeconds={duration}/>
        <Graph/>
        <ActionButtons tareCallback={tare}/>
    </flexboxLayout>
</page>


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