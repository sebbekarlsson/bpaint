#ifndef BPAINT_TONEMAP_H
#define BPAINT_TONEMAP_H
#include <bpaint/v3.h>

typedef enum {
  BPAINT_TONEMAP_NONE,
  BPAINT_TONEMAP_ACES,
  BPAINT_TONEMAP_UNREAL
} BPaintTonemapType;


BPV3 bpaint_tonemap(BPV3 c, BPaintTonemapType tone, float gamma);
BPV3 bpaint_tonemap_aces(BPV3 c);
BPV3 bpaint_tonemap_unreal(BPV3 c);
#endif
