<script lang="ts">
  export let isConnected: boolean;
</script>

<label text="{isConnected ? 'connected to scale' : 'not connected to scale'}"/>

<style lang="scss">
  Label {
    font-size: 20px;
    color: white;
    background-color: slategray;
    width: 100%;
    text-align: center;
  }
</style>