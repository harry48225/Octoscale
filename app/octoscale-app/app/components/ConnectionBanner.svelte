<script lang="ts">
  export let isConnected: boolean;
</script>

<label text="{isConnected ? 'connected' : 'disconnected'}" style="background-color:{isConnected ? "darkgreen" : "darkred"}"/>

<style lang="scss">
  Label {
    font-size: 16px;
    color: white;
    width: 100%;
    text-align: center;
  }
</style>