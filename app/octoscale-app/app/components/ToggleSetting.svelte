<script lang="ts">
  import LabelledSetting from "./LabelledSetting.svelte";
  export let label: string;
  let checked: boolean = false;
</script>

<LabelledSetting label={label}>
  <switch bind:checked={checked}/>
</LabelledSetting>

<style lang="scss">
  Switch[checked=true] {
    color: #899878;
  }

  Switch[checked=false] {
    color: #F7F7F2;
  }
</style>