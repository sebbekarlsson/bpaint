#include <bpaint/canvas.h>
#include <bpaint/macros.h>
#include <bpaint/shade.h>
#include <bpaint/shade_func.h>
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bpaint_shade_dispatch_init(BPaintShadeDispatch *dispatch,
                               BPaintShadeDispatchConfig cfg) {
  BPAINT_ASSERT_RETURN(dispatch != 0, 0);
  BPAINT_ASSERT_RETURN(cfg.n_x > 0, 0);
  BPAINT_ASSERT_RETURN(cfg.n_y > 0, 0);
  if (dispatch->initialized)
    return 1;

  dispatch->units = 0;
  dispatch->length = cfg.n_x * cfg.n_y;

  dispatch->config = cfg;
  dispatch->initialized = true;

  return 1;
}

int bpaint_shade_dispatch_destroy(BPaintShadeDispatch *dispatch) {
  BPAINT_ASSERT_RETURN(dispatch != 0, 0);
  BPAINT_ASSERT_RETURN(dispatch->initialized == true, 0);

  if (dispatch->units != 0) {
    free(dispatch->units);
  }

  dispatch->units = 0;
  dispatch->config = (BPaintShadeDispatchConfig){0};
  dispatch->length = 0;
  dispatch->initialized = false;

  return 0;
}

static void *bpaint_dispatch_unit_thread(void *ptr) {
  BPAINT_ASSERT_RETURN(ptr != 0, 0);
  BPaintDispatchUnit *unit = (BPaintDispatchUnit *)ptr;
  BPAINT_ASSERT_RETURN(unit->func != 0, 0);
  BPAINT_ASSERT_RETURN(unit->w > 0, 0);
  BPAINT_ASSERT_RETURN(unit->h > 0, 0);
  BPAINT_ASSERT_RETURN(unit->canvas.data != 0, 0);

  BPaintCanvas *canvas = &unit->canvas;

  BPV2 size = bpaint_canvas_get_size(*canvas);

  BPAINT_ASSERT_RETURN(size.x > 0.001f, 0);
  BPAINT_ASSERT_RETURN(size.y > 0.001f, 0);

  BPAINT_ASSERT_RETURN(unit->global_resolution.x > 0.001f, 0);
  BPAINT_ASSERT_RETURN(unit->global_resolution.y > 0.001f, 0);

  uint32_t w = unit->w;
  uint32_t h = unit->h;
  for (uint32_t x = 0; x < w; x++) {
    for (uint32_t y = 0; y < h; y++) {

      uint32_t rx = (unit->px * w) + x;
      uint32_t ry = (unit->py * h) + y;
      BPV2 fc = (BPV2){rx, ry};
      BPV4 pixel = unit->func(unit->id, fc, unit->global_resolution);
      if ((pixel.w * 255.0f) <= 0.00001f)
        continue;

      bpaint_canvas_set_pixel_absolute(canvas, rx, ry, pixel);
    }
  }

  pthread_exit(0);
  return 0;
}

static int bpaint_canvas_shade_(BPaintShadeDispatch *dispatch,
                                BPaintCanvas canvas,
                                BPaintCanvasShadeFunc func) {
  BPAINT_ASSERT_RETURN(dispatch != 0, 0);
  BPAINT_ASSERT_RETURN(dispatch->initialized == true, 0);
  BPAINT_ASSERT_RETURN(dispatch->length > 0, 0);
  BPAINT_ASSERT_RETURN(dispatch->config.n_x > 0, 0);
  BPAINT_ASSERT_RETURN(dispatch->config.n_y > 0, 0);
  BPAINT_ASSERT_RETURN(canvas.data != 0, 0);
  BPAINT_ASSERT_RETURN(canvas.config.width > 0, 0);
  BPAINT_ASSERT_RETURN(canvas.config.height > 0, 0);
  BPAINT_ASSERT_RETURN(func != 0, 0);

  if (dispatch->units == 0) {
    dispatch->units = (BPaintDispatchUnit *)calloc(dispatch->length,
                                                   sizeof(BPaintDispatchUnit));
  }
  BPAINT_ASSERT_RETURN(dispatch->units != 0, 0);

  uint32_t canvas_width = canvas.config.width;
  uint32_t canvas_height = canvas.config.height;

  uint32_t n_x = dispatch->config.n_x;
  uint32_t n_y = dispatch->config.n_y;

  uint32_t unit_w = canvas_width / n_x;
  uint32_t unit_h = canvas_height / n_y;

  uint32_t count = 0;

  for (uint32_t x = 0; x < n_x; x++) {
    for (uint32_t y = 0; y < n_y; y++) {
      uint32_t idx = x + y * n_x;
      BPaintDispatchUnit *unit = &dispatch->units[idx];
      unit->px = x;
      unit->py = y;
      unit->w = unit_w;
      unit->h = unit_h;
      unit->func = func;
      unit->canvas = canvas;
      unit->canvas.blend_mode = BPAINT_BLEND_MODE_ALPHA;
      unit->id = idx;
      count++;
    }
  }

  for (uint32_t i = 0; i < count; i++) {
    BPaintDispatchUnit *unit = &dispatch->units[i];
    unit->global_resolution = bpaint_canvas_get_size(canvas);
    unit->func = func;
    if (pthread_create(&unit->thread_id, 0, bpaint_dispatch_unit_thread,
                       unit)) {
      BPAINT_WARNING_RETURN(0, stderr, "Failed to create thread.\n");
    }
  }

  for (uint32_t i = 0; i < count; i++) {
    BPaintDispatchUnit unit = dispatch->units[i];
    pthread_join(unit.thread_id, 0);
  }

  return 1;
}

int bpaint_canvas_shade(BPaintCanvas canvas, BPaintShadeDispatchConfig cfg) {
  BPAINT_ASSERT_RETURN(canvas.data != 0, 0);
  BPAINT_ASSERT_RETURN(cfg.func != 0, 0);
  BPAINT_ASSERT_RETURN(cfg.n_x > 0, 0);
  BPAINT_ASSERT_RETURN(cfg.n_y > 0, 0);

  BPaintShadeDispatch dispatch = {0};
  if (!bpaint_shade_dispatch_init(
          &dispatch, (BPaintShadeDispatchConfig){.n_x = 2, .n_y = 2})) {
    BPAINT_WARNING_RETURN(0, stderr, "Failed to initialize dispatch group.\n");
  }

  if (!bpaint_canvas_shade_(&dispatch, canvas, cfg.func)) {
    BPAINT_WARNING_RETURN(0, stderr, "Failed to run shade procedure.\n");
  }

  bpaint_shade_dispatch_destroy(&dispatch);

  return 1;
}
