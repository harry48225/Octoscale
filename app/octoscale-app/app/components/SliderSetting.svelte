<script lang="ts">
  import { TextField } from "@nativescript/core";

  // Hacky workaround to the on:returnPress type not existing on text fields
  const onReturnPress = {onreturnPress : (e: { object: { text: string; }; }) => {value=parseFloat(e.object.text)}}


  export let name: String;

  const stepSize = 0.001

  // $: stepSize = 10**(Math.floor(Math.log10(value)))
  // $: console.log(stepSize)
  const max = 0.1;
  const min = 0;
  let value: number = 0
</script>
  
<flexboxLayout>
  <label textWrap={true}>{name}</label>
  <slider minValue={min/stepSize} maxValue={max/stepSize} value={value/stepSize} on:valueChange={(e) => {
    const newValue = e.object.value * stepSize
    if (value != newValue) {
      value = newValue
    }
    }}/>
  <textField text={value.toString()} returnKeyType="done" closeOnReturn={true} keyboardType="number"
  {...onReturnPress}/>
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