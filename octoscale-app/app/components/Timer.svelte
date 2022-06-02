<script lang="ts">
  import { fade } from 'svelte-native/transitions';

  export let isTiming: boolean;
  export let durationSeconds: number;

  const makeTwoDigits = (num: number) => {
    return (num+"").padStart(2, "0");
  }

  $: timeString = `${makeTwoDigits(Math.floor(durationSeconds / 60))}:${makeTwoDigits(durationSeconds % 60)}`;
</script>

<flexboxLayout>
  {#if isTiming}
  <label class="duration" text="{timeString}"/>
  {/if}
</flexboxLayout>


<style lang="scss">
  FlexboxLayout {
    text-align: center;
    font-size: 40px;
    color: lightcoral;
    justify-content: space-around;
    padding-left: 20em;
    width: 100%;
    height:200px;
    Label {
      width: 100%;
      text-shadow: 4px 4px 2px gray;
    }
  }
</style>