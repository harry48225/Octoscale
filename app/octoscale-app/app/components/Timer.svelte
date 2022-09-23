<script lang="ts">
  import { timerDurationSeconds } from  '../bleManager';

  let timeString = "00:00";

  const makeTwoDigits = (num: number) => {
    return (num+"").padStart(2, "0");
  }

  timerDurationSeconds.subscribe((seconds) => {
    timeString = `${makeTwoDigits(Math.floor(seconds / 60))}:${makeTwoDigits(seconds % 60)}`; 
  })
</script>

<flexboxLayout>
  <label class="duration" text="{timeString}"/>
</flexboxLayout>


<style lang="scss">
  FlexboxLayout {
    text-align: center;
    justify-content: space-around;
    padding-left: 20em;
    width: 100%;
    Label {
      width: 100%;
      font-size: 24px;
      font-weight: 900;
      color: #899878;
    }
  }
</style>