#ifndef BPAINT_V3_H
#define BPAINT_V3_H

typedef float (*BPV3CallFunc)(float x);

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
BPV3 bpaint_v3_div(BPV3 a, BPV3 b);
BPV3 bpaint_v3_add_f(BPV3 a, float b);
BPV3 bpaint_v3_sub(BPV3 a, BPV3 b);
BPV3 bpaint_v3_sub_f(BPV3 a, float b);
BPV3 bpaint_v3_mul(BPV3 a, BPV3 b);
BPV3 bpaint_v3_clamp(BPV3 a, float min, float max);
BPV3 bpaint_v3_call(BPV3 a, BPV3CallFunc func);
BPV3 bpaint_v3_pow_f(BPV3 a, float f);
BPV3 bpaint_v3_smoothstep(float edge0, float edge1, BPV3 a);
BPV3 bpaint_v3_fract(BPV3 a);
BPV3 bpaint_v3_floor(BPV3 a);
BPV3 bpaint_v3_mix(BPV3 a, BPV3 b, float f);
BPV3 bpaint_v3_cross(BPV3 a, BPV3 b);
BPV3 bpaint_v3_rotate(BPV3 v, BPV3 axis, float angle);
BPV3 bpaint_v3_reflect(BPV3 I, BPV3 N);


#endif
