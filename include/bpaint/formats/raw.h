#ifndef BPAINT_RAW_H
#define BPAINT_RAW_H
#include <bpaint/canvas.h>
#include <stdint.h>
#include <stdio.h>

int bpaint_write_to_disk__raw(BPaintCanvas canvas, FILE *fp);

#endif
