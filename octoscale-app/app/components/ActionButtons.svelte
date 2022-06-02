<script lang="ts">
  export let tareCallback: () => {};
</script>

<flexboxLayout>
  <button on:tap="{tareCallback}" text="tare"/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    justify-content: space-around;
  }
  Button {
    font-size: 40px;
    border-radius: 50%;
    width: 300px;
    height: 300px;
    box-shadow: 10px 10px 2px grey;
  }
</style>