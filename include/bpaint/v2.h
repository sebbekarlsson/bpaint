#ifndef BPAINT_V2_H
#define BPAINT_V2_H
typedef struct {
  float x;
  float y;
} BPV2;

float bpaint_v2_mag(BPV2 a);
float bpaint_v2_dot(BPV2 a, BPV2 b);
BPV2 bpaint_v2_unit(BPV2 a);
BPV2 bpaint_v2_scale(BPV2 a, float s);
BPV2 bpaint_v2_add(BPV2 a, BPV2 b);
BPV2 bpaint_v2_sub(BPV2 a, BPV2 b);
BPV2 bpaint_v2_mul(BPV2 a, BPV2 b);
#endif
