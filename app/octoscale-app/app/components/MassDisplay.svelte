<script lang="ts">
  import { displayedMass } from '../bleManager';
  let mass = "0.0";
  displayedMass.subscribe((val) => mass = val.toFixed(1));
</script>

<stackLayout>
  <label>
    <formattedString>
      <span text="{mass}"/>
    </formattedString>
  </label>
</stackLayout>

<style lang="scss">
  StackLayout {
    background-color: #E4E6C3;
    margin: 16px;
    border-radius: 64px;
    min-height: 360px;
    padding-top: -16px;

    Label {
      text-align: center;
      width: 100%;
      font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
      font-size: 96px;
      color: #222725;
  }
  }
</style>