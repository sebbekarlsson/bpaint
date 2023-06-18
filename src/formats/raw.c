#include <bpaint/canvas.h>
#include <bpaint/formats/raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bpaint_write_to_disk__raw(BPaintCanvas canvas, FILE *fp) {
  if (canvas.size_bytes <= 0 || canvas.data == 0 || canvas.config.width <= 0 ||
      canvas.config.height <= 0 || fp == 0)
    return 0;

  fwrite(&((uint8_t *)&canvas.config)[0], sizeof(BPaintCanvasConfig), 1, fp);
  fwrite(&canvas.size_bytes, sizeof(uint32_t), 1, fp);
  fwrite(&canvas.data[0], canvas.size_bytes, 1, fp);

  return 1;
}
