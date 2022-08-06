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
    background-color: #E4E6C3;
    margin: 16px;
    padding: 16px;
    border-radius: 64px;
  }
</style>