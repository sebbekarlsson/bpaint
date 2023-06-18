#ifndef BPAINT_CANVAS_H
#define BPAINT_CANVAS_H
#include <bpaint/formats/format.h>
#include <bpaint/shade_func.h>
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t width;
  uint32_t height;
  uint8_t channels;

  uint32_t ox;
  uint32_t oy;
} BPaintCanvasConfig;

typedef enum {
  BPAINT_BLEND_MODE_ALPHA,
  BPAINT_BLEND_MODE_REPLACE,
  BPAINT_BLEND_MODE_ADD,
  BPAINT_BLEND_MODE_MUL
} BPaintBlendMode;

typedef struct {
  BPaintCanvasConfig config;
  uint8_t *data;
  float *alpha;
  uint32_t size_bytes;
  BPaintBlendMode blend_mode;
  BPaintCanvasShadeFunc shade_func;
} BPaintCanvas;

int bpaint_canvas_create(BPaintCanvas *canvas, BPaintCanvasConfig cfg);
int bpaint_canvas_destroy(BPaintCanvas *canvas);

int bpaint_canvas_slice(BPaintCanvas canvas, uint32_t px, uint32_t py,
                        uint32_t w, uint32_t h, BPaintCanvas *out);

int bpaint_canvas_set_blend_mode(BPaintCanvas *canvas, BPaintBlendMode mode);

int bpaint_canvas_set_pixel(BPaintCanvas *canvas, BPV2 coords, BPV4 pixel);
int bpaint_canvas_set_pixel_absolute(BPaintCanvas *canvas, uint32_t x,
                                     uint32_t y, BPV4 pixel);
BPV4 bpaint_canvas_get_pixel(BPaintCanvas canvas, BPV2 coords);
BPV4 bpaint_canvas_get_pixel_absolute(BPaintCanvas canvas, uint32_t x,
                                      uint32_t y);
float bpaint_canvas_get_alpha(BPaintCanvas canvas, BPV2 coords);
float bpaint_canvas_get_alpha_absolute(BPaintCanvas canvas, uint32_t x,
                                       uint32_t y);
int bpaint_canvas_set_alpha(BPaintCanvas *canvas, BPV2 coords, float alpha);
int bpaint_canvas_set_alpha_absolute(BPaintCanvas *canvas, uint32_t x,
                                     uint32_t y, float alpha);
BPV2 bpaint_canvas_get_size(BPaintCanvas canvas);
BPV2 bpaint_canvas_get_center(BPaintCanvas canvas);
BPV2 bpaint_canvas_get_uv_at(BPaintCanvas canvas, uint32_t x, uint32_t y);
uint32_t bpaint_canvas_get_pixel_index(BPaintCanvas canvas, BPV2 coords);
uint32_t bpaint_canvas_get_alpha_index(BPaintCanvas canvas, BPV2 coords);

uint32_t bpaint_canvas_get_pixel_index_absolute(BPaintCanvas canvas, uint32_t x,
                                                uint32_t y);

uint32_t bpaint_canvas_get_alpha_index_absolute(BPaintCanvas canvas, uint32_t x,
                                                uint32_t y);

int bpaint_canvas_fill(BPaintCanvas *canvas, BPV4 pixel);

int bpaint_canvas_line(BPaintCanvas *canvas, BPV2 a, BPV2 b, BPV4 color,
                       float thick);

int bpaint_canvas_plot(BPaintCanvas *canvas, float *values, uint32_t length,
                       BPV4 color, float thick);

int bpaint_canvas_write_to_disk(BPaintCanvas canvas, FILE *fp,
                                BPaintFormat format);
#endif
