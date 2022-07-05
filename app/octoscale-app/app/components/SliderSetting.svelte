<script lang="ts">
  export let name: String;
  let rawValue: number = 0;
  $: value = rawValue / 100.0
</script>
  
<flexboxLayout>
  <label>{name}</label>
  <slider minValue=0 maxValue=100 bind:value={rawValue}/>
  <textField bind:text={value} keyboardType="number"/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    justify-content: space-between;
    align-items: center;
    padding-left: 32px;
    padding-right: 32px;

    Slider {
      flex-grow: 1;
    }

    TextField {
      font-size: 16px;
      text-align: right;
      min-width: 100px;
    }

    Label {
      font-size: 16px;
      text-align: center;
      min-width: 240px;
    }
  }
</style>