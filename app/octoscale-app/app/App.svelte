<script>
import Home from "./Home.svelte";
import Settings from "./Settings.svelte";

</script>
<frame>
  <Home/>
</frame>

<style global lang="scss">
  Page {
      background-color: #F7F7F2;
  }

  ActionBar {
        background-color: #899878;
        color: #F7F7F2;
    }

  Label {
    font-size: 16px;
    color: #121113;
  }

  Button {
    font-size: 24px;
    height: auto;
    width: auto;
    border-radius: 20%;
    padding: 16px 64px;
    color: #F7F7F2;
    background-color: #899878;
  }

  Slider {
    color: #899878;
    background-color: #899878;
  }
</style>
// Colours https://coolors.co/f7f7f2-e4e6c3-899878-9b1d20-121113