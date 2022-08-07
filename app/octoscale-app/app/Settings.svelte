<script lang="ts">

import { navigate } from "svelte-native";
import ActionButtons from "./components/ActionButtons.svelte";
import BottomNavigationBar from "./components/BottomNavigationBar.svelte";
import ConnectionBanner from "./components/ConnectionBanner.svelte";
import SettingsSection from "./components/SettingsSection.svelte";
import SliderSetting from "./components/SliderSetting.svelte";
import ToggleSetting from "./components/ToggleSetting.svelte";
import Home from "./Home.svelte";

</script>

<page>
  <actionBar title="Settings">
  </actionBar>
  <flexboxLayout flexDirection="column">
    <ConnectionBanner/>
    <scrollView>
      <stackLayout>
        <SettingsSection title="General">
          <ToggleSetting label="auto tare"/>
          <ToggleSetting label="beeps"/>
        </SettingsSection>

        <SettingsSection title="Timer">
          <ToggleSetting label="auto start"/>
          <ToggleSetting label="auto stop"/>
        </SettingsSection>

        <SettingsSection title="Developer settings">
          <SliderSetting label="min cut-off"/>
          <SliderSetting label="D cut-off"/>
          <SliderSetting label="beta"/>
          <SliderSetting label="settled tolerance"/>
          <SliderSetting label="unsettled tolerance"/>
        </SettingsSection>
      </stackLayout>
    </scrollView>
    <ActionButtons>
      <button>query scale</button>
      <button>apply</button>
    </ActionButtons>
    <BottomNavigationBar/>
  </flexboxLayout>
</page>

<style lang="scss">
  ScrollView {
    flex-grow: 1;
    
    StackLayout {
      padding: 32px;
    }
  }
</style>