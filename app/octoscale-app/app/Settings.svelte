<script lang="ts">

import { navigate } from "svelte-native";
import SliderSetting from "./components/SliderSetting.svelte";
import Home from "./Home.svelte";

</script>

<page>
  <actionBar title="Settings">
    <actionItem text="home" on:tap={() => {navigate({page: Home})}}/>
  </actionBar>
  <flexboxLayout flexDirection="column">
    <stackLayout>
      <label>Developer settings</label>
      <SliderSetting name="min cut-off"/>
      <SliderSetting name="D cut-off"/>
      <SliderSetting name="beta"/>
      <SliderSetting name="settled tolerance"/>
      <SliderSetting name="unsettled tolerance"/>
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
    }
  }
</style>