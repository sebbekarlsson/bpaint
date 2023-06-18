#ifndef BPAINT_V4_H
#define BPAINT_V4_H
typedef struct {
  float x;
  float y;
  float z;
  float w;
} BPV4;

float bpaint_v4_mag(BPV4 a);
float bpaint_v4_dot(BPV4 a, BPV4 b);
BPV4 bpaint_v4_unit(BPV4 a);
BPV4 bpaint_v4_scale(BPV4 a, float s);
BPV4 bpaint_v4_add(BPV4 a, BPV4 b);
BPV4 bpaint_v4_sub(BPV4 a, BPV4 b);
BPV4 bpaint_v4_mul(BPV4 a, BPV4 b);
#endif
