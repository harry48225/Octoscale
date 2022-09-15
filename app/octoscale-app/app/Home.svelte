<script lang="ts">
  import MassDisplay from './components/MassDisplay.svelte';
  import Timer from './components/Timer.svelte';
  import { onMount } from 'svelte';
  import { connectToScale, mass, displayedMass, isTiming, timerDurationSeconds, aButton, bButton } from  './bleManager';
  import ActionButtons from './components/ActionButtons.svelte';
  import ConnectionBanner from './components/ConnectionBanner.svelte';
  import Graph from './components/Graph.svelte';
  import BottomNavigationBar from './components/BottomNavigationBar.svelte';
  import { GraphData } from './models/GraphData';
  import { writable } from 'svelte/store';
        
  const graphData = writable<GraphData>([]);

  let timing: boolean = false;
  isTiming.subscribe(val => {
    const wasTiming = timing;
    timing = !!val;

    if (!wasTiming && timing) {
      startTime = Date.now();
      graphData.set([]);
    }
  });
  let startTime = Date.now();

  let duration: number = 0;
  timerDurationSeconds.subscribe(val => {duration = val;});

  let massValue = 0;

  const updateGraph = () => {
    const millis = Date.now();
    graphData.update((data) => [...data, {x: (millis - startTime)/1000, y: massValue}]);

    setTimeout(updateGraph, 16);
  }

  onMount(async () => {
    await connectToScale();

    mass.subscribe(val => massValue = val);

    updateGraph();
  });
</script>

<page>
  <actionBar title="octoscale">
  </actionBar>
  <flexboxLayout>
    <ConnectionBanner/>
    <MassDisplay/>
    <Timer isTiming={timing} durationSeconds={duration}/>
    <Graph dataStore={graphData}/>
    <ActionButtons>
      <button on:tap="{aButton}" text="time"/>
      <button on:tap="{bButton}" text="tare"/>
    </ActionButtons>
    <BottomNavigationBar/>
  </flexboxLayout>
</page>

<style lang="scss">
  FlexboxLayout {
    flex-direction: column;
    justify-items: center;
    justify-content: space-between;
  }
</style>