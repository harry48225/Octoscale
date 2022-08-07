<script>

import { navigate } from "svelte-native";
import Settings from "~/Settings.svelte";
import Home from '../Home.svelte';

</script>
<flexboxLayout>
  <button text="🏠" on:tap={() => navigate({page: Home})}/>
  <button text="🔧" on:tap={() => navigate({page: Settings})}/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    background-color: #899878;
    padding: 16px;
    max-height: 196px;
    min-height: 196px;
    height: 196px;
    border-top-width: 8px;
    border-top-color: #121113;
    border-top-style: solid;
    justify-content: space-around;
    flex-direction: row;
  }

  Button {
    font-size: 28px;
    background-color: #E4E6C3;
    color: #899878;
    width: 160px;
    height: 160px;
    min-height: 160px;
  }
</style>