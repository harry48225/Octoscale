<script lang="ts">
  export let isTiming: boolean;
  export let durationSeconds: number;

  const makeTwoDigits = (num: number) => {
    return (num+"").padStart(2, "0");
  }

  $: timeString = `${makeTwoDigits(Math.floor(durationSeconds / 60))}:${makeTwoDigits(durationSeconds % 60)}`;
</script>

<flexboxLayout>
  <label text="{isTiming ? "T" : "-"}"/>
  <label class="duration" text="{timeString}"/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    background-color: darkcyan;
    text-align: center;
    font-size: 100px;
    color: white;
    justify-content: space-around;
    padding-left: 20em;
    Label {
      text-shadow: 10px 20px 10px darkgrey;
    }
  }

  .duration {
    flex-grow: 1;
  }
</style>