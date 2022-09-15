<script lang="ts">
  import MassDisplay from './components/MassDisplay.svelte';
  import Timer from './components/Timer.svelte';
  import { onMount } from 'svelte';
  import { connectToScale, displayedMass, tare, isTiming, timerDurationSeconds, aButton, bButton } from  './bleManager';
  import ActionButtons from './components/ActionButtons.svelte';
  import ConnectionBanner from './components/ConnectionBanner.svelte';
  import Graph from './components/Graph.svelte';
  import { navigate } from 'svelte-native';
  import Settings from './Settings.svelte';
  import BottomNavigationBar from './components/BottomNavigationBar.svelte';
        
  let mass: number;
  displayedMass.subscribe(val => mass = val);

  let timing: boolean;
  isTiming.subscribe(val => timing = !!val);

  let duration: number;
  timerDurationSeconds.subscribe(val => duration = val);

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
    <Graph/>
    <ActionButtons>
      <button on:tap="{aButton}" text="time" />
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