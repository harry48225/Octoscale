<script lang="ts">
  export let mass: number;

  $: displayedMass = mass.toFixed(1);
</script>

<stackLayout>
  <label>
    <formattedString>
      <span text="{displayedMass}"/>
    </formattedString>
  </label>
</stackLayout>

<style lang="scss">
  StackLayout {
    background-color: #E4E6C3;
    margin: 16px;
    border-radius: 64px;

    Label {
    text-align: center;
    width:100%;
    font-family:'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
    font-size: 100px;
    color: #222725;
  }
  }
</style>