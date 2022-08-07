<script lang="ts" context="module">
  export type ReturnPressEvent = CustomEvent<{ object: { text: string; }; }>;
</script>

<script lang="ts">
  // @ts-nocheck
  // Disable error checking to disable errors for the on:returnPress
  // a bit of a hack...
  import { createEventDispatcher } from "svelte"; 
  const dispatch = createEventDispatcher();
  const returnPress = (e: ReturnPressEvent) => {
    dispatch('returnPress', e)
  };

  export let value: string;
</script>

<textField text={value} 
  returnKeyType="done" 
  closeOnReturn="true" 
  keyboardType="number" 
  on:returnPress={returnPress}
/>

<style lang="scss">
  TextField {
      font-size: 16px;
      text-align: right;
      min-width: 120px;
      max-width: 120px;
    }
</style>