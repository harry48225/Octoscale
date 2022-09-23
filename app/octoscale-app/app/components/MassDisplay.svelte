<script lang="ts">
  import { FlexboxLayout } from '@nativescript/core';
import { displayedMass } from '../bleManager';
  import Timer from './Timer.svelte';
  let mass = "0.0";
  displayedMass.subscribe((val) => mass = val.toFixed(1));
</script>

<flexboxLayout>
  <label>{mass}
    <!-- <formattedString>
      <span text="{mass}"/>
    </formattedString> -->
  </label>
  <!-- <Timer durationSeconds={100}/> -->
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    flex: none;
    background-color: #E4E6C3;
    margin: 16px;
    border-radius: 64px;
    padding-top: -16px;
    justify-content: space-around;

    Label {
      text-align: center;
      font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
      font-size: 96px;
      color: #222725;
      margin: 16px 32px;
      min-height: 120px;
    }
  }
</style>