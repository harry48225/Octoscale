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
  let updateInterval = 50;
  let graphLock = false;
  let runningGraphId = "";
  let animation = -1;

  const updateGraph = (id: string) => {
    //console.log(`${Date.now()}, ${id}, ${runningGraphId}`)
    if (graphLock || id !== runningGraphId) return;
    graphLock = true;
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

        reducedData.push(newData[newData.length - 1]);

        updateInterval *= 2;
        newData = reducedData
      }
      return newData;
    });
    graphLock = false;
    //setTimeout(() => updateGraph(id), updateInterval);
    animation = requestAnimationFrame(() => updateGraph(runningGraphId));
  }

  let timingUnsubscribe: Unsubscriber;
  let massUnsubscribe: Unsubscriber;
  let timerDurationUnsubscribe: Unsubscriber;

  onMount(async () => {
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

    runningGraphId = Math.random().toFixed(3) + Math.random().toFixed(3);
    //updateGraph(runningGraphId);
    animation = requestAnimationFrame(() => updateGraph(runningGraphId));
  });


  const destroy = () => {
    console.log("onDestroy")
    timingUnsubscribe();
    massUnsubscribe();
    timerDurationUnsubscribe();
    runningGraphId = "";
    cancelAnimationFrame(animation);
  }
</script>

<page on:navigatedFrom={destroy}>
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