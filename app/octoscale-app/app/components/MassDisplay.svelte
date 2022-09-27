<script lang="ts">
  import { displayedMass, isTiming } from '../bleManager';
  import Timer from './Timer.svelte';
  let mass = "0.0";
  displayedMass.subscribe((val) => mass = val.toFixed(1));

  let timing = false;
  isTiming.subscribe((state) => {
    timing = timing || !!state;
  })
</script>

<flexboxLayout>
  <label>{mass}</label>
  {#if timing}
    <Timer/>
  {/if}
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    flex: none;
    flex-direction: column;
    background-color: #E4E6C3;
    margin: 16px;
    border-radius: 64px;
    padding: 16px;
    justify-content: space-around;

    Label {
      text-align: center;
      font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
      font-size: 96px;
      color: #222725;
    }
  }
</style>