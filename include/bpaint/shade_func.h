#ifndef BPAINT_SHADE_FUNC_H
#define BPAINT_SHADE_FUNC_H
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>
#include <stdint.h>

typedef BPV4 (*BPaintCanvasShadeFunc)(uint32_t idx, BPV2 frag_coord,
                                      BPV2 resolution);
#endif
