<script lang="ts">

import { navigate } from "svelte-native";
import SliderSetting from "./components/SliderSetting.svelte";
import Home from "./Home.svelte";

let betaValue = 10
</script>

<page>
  <actionBar title="Settings">
    <actionItem text="home" on:tap={() => {navigate({page: Home})}}/>
  </actionBar>
  <stackLayout>
    <label>Advanced settings</label>
    <SliderSetting name="min cut-off"/>
    <SliderSetting name="D cut-off"/>
    <SliderSetting name="beta"/>
  </stackLayout>
</page>

<style lang="scss">

</style>