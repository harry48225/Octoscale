<script lang="ts">
  import MassDisplay from './components/MassDisplay.svelte';
  import Timer from './components/Timer.svelte';
  import { onMount } from 'svelte';
  import { connectToScale, displayedMass, isTiming, timerDurationSeconds, aButton, bButton } from  './bleManager';
  import ActionButtons from './components/ActionButtons.svelte';
  import ConnectionBanner from './components/ConnectionBanner.svelte';
  import Graph from './components/Graph.svelte';
  import BottomNavigationBar from './components/BottomNavigationBar.svelte';
  import { GraphData } from './models/GraphData';
  import { writable } from 'svelte/store';
        
  const graphData = writable<GraphData>([]);
  let mass: number;
  displayedMass.subscribe(val => mass = val);

  let timing: boolean;
  isTiming.subscribe(val => timing = !!val);

  let duration: number;
  timerDurationSeconds.subscribe(val => {
    duration = val;

    if (duration === 0) graphData.set([]);

    graphData.update((data) => [...data, {x: duration, y: mass}]);
  });

  onMount(async () => {
      await connectToScale();
  });
</script>

<page>
  <actionBar title="octoscale">
  </actionBar>
  <flexboxLayout>
    <ConnectionBanner/>
    <MassDisplay mass={mass}/>
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