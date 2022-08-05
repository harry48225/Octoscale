<script lang="ts" context="module">
  export type ReturnPressEvent = CustomEvent<{ object: { text: string; }; }>;
</script>

<script lang="ts">
  // @ts-nocheck
  // Disable error checking to disable errors for the on:returnPress
  // a bit of a hack...
  import { createEventDispatcher } from "svelte";
  export let value: number; 
  const dispatch = createEventDispatcher();

  

  const returnPress = (e: ReturnPressEvent) => {
    dispatch('returnPress', e)
  };
</script>

<textField text={value.toString()} 
  returnKeyType="done" 
  closeOnReturn="true" 
  keyboardType="number" 
  on:returnPress={returnPress}
>

</textField>