<script lang="ts">
  import { Canvas } from '@nativescript/canvas';
    import { onMount } from 'svelte';
  import { Writable } from 'svelte/store';
  import { GraphData } from '~/models/GraphData';

  export let dataStore: Writable<GraphData>;
  let lastMaxX = 0;
  let lastMaxY = 0;

  onMount(() => {
    dataStore.subscribe(val => update(val));
  })
  let canvas: Canvas | undefined;

  const drawFilledRegion = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: GraphData) => {
    ctx.fillStyle = '#E4E6C3';
    ctx.beginPath();
    ctx.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length - 1; i++) {
      ctx.lineTo(data[i].x, data[i].y);
    }

    ctx.lineTo(data[data.length-1].x, data[data.length-1].y);
    ctx.lineTo(data[data.length-1].x, data[0].y);
    ctx.closePath();
    ctx.fill();
  };

  const getDataScale = (data: GraphData, canvasHeight: number, canvasWidth: number, filledProportionX: number = 0.8, filledProportionY: number = 0.8) => {
    const maxY = Math.max(lastMaxY, ...data.map(p => p.y));
    const maxX = Math.max(lastMaxX, ...data.map(p => p.x));

    const yScale = (canvasHeight/maxY) * filledProportionY;
    const xScale = (canvasWidth/maxX) * filledProportionX;

    return [xScale, yScale];
  }

  const drawGraphLine = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: GraphData) => {
    ctx.strokeStyle = '#899878';
    ctx.lineWidth = 8;

    ctx.beginPath();
    ctx.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length; i++) {
      ctx.lineTo(data[i].x, data[i].y);
    }
    ctx.stroke();
  };

  const drawGraphEndMarker = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: GraphData) => {
    const lastPoint = data[data.length - 1];
    //ctx.moveTo(data[lastIndex].x, data[lastIndex].y);
    ctx.beginPath();
    ctx.arc(lastPoint.x, lastPoint.y, 16, 0, 2*Math.PI);
    ctx.fillStyle = '#899878';
    ctx.fill();
  };

  const canvasReady = (args: any) => {
    console.log('canvas ready');
    const potentialCanvas = args.object;
    if (potentialCanvas === undefined) return
    let ctx = potentialCanvas.getContext('2d');
    if (!(ctx instanceof CanvasRenderingContext2D)) return

    canvas = potentialCanvas;
    ctx.transform(1, 0, 0, -1, 0, potentialCanvas.height as number)
  }

  const update = (data: GraphData) => {
    if (canvas === undefined) return;
    let ctx = canvas.getContext('2d');
    if (!(ctx instanceof CanvasRenderingContext2D)) return

    if (data.length === 0) data = [{x: 0, y: 0}]; 

    const [xScale, yScale] = getDataScale(data, canvas.height as number, canvas.width as number);

    ctx.clearRect(0,0, canvas.width as number, canvas.height as number);
    const scaledData = data.map(p => ({x: p.x * xScale, y: p.y * yScale}));
    drawHorizontalTicks(canvas, ctx, xScale);
    drawFilledRegion(canvas, ctx, scaledData);
    drawGraphLine(canvas, ctx, scaledData);
    drawGraphEndMarker(canvas, ctx, scaledData);
  }

  const drawHorizontalTicks = (canvas: Canvas, ctx: CanvasRenderingContext2D, xScale: number) => {
    const tickWidth = 4;
    ctx.strokeStyle = '#899878';
    ctx.lineWidth = tickWidth;
    ctx.font = '60px Arial';
    for (let xCoordinate = tickWidth/2; xCoordinate < (canvas.width as number) - tickWidth/2; xCoordinate += 5*xScale) {
      ctx.beginPath();
      ctx.moveTo(xCoordinate, 0);
      ctx.lineTo(xCoordinate, (canvas.height as number) * 0.9);
      ctx.stroke();

      // if (xCoordinate < (canvas.width as number) - 200) {
      //   console.log(`drawing tick at ${xCoordinate}, ${Math.floor((xCoordinate - tickWidth / 2)/xScale)}s`);
      //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
      //   ctx.fillText(`s`, xCoordinate, 200);
      //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
      // }
    }
  }
</script>

<flexboxLayout>
  <canvas id="canvas" width="100%" height="100%" on:ready={canvasReady}/>
</flexboxLayout>

<style lang="scss">
  FlexboxLayout {
    border-color: #899878;
    border-width: 8px;
    border-radius: 64px;
    margin: 32px;
    overflow: hidden;
    padding-left: 64px;
    padding-right: 64px;
  }
</style>