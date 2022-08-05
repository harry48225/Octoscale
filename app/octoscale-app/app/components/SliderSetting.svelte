<script lang="ts">
  import NumberField, { ReturnPressEvent } from "./NumberField.svelte";
  const SIGNIFICANT_FIGURES = 3;
  export let label: String
  export let min = 0;
  export let max = 1;
  export let value = 0;
  $: stepSize = 10**(Math.floor(Math.log10(max)) - SIGNIFICANT_FIGURES);

  const updateValue = (newValue: number) => {
    if (newValue.toPrecision(SIGNIFICANT_FIGURES) === value.toPrecision(SIGNIFICANT_FIGURES)) return
    if (newValue > max || newValue < min) return;
    value = parseFloat(newValue.toPrecision(SIGNIFICANT_FIGURES));
  }
  const updateValueFromNumberField = (e: ReturnPressEvent) => {
    updateValue(parseFloat(e.detail.object.text));
  }
</script>
  
<flexboxLayout>
  <label textWrap={true}>{label}</label>
  <slider minValue={min/stepSize} 
    maxValue={max/stepSize} 
    value={value/stepSize} 
    on:valueChange={e => {updateValue(e.value * stepSize)}}/>
  <NumberField value={value.toPrecision(SIGNIFICANT_FIGURES)}
    on:returnPress={updateValueFromNumberField}/>
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

    Label {
      font-size: 12px;
      text-align: center;
      max-width: 200px;
      width: 200px;
    }
  }
</style>