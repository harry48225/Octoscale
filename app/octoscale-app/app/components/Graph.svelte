<script lang="ts">
  import { Canvas, CanvasView, Cap, Join, Paint, Path, Style } from '@nativescript-community/ui-canvas';
  import { onMount } from 'svelte';
  import { Writable } from 'svelte/store';
  import { GraphData } from '~/models/GraphData';

  export let dataStore: Writable<GraphData>;

  let updating = false;

  onMount(() => {
    dataStore.subscribe(val => update(val));
  })

  let rawData: GraphData;
  
  let canvasView: CanvasView | undefined;

  const drawFilledRegion = (canvas: Canvas, data: GraphData) => {
    // ctx.fillStyle = '#E4E6C3';
    // ctx.beginPath();
    // ctx.moveTo(data[0].x, data[0].y);

    // for (let i = 1; i < data.length - 1; i++) {
    //   ctx.lineTo(data[i].x, data[i].y);
    // }

    // ctx.lineTo(data[data.length-1].x, data[data.length-1].y);
    // ctx.lineTo(data[data.length-1].x, data[0].y);
    // ctx.closePath();
    // ctx.fill();
  };

  const getDataScale = (canvasHeight: number, canvasWidth: number, filledProportionX: number = 0.9, filledProportionY: number = 0.9): number[] => {
    const maxY = Math.max(...rawData.map(p => p.y));
    const maxX = Math.max(...rawData.map(p => p.x));

    const yScale = (canvasHeight/maxY) * filledProportionY;
    const xScale = (canvasWidth/maxX) * filledProportionX;
    return [xScale, Math.floor(yScale)];
  }

  const drawGraphLine = (canvas: Canvas, data: GraphData) => {
    // ctx.strokeStyle = '#899878';
    // ctx.lineWidth = 8;

    const paint = new Paint();
    paint.setColor('#899878');
    paint.strokeWidth = 4;
    paint.setStyle(Style.STROKE);
    paint.setStrokeJoin(Join.ROUND);
    paint.setStrokeCap(Cap.BUTT);
    // ctx.beginPath();
    // ctx.moveTo(data[0].x, data[0].y);

    const path = new Path();
    
    path.moveTo(data[0].x, data[0].y);

    //canvas.drawLines(data.flatMap((p) => [p.x, p.y]), 0, data.length * 2, paint);

    for (let i = 1; i < data.length - 2; i+=3) {
      //ctx.lineTo(data[i].x, data[i].y);
      //canvas.drawLine(data[i-1].x, data[i-1].y, data[i].x, data[i].y, paint);
      path.cubicTo(data[i].x, data[i].y, data[i+1].x, data[i+1].y, data[i+2].x, data[i+2].y);
    }

    canvas.drawPath(path, paint);
    // ctx.stroke();
  };

  const drawGraphEndMarker = (canvas: Canvas, data: GraphData) => {
    // const lastPoint = data[data.length - 1];
    // //ctx.moveTo(data[lastIndex].x, data[lastIndex].y);
    // ctx.beginPath();
    // ctx.arc(lastPoint.x, lastPoint.y, 16, 0, 2*Math.PI);
    // ctx.fillStyle = '#899878';
    // ctx.fill();
  };

  const canvasReady = (args: any) => {
    console.log('canvas ready');
    // const potentialCanvas = args.object;
    // if (potentialCanvas === undefined) return
    // let ctx = potentialCanvas.getContext('2d');
    // if (!(ctx instanceof CanvasRenderingContext2D)) return

    // canvas = potentialCanvas;
    // ctx.transform(1, 0, 0, -1, 0, potentialCanvas.height as number)
  }

  const update = (newData: GraphData) => {
    if (updating) return
    //if (canvas === undefined) return;
    rawData = newData;
    updating = true;
    canvasView?.nativeView.invalidate();
    // let ctx = canvas.getContext('2d');
    // if (!(ctx instanceof CanvasRenderingContext2D)) return

    // ctx.setTransform(1, 0, 0, -1, 0, canvas.height as number);

    // if (data.length === 0) data = [{x: 0, y: 0}]; 

    // const [xScale, yScale] = getDataScale(data, canvas.height as number, canvas.width as number);
    // console.log(`${xScale}, ${yScale}`);
    // ctx.clearRect(0,0, canvas.width as number, canvas.height as number);
    // const scaledData = data.map(p => ({x: p.x * xScale, y: p.y * yScale}));
    // drawHorizontalTicks(canvas, ctx, xScale);
    // drawFilledRegion(canvas, ctx, scaledData);
    // drawGraphLine(canvas, ctx, scaledData);
    // drawGraphEndMarker(canvas, ctx, scaledData);
    updating = false;
  }

  const drawHorizontalTicks = (canvas: Canvas, ctx: CanvasRenderingContext2D, xScale: number) => {
    // const tickWidth = 4;
    // ctx.strokeStyle = '#899878';
    // ctx.lineWidth = tickWidth;
    // ctx.font = '60px Arial';
    // for (let xCoordinate = tickWidth/2; xCoordinate < (canvas.width as number) - tickWidth/2; xCoordinate += 5*xScale) {
    //   ctx.beginPath();
    //   ctx.moveTo(xCoordinate, 0);
    //   ctx.lineTo(xCoordinate, (canvas.height as number) * 0.9);
    //   ctx.stroke();

    //   // if (xCoordinate < (canvas.width as number) - 200) {
    //   //   console.log(`drawing tick at ${xCoordinate}, ${Math.floor((xCoordinate - tickWidth / 2)/xScale)}s`);
    //   //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
    //   //   ctx.fillText(`s`, xCoordinate, 200);
    //   //   ctx.transform(1, 0, 0, -1, 0, canvas.height as number);
    //   // }
    // }
  }

  const onDraw = (event: { canvas: Canvas}) => {
    const canvas = event.canvas;
    if (rawData.length < 3) return;
    const [xScale, yScale] = getDataScale(canvas.getHeight(), canvas.getWidth());
    const scaledData = rawData.map(p => ({x: p.x * xScale, y: p.y * yScale}));
    //event.canvas.scale(xScale, yScale);
    drawGraphLine(event.canvas, scaledData);
  }
</script>

<scrollView orientation="horizontal">
  <stackLayout orientation="horizontal">
    <flexboxLayout>
      <canvas id="canvas" bind:this={canvasView} on:draw={onDraw}/>
    </flexboxLayout>
    <stackLayout>
      <label>hi</label>
    </stackLayout>
  </stackLayout>
</scrollView>


<style lang="scss">
  ScrollView {
    flex: auto;
    margin: 32px;
    overflow: hidden;
    padding-left: 64px;
    padding-right: 64px;
    
    StackLayout {
      width: 200%;

      FlexboxLayout {
        border-color: #899878;
        border-width: 8px;
        border-radius: 64px;
        width: 50%;

        Canvas {
          width: 100%;
        }
      }
    }
  }
</style>