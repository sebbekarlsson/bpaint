#include <bpaint/canvas.h>
#include <bpaint/formats/bmp.h>
#include <bpaint/formats/format.h>
#include <bpaint/formats/raw.h>
#include <bpaint/macros.h>
#include <bpaint/sdf.h>
#include <bpaint/utils.h>
#include <bpaint/v2.h>
#include <bpaint/v4.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static uint32_t clamp_index(BPaintCanvas canvas, uint32_t idx) {
  uint32_t max = ((canvas.config.width) * (canvas.config.height) * canvas.config.channels) - 1;
  idx = MIN(idx, max);
  idx = MAX(idx, 0);
  return idx;
}

int bpaint_canvas_create(BPaintCanvas *canvas, BPaintCanvasConfig cfg) {
  if (canvas == 0 || (cfg.width <= 0 || cfg.height <= 0))
    return 0;
  cfg.channels = cfg.channels ? cfg.channels : 3;
  cfg.channels = MAX(cfg.channels, 3);

  canvas->config = cfg;

  uint32_t size = cfg.channels * cfg.width * cfg.height;

  uint8_t *data = (uint8_t *)calloc(size, sizeof(uint8_t));
  if (data == 0)
    return 0;
  canvas->data = data;
  canvas->blend_mode = BPAINT_BLEND_MODE_ALPHA;

  if (canvas->config.channels <= 3) {
    canvas->alpha = (float *)calloc(cfg.width * cfg.height, sizeof(float));
    BPAINT_ASSERT_RETURN(canvas->alpha != 0, 0);
  }

  canvas->size_bytes = size * sizeof(uint8_t);

  return 1;
}

int bpaint_canvas_destroy(BPaintCanvas *canvas) {
  if (!canvas)
    return 0;

  if (canvas->data != 0) {
    free(canvas->data);
  }

  if (canvas->alpha != 0) {
    free(canvas->alpha);
  }

  canvas->alpha = 0;
  canvas->data = 0;
  canvas->size_bytes = 0;
  canvas->config = (BPaintCanvasConfig){0};

  return 1;
}

int bpaint_canvas_slice(BPaintCanvas canvas, uint32_t px, uint32_t py,
                        uint32_t w, uint32_t h, BPaintCanvas *out) {
  if (canvas.data == 0 || w <= 0 || h <= 0 || out == 0)
    return 0;

  float *alpha = canvas.alpha;
  uint8_t *data = canvas.data;

  uint32_t total_size = w * h * canvas.config.channels;

  *out = (BPaintCanvas){
      .config = (BPaintCanvasConfig){.channels = canvas.config.channels,
                                     .width = w,
                                     .height = h,
                                     .ox = 0,
                                     .oy = 0},
      .alpha = alpha,
      .data = data,
      .size_bytes = total_size * sizeof(uint8_t),
      .blend_mode = canvas.blend_mode};

  return 1;
}

int bpaint_canvas_set_blend_mode(BPaintCanvas *canvas, BPaintBlendMode mode) {
  if (!canvas)
    return 0;
  canvas->blend_mode = mode;
  return 1;
}

uint32_t bpaint_canvas_get_alpha_index_absolute(BPaintCanvas canvas, uint32_t x,
                                                uint32_t y) {

  x += canvas.config.ox;
  y += canvas.config.oy;

  int ky = (int)canvas.config.height - (int)y;
  y = (uint32_t)ky;

  x = MAX(0, MIN(x, canvas.config.width - 1));
  y = MAX(0, MIN(y, canvas.config.height - 1));
  uint32_t idx = (x + y * canvas.config.width);
  uint32_t total_size = canvas.config.width * canvas.config.height;

  idx = MIN(idx, total_size - 1);
  idx = MAX(idx, 0);
  return idx;
}

uint32_t bpaint_canvas_get_pixel_index_absolute(BPaintCanvas canvas, uint32_t x,
                                                uint32_t y) {
  uint8_t channels = canvas.config.channels;

  x += canvas.config.ox;
  y += canvas.config.oy;

  int ky = (int)canvas.config.height - (int)y;
  y = (uint32_t)ky;

  x = MAX(0, MIN(x, canvas.config.width - 1));
  y = MAX(0, MIN(y, canvas.config.height - 1));
  uint32_t idx = (x + y * canvas.config.width) * channels;
  uint32_t total_size =
      canvas.config.width * canvas.config.height * canvas.config.channels;

  idx = MIN(idx, total_size - 1);
  idx = MAX(idx, 0);

  return idx;
}

