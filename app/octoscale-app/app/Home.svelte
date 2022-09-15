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
  let updateInterval = 16;
  let updatingGraph = false;

  const updateGraph = () => {
    const millis = Date.now();
    graphData.update((data) => {
      let newData = [...data, {x: (millis - startTime)/1000, y: massValue}];
      if (newData.length > 600) {
        const reducedData = [newData[0]];
        for (let i = 1; i < newData.length - 1; i+=2) {
          reducedData.push(
            {
              x: (newData[i].x + newData[i+1].x) / 2 ,
              y: (newData[i].y + newData[i+1].y) / 2
            }
          )
        }

        updateInterval *= 2;
        newData = reducedData
      }
      return newData;
    });
    setTimeout(updateGraph, updateInterval);
  }

  onMount(async () => {
    await connectToScale();

    mass.subscribe(val => massValue = val);

    if (!updatingGraph) {
      updatingGraph = true;
      updateGraph();
    }
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