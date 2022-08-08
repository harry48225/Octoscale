<script lang="ts">
  import { Canvas } from '@nativescript/canvas';

  type Point = {x: number, y: number}
  export const data: Point[] = [{x: 0, y: 0}, {x: 10, y: 10}, {x: 20, y: 30}, {x: 30, y: 35}, {x: 50, y: 38}, {x: 53, y: 39}];

  const drawFilledRegion = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: Point[]) => {
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

  const getDataScale = (canvasHeight: number, canvasWidth: number, filledProportionX: number = 0.8, filledProportionY: number = 0.8) => {
    const maxY = Math.max(...data.map(p => p.y));
    const maxX = Math.max(...data.map(p => p.x));

    const yScale = (canvasHeight/maxY) * filledProportionY;
    const xScale = (canvasWidth/maxX) * filledProportionX;

    return [xScale, yScale];
  }

  const drawGraphLine = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: Point[]) => {
    ctx.strokeStyle = '#899878';
    ctx.lineWidth = 8;

    ctx.beginPath();
    ctx.moveTo(data[0].x, data[0].y);

    for (let i = 1; i < data.length; i++) {
      ctx.lineTo(data[i].x, data[i].y);
    }
    ctx.stroke();
  };

  const drawGraphEndMarker = (canvas: Canvas, ctx: CanvasRenderingContext2D, data: Point[]) => {
    const lastPoint = data[data.length - 1];
    //ctx.moveTo(data[lastIndex].x, data[lastIndex].y);
    ctx.beginPath();
    ctx.arc(lastPoint.x, lastPoint.y, 16, 0, 2*Math.PI);
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

    const [xScale, yScale] = getDataScale(canvas.height, canvas.width);

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

    console.log('done');
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