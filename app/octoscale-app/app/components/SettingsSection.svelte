<script lang="ts">
  export let title: String;
</script>

<flexboxLayout>
  <label>{title}</label>
  <stackLayout>
    <slot/>
  </stackLayout>
</flexboxLayout>

<style lang="scss">
  Label {
    background-color: #899878;
    padding: 32px;
    border-radius: 32px;
    font-weight: 500;
    color: #F7F7F2;
  }

  FlexboxLayout {
    background-color: #E4E6C3;
    border-radius: 32px;
    padding: 64px;
    flex-direction: column;
    margin-bottom: 64px;
  }
</style>