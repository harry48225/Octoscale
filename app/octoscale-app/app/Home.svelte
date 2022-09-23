<script lang="ts">
  import MassDisplay from './components/MassDisplay.svelte';
  import Timer from './components/Timer.svelte';
  import { onMount, onDestroy, beforeUpdate } from 'svelte';
  import { connectToScale, mass, displayedMass, isTiming, timerDurationSeconds, aButton, bButton } from  './bleManager';
  import ActionButtons from './components/ActionButtons.svelte';
  import ConnectionBanner from './components/ConnectionBanner.svelte';
  import Graph from './components/Graph.svelte';
  import BottomNavigationBar from './components/BottomNavigationBar.svelte';
  import { GraphData } from './models/GraphData';
  import { Unsubscriber, writable } from 'svelte/store';
        
  const graphData = writable<GraphData>([]);

  let timing: boolean = false;

  let startTime = Date.now();

  let duration: number = 0;

  let massValue = 0;
  let animation = -1;

  let graphUpdateInterval = 16;
  let lastGraphUpdateMillis = -1;

  const reduceData = (data: GraphData) => {
    const reducedData = [data[0]];
    for (let i = 1; i < data.length - 1; i+=2) {
      reducedData.push(
        {
          x: (data[i].x + data[i+1].x) / 2 ,
          y: (data[i].y + data[i+1].y) / 2
        }
      )
    }

    reducedData.push(data[data.length - 1]);
    return reducedData;
  }

  const updateGraph = () => {
    const millis = Date.now();

    if (millis - lastGraphUpdateMillis > graphUpdateInterval) {
      lastGraphUpdateMillis = Date.now();
      graphData.update((data) => {
        let newData = [...data, {x: (millis - startTime)/1000, y: massValue}];
        if (newData.length > 600) {
          newData = reduceData(newData);
          graphUpdateInterval *= 2;
        }
        return newData;
      });
    }
    animation = requestAnimationFrame(updateGraph);
  }

  let timingUnsubscribe: Unsubscriber;
  let massUnsubscribe: Unsubscriber;
  let timerDurationUnsubscribe: Unsubscriber;

  const create = async () => {
    await connectToScale();

    timingUnsubscribe = isTiming.subscribe(val => {
      const wasTiming = timing;
      timing = !!val;

      if (!wasTiming && timing) {
        startTime = Date.now();
        graphData.set([]);
      }
    });

    massUnsubscribe = mass.subscribe(val => massValue = val);
    timerDurationUnsubscribe = timerDurationSeconds.subscribe(val => {duration = val;});
    animation = requestAnimationFrame(updateGraph);
  };


  const destroy = () => {
    console.log("onDestroy")

    try {
      timingUnsubscribe();
      massUnsubscribe();
      timerDurationUnsubscribe();
      cancelAnimationFrame(animation);
    } catch {}
  }
</script>

<page on:navigatedFrom={destroy} on:navigatedTo={create}>
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
    <BottomNavigationBar currentPage="home"/>
  </flexboxLayout>
</page>

<style lang="scss">
  FlexboxLayout {
    flex-direction: column;
    justify-items: center;
    justify-content: space-between;
  }
</style>