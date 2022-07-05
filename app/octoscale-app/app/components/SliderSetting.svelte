<script lang="ts">
  export let name: String;
  let rawValue: number = 0;
  $: value = rawValue / 100.0
</script>
  
<flexboxLayout>
  <label textWrap={true}>{name}</label>
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
      font-size: 12px;
      text-align: center;
      max-width: 200px;
      width: 200px;
    }
  }
</style>