<script>

import { navigate } from "svelte-native";
import Home from "./Home.svelte";

</script>

<page>
  <actionBar title="Settings">
    <actionItem text="home" on:tap={() => {navigate({page: Home})}}/>
  </actionBar>
  <label>Hello world</label>
</page>