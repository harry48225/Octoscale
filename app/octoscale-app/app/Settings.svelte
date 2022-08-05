<script lang="ts">

import { navigate } from "svelte-native";
import ConnectionBanner from "./components/ConnectionBanner.svelte";
import SliderSetting from "./components/SliderSetting.svelte";
import Home from "./Home.svelte";

</script>

<page>
  <actionBar title="Settings">
    <actionItem text="home" on:tap={() => {navigate({page: Home})}}/>
  </actionBar>
  <flexboxLayout flexDirection="column">
    <ConnectionBanner/>
    <stackLayout>
      <label>Developer settings</label>
      <SliderSetting label="min cut-off"/>
      <SliderSetting label="D cut-off"/>
      <SliderSetting label="beta"/>
      <SliderSetting label="settled tolerance"/>
      <SliderSetting label="unsettled tolerance"/>
    </stackLayout>

    <flexboxLayout justifyContent="space-around">
      <button>query scale</button>
      <button>apply</button>
    </flexboxLayout>
  </flexboxLayout>
</page>

<style lang="scss">
  Label {
    font-size: 16px;
  }

  FlexboxLayout {
    StackLayout {
      flex-grow: 1;
      margin: 32px;
    }
  }
</style>