uint32_t bpaint_canvas_get_pixel_index(BPaintCanvas canvas, BPV2 coords) {
  BPV2 size = bpaint_canvas_get_size(canvas);
  uint8_t channels = canvas.config.channels;

  coords.y = size.y - coords.y;
  float x = bpaint_clamp(coords.x + 0.0001f + canvas.config.ox, 0.0f, size.x);
  float y = bpaint_clamp(coords.y + 0.0001f + canvas.config.oy, 0.0f, size.y);
  uint32_t idx = (x + y * size.x) * channels;
  uint32_t total_size =
      canvas.config.width * canvas.config.height * canvas.config.channels;

  idx = MIN(idx, total_size - 1);
  idx = MAX(idx, 0);

  return idx;
}

uint32_t bpaint_canvas_get_alpha_index(BPaintCanvas canvas, BPV2 coords) {
  BPV2 size = bpaint_canvas_get_size(canvas);

  coords.x += canvas.config.ox;
  coords.y += canvas.config.oy;

  coords.y = size.y - coords.y;
  float x = bpaint_clamp(coords.x + 0.0001f, 0.0f, size.x);
  float y = bpaint_clamp(coords.y + 0.0001f, 0.0f, size.y);
  uint32_t idx = (x + y * size.x);
  uint32_t total_size = canvas.config.width * canvas.config.height;

  idx = MIN(idx, total_size - 1);
  idx = MAX(idx, 0);
  return idx;
}

static BPV4 blend_pixels_alpha(BPV4 a, BPV4 b) {
  BPV4 next = (BPV4){0, 0, 0, 0};

  a.w = fmaxf(0.0f, a.w - b.w);
  next = bpaint_v4_add(next, bpaint_v4_scale((BPV4){a.x, a.y, a.z, 1.0f}, a.w));
  next = bpaint_v4_add(next, bpaint_v4_scale((BPV4){b.x, b.y, b.z, 1.0f}, b.w));

  return next;
}

static BPV4 blend_pixels(BPV4 a, BPV4 b, BPaintBlendMode mode) {
  switch (mode) {
  case BPAINT_BLEND_MODE_REPLACE:
    return b;
    break;
  case BPAINT_BLEND_MODE_ADD:
    return bpaint_v4_add(a, b);
    break;
  case BPAINT_BLEND_MODE_MUL:
    return bpaint_v4_mul(a, b);
    break;
  default:
  case BPAINT_BLEND_MODE_ALPHA: {
    return blend_pixels_alpha(a, b);

  } break;
  }

  return blend_pixels_alpha(a, b);
}

int bpaint_canvas_set_alpha(BPaintCanvas *canvas, BPV2 coords, float alpha) {
  if (canvas == 0 || canvas->data == 0 || canvas->alpha == 0)
    return 0;

  uint32_t idx = bpaint_canvas_get_alpha_index(*canvas, coords);
  canvas->alpha[idx] = alpha;
  return 1;
}

int bpaint_canvas_set_alpha_absolute(BPaintCanvas *canvas, uint32_t x,
                                     uint32_t y, float alpha) {
  if (canvas == 0 || canvas->data == 0 || canvas->alpha == 0)
    return 0;

  uint32_t idx = bpaint_canvas_get_alpha_index_absolute(*canvas, x, y);
  canvas->alpha[idx] = alpha;
  return 1;
}

