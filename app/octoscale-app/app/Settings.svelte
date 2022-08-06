<script lang="ts">

import { navigate } from "svelte-native";
import BottomNavigationBar from "./components/BottomNavigationBar.svelte";
import ConnectionBanner from "./components/ConnectionBanner.svelte";
import SliderSetting from "./components/SliderSetting.svelte";
import Home from "./Home.svelte";

</script>

<page>
  <actionBar title="Settings">
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
    <BottomNavigationBar/>
  </flexboxLayout>
</page>

<style lang="scss">
  FlexboxLayout {
    StackLayout {
      flex-grow: 1;
      margin: 32px;
    }
  }
</style>