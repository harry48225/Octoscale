<script lang="ts">
import LabelledSetting from "./LabelledSetting.svelte";

  import NumberField, { ReturnPressEvent } from "./NumberField.svelte";
  export let SIGNIFICANT_FIGURES = 3;
  export let label: string
  export let min = 0;
  export let max = 1;
  export let value = 0;
  export let stepSize = 10**(Math.floor(Math.log10(max)) - SIGNIFICANT_FIGURES);

  const updateValue = (newValue: number) => {
    if (newValue.toPrecision(SIGNIFICANT_FIGURES) === value.toPrecision(SIGNIFICANT_FIGURES)) return
    if (newValue > max || newValue < min) return;
    value = parseFloat(newValue.toPrecision(SIGNIFICANT_FIGURES));
  }
  const updateValueFromNumberField = (e: ReturnPressEvent) => {
    updateValue(parseFloat(e.detail.object.text));
  }
</script>

<LabelledSetting label={label}>
  <slider minValue={min/stepSize} 
    maxValue={max/stepSize} 
    value={value/stepSize} 
    on:valueChange={e => {updateValue(e.value * stepSize)}}/>
  <NumberField value={value.toPrecision(SIGNIFICANT_FIGURES)}
    on:returnPress={updateValueFromNumberField}/>
</LabelledSetting>

<style lang="scss">
  Slider {
    flex-grow: 1;
  }
</style>