<script lang="ts">
  export let mass: number;

  $: displayedMass = mass.toFixed(1);
</script>

<label>
  <formattedString>
    <span text="{displayedMass}"/>
  </formattedString>
</label>

<style lang="scss">
  Label {
    background-color: darkred;
    text-align: center;
    width:100%;
    font-family:'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
    font-size: 100px;
    color: white;
    text-shadow: 10px 20px 10px grey;
  }
</style>