<script lang="ts">
  export let label: String;
</script>

<flexboxLayout>
  <label textWrap={true}>{label}</label>
  <slot></slot>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    justify-content: space-between;
    align-items: center;
    padding-left: 32px;
    padding-right: 32px;

    Label {
      font-size: 12px;
      text-align: center;
      max-width: 200px;
      width: 200px;
    }
  }
</style>