int bpaint_canvas_set_pixel(BPaintCanvas *canvas, BPV2 coords, BPV4 pixel) {

  if (canvas == 0)
    return 0;
  BPAINT_ASSERT_RETURN(canvas->data != 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.width > 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.height > 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.channels > 0, 0);

  uint32_t idx = bpaint_canvas_get_pixel_index(*canvas, coords);
  // uint32_t idx = (x * canvas->config.height + y) * canvas->config.channels;

  BPAINT_ASSERT_RETURN(idx < (canvas->config.width * canvas->config.height *
                              canvas->config.channels),
                       0);

  BPV4 current = bpaint_canvas_get_pixel(*canvas, coords);
  pixel = blend_pixels(current, pixel, canvas->blend_mode);

  if (canvas->config.channels <= 3) {
    canvas->data[clamp_index(*canvas, idx + 2)] = pixel.x * 255.0f;
    canvas->data[clamp_index(*canvas, idx + 1)] = pixel.y * 255.0f;
    canvas->data[clamp_index(*canvas, idx + 0)] = pixel.z * 255.0f;
    bpaint_canvas_set_alpha(canvas, coords, pixel.w);
  } else {
    canvas->data[clamp_index(*canvas, idx + 3)] = pixel.x * 255.0f;
    canvas->data[clamp_index(*canvas, idx + 2)] = pixel.y * 255.0f;
    canvas->data[clamp_index(*canvas, idx + 1)] = pixel.z * 255.0f;
    canvas->data[clamp_index(*canvas, idx + 0)] = pixel.w * 255.0f;
  }

  return 1;
}
int bpaint_canvas_set_pixel_absolute(BPaintCanvas *canvas, uint32_t x,
                                     uint32_t y, BPV4 pixel) {
  if (canvas == 0)
    return 0;
  BPAINT_ASSERT_RETURN(canvas->data != 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.width > 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.height > 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.channels > 0, 0);

  uint32_t idx = bpaint_canvas_get_pixel_index_absolute(*canvas, x, y);
  // uint32_t idx = (x * canvas->config.height + y) * canvas->config.channels;

  BPAINT_ASSERT_RETURN(idx < (canvas->config.width * canvas->config.height *
                              canvas->config.channels),
                       0);

  BPV4 current = bpaint_canvas_get_pixel_absolute(*canvas, x, y);
  pixel = blend_pixels(current, pixel, canvas->blend_mode);

  if (canvas->config.channels <= 3) {
    canvas->data[idx + 2] = pixel.x * 255.0f;
    canvas->data[idx + 1] = pixel.y * 255.0f;
    canvas->data[idx + 0] = pixel.z * 255.0f;
    bpaint_canvas_set_alpha_absolute(canvas, x, y, pixel.w);
  } else {
    canvas->data[idx + 3] = pixel.x * 255.0f;
    canvas->data[idx + 2] = pixel.y * 255.0f;
    canvas->data[idx + 1] = pixel.z * 255.0f;
    canvas->data[idx + 0] = pixel.w * 255.0f;
  }

  return 1;
}
BPV4 bpaint_canvas_get_pixel(BPaintCanvas canvas, BPV2 coords) {

  BPV4 pixel = {0};
  BPAINT_ASSERT_RETURN(canvas.data != 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.width > 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.height > 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.channels > 0, pixel);

  uint32_t idx = bpaint_canvas_get_pixel_index(canvas, coords);

  BPAINT_ASSERT_RETURN(idx < (canvas.config.width * canvas.config.height *
                              canvas.config.channels),
                       pixel);

  if (canvas.config.channels <= 3) {
    pixel.x = canvas.data[clamp_index(canvas, idx + 2)] / 255.0f;
    pixel.y = canvas.data[clamp_index(canvas, idx + 1)] / 255.0f;
    pixel.z = canvas.data[clamp_index(canvas, idx + 0)] / 255.0f;
    pixel.w = 1.0f;
  } else {
    pixel.x = canvas.data[clamp_index(canvas, idx + 3)] / 255.0f;
    pixel.y = canvas.data[clamp_index(canvas, idx + 2)] / 255.0f;
    pixel.z = canvas.data[clamp_index(canvas, idx + 1)] / 255.0f;
    pixel.w = canvas.data[clamp_index(canvas, idx + 0)] / 255.0f;
  }

  return pixel;
}

BPV4 bpaint_canvas_get_pixel_absolute(BPaintCanvas canvas, uint32_t x,
                                      uint32_t y) {
  BPV4 pixel = {0};
  BPAINT_ASSERT_RETURN(canvas.data != 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.width > 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.height > 0, pixel);
  BPAINT_ASSERT_RETURN(canvas.config.channels > 0, pixel);

  uint32_t idx = bpaint_canvas_get_pixel_index_absolute(canvas, x, y);

  BPAINT_ASSERT_RETURN(idx < (canvas.config.width * canvas.config.height *
                              canvas.config.channels),
                       pixel);

  if (canvas.config.channels <= 3) {
    pixel.x = canvas.data[idx + 2] / 255.0f;
    pixel.y = canvas.data[idx + 1] / 255.0f;
    pixel.z = canvas.data[idx + 0] / 255.0f;
    pixel.w = 1.0f;
  } else {
    pixel.x = canvas.data[idx + 3] / 255.0f;
    pixel.y = canvas.data[idx + 2] / 255.0f;
    pixel.z = canvas.data[idx + 1] / 255.0f;
    pixel.w = canvas.data[idx + 0] / 255.0f;
  }

  return pixel;
}

float bpaint_canvas_get_alpha(BPaintCanvas canvas, BPV2 coords) {
  if (canvas.alpha != 0) {
    uint32_t idx = bpaint_canvas_get_alpha_index(canvas, coords);
    return canvas.alpha[idx];
  }

  BPV4 pixel = bpaint_canvas_get_pixel(canvas, coords);

  return pixel.w;
}

