<script lang="ts">
  import { isConnected } from '../bleManager';
  let connected: boolean;
  isConnected.subscribe(val => connected = val);

</script>

<label text="{connected ? 'connected' : 'disconnected'}" style="background-color:{connected ? "darkgreen" : "darkred"}"/>

<style lang="scss">
  Label {
    font-size: 16px;
    color: white;
    width: 100%;
    text-align: center;
    height: 72px;
    min-height: 72px;
  }
</style>