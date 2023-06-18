#ifndef BPAINT_SHADE_H
#define BPAINT_SHADE_H
#include <bpaint/canvas.h>
#include <bpaint/shade_func.h>
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
  uint32_t n_x;
  uint32_t n_y;
  BPaintCanvasShadeFunc func;
} BPaintShadeDispatchConfig;

typedef struct {
  BPV2 global_resolution;
  BPaintCanvas canvas;
  pthread_t thread_id;
  BPaintCanvasShadeFunc func;

  uint32_t px;
  uint32_t py;

  uint32_t w;
  uint32_t h;

  uint32_t id;
} BPaintDispatchUnit;

typedef struct BPAINT_SHADE_DISPATCH_STRUCT {
  BPaintShadeDispatchConfig config;
  BPaintDispatchUnit *units;
  uint32_t length;
  bool initialized;
} BPaintShadeDispatch;

int bpaint_shade_dispatch_init(BPaintShadeDispatch *dispatch,
                               BPaintShadeDispatchConfig cfg);
int bpaint_shade_dispatch_destroy(BPaintShadeDispatch *dispatch);
int bpaint_canvas_shade(BPaintCanvas canvas, BPaintShadeDispatchConfig cfg);
#endif
