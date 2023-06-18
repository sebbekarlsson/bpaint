#ifndef BPAINT_V3_H
#define BPAINT_V3_H
typedef struct {
  float x;
  float y;
  float z;
} BPV3;

float bpaint_v3_mag(BPV3 a);
float bpaint_v3_dot(BPV3 a, BPV3 b);
BPV3 bpaint_v3_unit(BPV3 a);
BPV3 bpaint_v3_scale(BPV3 a, float s);
BPV3 bpaint_v3_add(BPV3 a, BPV3 b);
BPV3 bpaint_v3_sub(BPV3 a, BPV3 b);
BPV3 bpaint_v3_mul(BPV3 a, BPV3 b);
#endif
