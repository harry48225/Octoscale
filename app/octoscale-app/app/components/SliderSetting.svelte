<script lang="ts">
  import NumberField, { ReturnPressEvent } from "./NumberField.svelte";
  const PRECISION = 3
  export let name: String
  const stepSize = 0.001

  const updateValue = (newValue: number) => {
    if (newValue.toPrecision(PRECISION) == value.toPrecision(PRECISION)) return
    value = parseFloat(newValue.toPrecision(PRECISION))
  }
  const updateValueFromNumberField = (e: ReturnPressEvent) => {
    updateValue(parseFloat(e.detail.object.text));
  }

  // $: stepSize = 10**(Math.floor(Math.log10(value)))
  // $: console.log(stepSize)
  const max = 0.1;
  const min = 0;
  let value: number = 0
</script>
  
<flexboxLayout>
  <label textWrap={true}>{name}</label>
  <slider minValue={min/stepSize} maxValue={max/stepSize} value={value/stepSize} on:valueChange={e => {
    updateValue(e.value * stepSize)}}/>
  <NumberField bind:value={value} on:returnPress = {updateValueFromNumberField}/>
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

    NumberField {
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