float bpaint_canvas_get_alpha_absolute(BPaintCanvas canvas, uint32_t x,
                                       uint32_t y) {
  if (canvas.alpha != 0) {
    uint32_t idx = bpaint_canvas_get_alpha_index_absolute(canvas, x, y);
    return canvas.alpha[idx];
  }

  BPV4 pixel = bpaint_canvas_get_pixel_absolute(canvas, x, y);

  return pixel.w;
}

int bpaint_canvas_fill(BPaintCanvas *canvas, BPV4 pixel) {
  BPAINT_ASSERT_RETURN(canvas != 0, 0);
  BPAINT_ASSERT_RETURN(canvas->data != 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.width > 0, 0);
  BPAINT_ASSERT_RETURN(canvas->config.height > 0, 0);

  for (uint32_t x = 0; x < canvas->config.width; x++) {
    for (uint32_t y = 0; y < canvas->config.height; y++) {
      BPV2 coord = (BPV2){x, y};
      bpaint_canvas_set_pixel(canvas, coord, pixel);
    }
  }

  return 1;
}

int bpaint_canvas_line(BPaintCanvas *canvas, BPV2 a, BPV2 b, BPV4 color,
                       float thick) {

  BPAINT_ASSERT_RETURN(canvas != 0, 0);

  BPV2 size = bpaint_canvas_get_size(*canvas);

  a.x /= size.x;
  a.y /= size.y;

  b.x /= size.x;
  b.y /= size.y;

  for (uint32_t x = 0; x < canvas->config.width; x++) {
    for (uint32_t y = 0; y < canvas->config.height; y++) {
      BPV2 uv = bpaint_canvas_get_uv_at(*canvas, x, y);
      float sdf = bpaint_sdf_line(uv, a, b, thick);
      BPV4 col = bpaint_v4_scale(color, sdf);
      bpaint_canvas_set_pixel(canvas, bpaint_v2_mul(uv, size), col);
    }
  }

  return 1;
}

int bpaint_canvas_plot(BPaintCanvas *canvas, float *values, uint32_t length,
                       BPV4 color, float thick) {

  BPAINT_ASSERT_RETURN(canvas != 0, 0);
  BPAINT_ASSERT_RETURN(values != 0, 0);
  BPAINT_ASSERT_RETURN(length > 0, 0);

  BPV2 size = bpaint_canvas_get_size(*canvas);
  float peak = bpaint_float_buffer_fabsf_fmaxf(values, length);
  peak = fmaxf(peak, 0.00002f);
  float len = (float)length;
  float h = size.y * 0.96f;

  for (uint32_t ix = 0; ix < length; ix++) {
    float v = values[ix];
    float ux = (float)ix / len;
    float uy = v / peak;
    float x = ux * size.x;
    float y = uy * h;

    BPV2 start = (BPV2){x, 0};
    BPV2 end = (BPV2){x, y};
    bpaint_canvas_line(canvas, start, end, color, thick);
  }

  return 1;
}

BPV2 bpaint_canvas_get_size(BPaintCanvas canvas) {
  return (BPV2){fmaxf(1.0f, roundf((float)canvas.config.width)),
                fmaxf(1.0f, roundf((float)canvas.config.height))};
}
BPV2 bpaint_canvas_get_center(BPaintCanvas canvas) {
  BPV2 size = bpaint_canvas_get_size(canvas);
  return bpaint_v2_scale(size, 0.5f);
}
BPV2 bpaint_canvas_get_uv_at(BPaintCanvas canvas, uint32_t x, uint32_t y) {
  BPV2 size = bpaint_canvas_get_size(canvas);
  if (size.x <= 0.000001f || size.y <= 0.000001f)
    return (BPV2){0.0f, 0.0f};

  x += canvas.config.ox;
  y += canvas.config.oy;

  BPV2 coords = (BPV2){(float)x, (float)y};
  coords.x /= size.x;
  coords.y /= size.y;

  return coords;
}

int bpaint_canvas_write_to_disk(BPaintCanvas canvas, FILE *fp,
                                BPaintFormat format) {
  if (canvas.data == 0 || canvas.size_bytes <= 0 || fp == 0)
    return 0;

  switch (format) {
  case BPAINT_FORMAT_RAW:
    return bpaint_write_to_disk__raw(canvas, fp);
    break;
  case BPAINT_FORMAT_BMP: {
    if (canvas.config.channels != 3) {
      BPAINT_WARNING_RETURN(
          0, stderr, "Channels needs to be exactly 3 when writing as BMP.\n");
    }
    return bpaint_write_to_disk__bmp(canvas.data, canvas.config.width,
                                     canvas.config.height, fp);
  }; break;
  }

  return 1;
}
