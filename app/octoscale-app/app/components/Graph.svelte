<script lang="ts">
  import { Canvas } from '@nativescript/canvas'

  type Point = {x: number, y: number}
  export const data: Point[] = [{x: 0, y: 0}, {x: 10, y: 10}, {x: 20, y: 30}, {x: 30, y: 35}];

  const drawFilledRegion = (canvas: Canvas, ctx: CanvasRenderingContext2D) => {
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

  const drawGraphLine = (canvas: Canvas, ctx: CanvasRenderingContext2D) => {
    ctx.strokeStyle = '#899878';

    ctx.beginPath();
    ctx.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length; i++) {
      ctx.lineTo(data[i].x, data[i].y);
    }
    ctx.stroke();
  };

  const drawGraphEndMarker = (canvas: Canvas, ctx: CanvasRenderingContext2D) => {
    const lastPoint = data[data.length - 1];
    //ctx.moveTo(data[lastIndex].x, data[lastIndex].y);
    ctx.beginPath();
    ctx.arc(lastPoint.x, lastPoint.y, 1, 0, 2*Math.PI);
    ctx.fillStyle = '#899878';
    ctx.fill();
  };

  const canvasReady = (args: any) => {
    console.log('canvas ready');
    const canvas = args.object;
    console.log(canvas);
    let ctx = canvas.getContext('2d');
    if (!(ctx instanceof CanvasRenderingContext2D)) return
    
    ctx.transform(1, 0, 0, -1, 0, canvas.height as number)
    ctx.scale(20, 20);

    drawFilledRegion(canvas, ctx);
    drawGraphLine(canvas, ctx);
    drawGraphEndMarker(canvas, ctx);
  }
</script>

<canvas id="canvas" width="100%" height="100%" on:ready={canvasReady}/>

<style>
</style>