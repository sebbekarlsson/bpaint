#include <bpaint/v2.h>
#include <bpaint/utils.h>
#include <math.h>

float bpaint_v2_mag(BPV2 a) { return sqrtf(powf(a.x, 2) + powf(a.y, 2)); }
float bpaint_v2_dot(BPV2 a, BPV2 b) {
  float dot_x = a.x * b.x;
  float dot_y = a.y * b.y;

  return dot_x + dot_y;
}
BPV2 bpaint_v2_unit(BPV2 a) {
  float mag = bpaint_v2_mag(a);

  if (mag == 0.0 || (isinf(mag) || isnan(mag))) {
    return (BPV2){0, 0};
  }

  return (BPV2){a.x / mag, a.y / mag};
}
BPV2 bpaint_v2_scale(BPV2 a, float s) { return (BPV2){a.x * s, a.y * s}; }
BPV2 bpaint_v2_add(BPV2 a, BPV2 b) { return (BPV2){a.x + b.x, a.y + b.y}; }
BPV2 bpaint_v2_sub(BPV2 a, BPV2 b) { return (BPV2){a.x - b.x, a.y - b.y}; }
BPV2 bpaint_v2_mul(BPV2 a, BPV2 b) { return (BPV2){a.x * b.x, a.y * b.y}; }

BPV2 bpaint_v2_smoothstep(float edge0, float edge1, BPV2 a) {
  a.x = bpaint_smoothstep(edge0, edge1, a.x);
  a.y = bpaint_smoothstep(edge0, edge1, a.y);
  return a;
}

BPV2 bpaint_v2_call(BPV2 a, BPV2CallFunc func) {
  return (BPV2){ func(a.x), func(a.y) };
}

BPV2 bpaint_v2_fract(BPV2 a) {
  return bpaint_v2_call(a, bpaint_fract);
}

BPV2 bpaint_v2_floor(BPV2 a) {
  return bpaint_v2_call(a, floorf);
}

BPV2 bpaint_v2_mix(BPV2 a, BPV2 b, float f) {
  a.x = bpaint_mix(a.x, b.x, f);
  a.y = bpaint_mix(a.y, b.y, f);
  return a;
}
