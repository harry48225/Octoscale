<script lang="ts">
  export let tareCallback: () => {};
</script>

<flexboxLayout>
  <button text="time" />
  <button on:tap="{tareCallback}" text="tare"/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    justify-content: space-around;
  }
  Button {
    font-size: 32px;
    width: 300px;
    border-radius: 10%;
    height: 300px;
  }
</style>