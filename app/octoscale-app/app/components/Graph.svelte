<script lang="ts">
  import { Canvas, CanvasView, Cap, DashPathEffect, Join, Paint, Path, Style } from '@nativescript-community/ui-canvas';
  import { FlexboxLayout } from '@nativescript/core';
  import { onMount } from 'svelte';
  import { Writable } from 'svelte/store';
  import { GraphData } from '~/models/GraphData';
  import SettingsSection from './SettingsSection.svelte';
  import SliderSetting from './SliderSetting.svelte';

  export let dataStore: Writable<GraphData>;

  let updating = false;

  let preinfusion: number;
  let pullTime: number;
  let targetMass: number;
  $: totalBrewTime = preinfusion + pullTime;

  const zeroOffset = 20;

  onMount(() => {
    dataStore.subscribe(val => update(val));
  })

  let rawData: GraphData;
  
  let canvasView: CanvasView | undefined;

  const drawFilledRegion = (canvas: Canvas, data: GraphData) => {
    const paint = new Paint();
    paint.setColor('#E4E6C3');
    paint.strokeWidth = 4;
    paint.setStyle(Style.FILL);
    paint.setStrokeJoin(Join.ROUND);
    paint.setStrokeCap(Cap.BUTT);
    const path = new Path();
    
    path.moveTo(data[0].x, 0);

    for (let i = 1; i < data.length - 2; i+=3) {
      path.cubicTo(data[i].x, data[i].y, data[i+1].x, data[i+1].y, data[i+2].x, data[i+2].y);
    }

    path.quadTo(data[data.length -2].x, data[data.length -2].y, data[data.length -1].x, data[data.length -1].y);

    path.lineTo(data[data.length - 1].x, 0);
    path.close();

    canvas.drawPath(path, paint);
  };

  const getDataScale = (canvasHeight: number, canvasWidth: number, filledProportionX: number = 0.9, filledProportionY: number = 0.9): number[] => {
    const maxY = Math.max(...rawData.map(p => p.y), targetMass + 10);
    const maxX = Math.max(...rawData.map(p => p.x), totalBrewTime + 5);

    const yScale = (canvasHeight/maxY) * filledProportionY;
    const xScale = (canvasWidth/maxX) * filledProportionX;
    return [xScale, yScale];
  }

  const drawGraphLine = (canvas: Canvas, data: GraphData) => {
    const paint = new Paint();
    paint.setColor('#899878');
    paint.strokeWidth = 4;
    paint.setStyle(Style.STROKE);
    paint.setStrokeJoin(Join.ROUND);
    paint.setStrokeCap(Cap.BUTT);
    const path = new Path();
    
    path.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length - 2; i+=3) {
      path.cubicTo(data[i].x, data[i].y, data[i+1].x, data[i+1].y, data[i+2].x, data[i+2].y);
    }

    path.quadTo(data[data.length -2].x, data[data.length -2].y, data[data.length -1].x, data[data.length -1].y);

    canvas.drawPath(path, paint);
  };

  const drawGraphEndMarker = (canvas: Canvas, data: GraphData) => {
    const paint = new Paint();
    paint.setColor('#899878');
    paint.setStyle(Style.FILL);
    const lastPoint = data[data.length - 1];
    canvas.drawCircle(lastPoint.x, lastPoint.y, 6, paint);
  };

  const drawTargetBrewLine = (canvas: Canvas, xScale: number, yScale: number) => {
    const data: GraphData = [{x: 0, y: 0}, {x: preinfusion, y: 0}, {x: totalBrewTime, y: targetMass}]
      .map((p) => ({x: p.x * xScale, y: (p.y * yScale) + zeroOffset}));

    const paint = new Paint();
    paint.setColor('#9B1D20');
    paint.strokeWidth = 4;
    paint.setStyle(Style.STROKE);
    paint.setStrokeJoin(Join.ROUND);
    paint.setStrokeCap(Cap.BUTT);
    paint.setPathEffect(new DashPathEffect([16, 16], 0));
    const path = new Path();
    
    path.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length; i++) {
      path.lineTo(data[i].x, data[i].y);
    }

    path.lineTo(canvas.getWidth(), data[data.length - 1].y);

    canvas.drawPath(path, paint);
  }

  const update = (newData: GraphData) => {
    if (updating) return
    rawData = newData;
    updating = true;
    canvasView?.nativeView.invalidate();
    updating = false;
  }

  const drawHorizontalTicks = (canvas: Canvas, xScale: number) => {
    const tickWidth = 2;
    
    const paint = new Paint();
    paint.setColor('#899878');
    paint.strokeWidth = tickWidth;
    paint.setStyle(Style.STROKE);
    paint.setStrokeJoin(Join.ROUND);
    paint.setStrokeCap(Cap.BUTT);
    for (let xCoordinate = tickWidth/2; xCoordinate < canvas.getWidth() - tickWidth/2; xCoordinate += 5*xScale) {
      canvas.drawLine(xCoordinate, 0, xCoordinate, canvas.getHeight() * 0.9, paint);

      // if (xCoordinate < (canvas.width as number) - 200) {
      //   console.log(`drawing tick at ${xCoordinate}, ${Math.floor((xCoordinate - tickWidth / 2)/xScale)}s`);
      //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
      //   ctx.fillText(`s`, xCoordinate, 200);
      //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
      // }
    }
  }

  const onDraw = (event: { canvas: Canvas}) => {
    const canvas = event.canvas;
    
    canvas.scale(1, -1, canvas.getWidth() / 2, canvas.getHeight() / 2);
    const [xScale, yScale] = getDataScale(canvas.getHeight(), canvas.getWidth());
    const scaledData = rawData.map(p => ({x: p.x * xScale, y: (p.y * yScale) + zeroOffset}));
    
    drawHorizontalTicks(canvas, xScale);
    
    if (rawData.length >= 3) {
      drawFilledRegion(canvas, scaledData);
      drawGraphLine(canvas, scaledData);
      drawGraphEndMarker(canvas, scaledData);
    }
    drawTargetBrewLine(canvas, xScale, yScale);
  }
</script>

<!-- <gridLayout id="main"> -->
  <flexboxLayout id="main">
  <carousel height="96%"
  width="100%" showIndicator={true} android:indicatorAnimation="worm" indicatorColor="#899878" indicatorColorUnselected="#E4E6C3">
    <carouselItem id="carousel-item">
      <flexboxLayout id="canvas-wrapper">
        <canvas id="canvas" bind:this={canvasView} on:draw={onDraw}/>
      </flexboxLayout>
    </carouselItem>
    <carouselItem id="carousel-item">
      <flexboxLayout>  
        <SettingsSection title="Brew designer">
          <SliderSetting bind:value={preinfusion} label="Preinfusion" stepSize=1 SIGNIFICANT_FIGURES=2 max=20/>
          <SliderSetting bind:value={pullTime} label="Pull time" stepSize=1 SIGNIFICANT_FIGURES=2 max=60/>
          <SliderSetting bind:value={targetMass} label="Mass" stepSize=0.1 SIGNIFICANT_FIGURES=3 max=60/>
        </SettingsSection>
      </flexboxLayout>
    </carouselItem>
  </carousel>
</flexboxLayout>
<!-- </gridLayout> -->
  

<style lang="scss">
  #main {
    flex-direction: column;
    align-items: center;
    justify-content: space-between;
  }

  #carousel-item > * {
    flex-direction: column;
    margin: 32px;
  }

  #canvas-wrapper {
    border-color: #899878;
    border-width: 8px;
    border-radius: 64px;
    padding: 0px 16px;

    Canvas {
      width: 100%;
    }
  }
</style>