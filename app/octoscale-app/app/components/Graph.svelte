<script lang="ts">
  let ctx: CanvasRenderingContext2D;
  let canvas;

  const canvasReady = (args: any) => {
    console.log('canvas ready');
    canvas = args.object;
    console.log(canvas);
    ctx = canvas.getContext('2d');
    ctx.fillStyle = 'green';
    ctx.fillRect(10, 10, 150, 100);
  }
</script>

<canvas id="canvas" width="100%" height="100%" on:ready={canvasReady}/>

<style>
</style>