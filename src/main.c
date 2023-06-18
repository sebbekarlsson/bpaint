#include <bpaint/bpaint.h>

// Your custom "shader" function.
// This one draws a line using an SDF.
static BPV4 my_shade_func(uint32_t idx, BPV2 fc, BPV2 res) {
  BPV2 uv = (BPV2){fc.x / res.x, fc.y / res.y};
  BPV4 color = (BPV4){1.0f, 0, 0, 1.0f};

  BPV2 center = bpaint_v2_scale(res, 0.5f);
  center.x /= res.x;
  center.y /= res.y;

  float linesdf = bpaint_sdf_line(
      uv, center, bpaint_v2_add(center, (BPV2){0.25f, 0.0f}), 0.01f);

  color = bpaint_v4_scale(color, linesdf);

  return color;
}

int main(int argc, char *argv[]) {
  int w = 512;
  int h = 512;

  // create a new canvas.
  BPaintCanvas canvas = {0};
  bpaint_canvas_create(&canvas, (BPaintCanvasConfig){.width = w, .height = h});

  // fill canvas with some color.
  bpaint_canvas_fill(
    &canvas,
    (BPV4){.x = 0.5f, .y = 0.48f, .z = 0.48f, .w = 1.0f}
  );

  // "shade" the canvas using mutliple threads.
  bpaint_canvas_shade(canvas, (BPaintShadeDispatchConfig){
      
    .func = my_shade_func, // Your shade function
    
    .n_x = 2, // N threads in the X-axis
    
    .n_y = 2 // N threads in the Y-axis
  });

  // Write the bitmap to disk.
  FILE *fp = fopen("image.bmp", "wb+");
  bpaint_canvas_write_to_disk(canvas, fp, BPAINT_FORMAT_BMP);
  fclose(fp);

  // Destroy canvas.
  bpaint_canvas_destroy(&canvas);
  return 0;
}
