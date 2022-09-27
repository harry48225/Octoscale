<script lang="ts">
  import MassDisplay from './components/MassDisplay.svelte';
  import { connectToScale, mass, displayedMass, isTiming, timerDurationSeconds, aButton, bButton } from  './bleManager';
  import ActionButtons from './components/ActionButtons.svelte';
  import ConnectionBanner from './components/ConnectionBanner.svelte';
  import Graph from './components/Graph.svelte';
  import BottomNavigationBar from './components/BottomNavigationBar.svelte';
  import { GraphData } from './models/GraphData';
  import { Unsubscriber, writable } from 'svelte/store';
        
  const graphData = writable<GraphData>([{x: 0, y: 0}]);

  let timing: boolean = false;

  let startTime = Date.now();

  let duration: number = 0;

  let massValue = 0;
  let animation = -1;

  let graphUpdateInterval = 8;
  let lastGraphUpdateMillis = -1;

  let timingUnsubscribe: Unsubscriber;
  let massUnsubscribe: Unsubscriber;
  let timerDurationUnsubscribe: Unsubscriber;

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
        if (timing) {
          let newData = [...data, {x: (millis - startTime)/1000, y: massValue}];
          if (newData.length > 1600) {
           newData = reduceData(newData);
           graphUpdateInterval *= 2;
          }
          return newData; 
        }

        return data;
      });
    }
    animation = requestAnimationFrame(updateGraph);
  }

  const resetGraph = () => {
    graphData.set([]);
    lastGraphUpdateMillis = 0;
    graphUpdateInterval = 16;
  }

  const create = async () => {
    console.log("onCreate");
    connectToScale();

    timingUnsubscribe = isTiming.subscribe(val => {
      const wasTiming = timing;
      timing = !!val;

      if (!wasTiming && timing) {
        startTime = Date.now();
        resetGraph();
        graphData.set([{x: 0, y: 0}])
      }
    });

    massUnsubscribe = displayedMass.subscribe(val => massValue = val);
    timerDurationUnsubscribe = timerDurationSeconds.subscribe(val => {duration = val;});
    resetGraph();
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