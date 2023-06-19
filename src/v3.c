#include <bpaint/v3.h>
#include <bpaint/utils.h>
#include <math.h>

float bpaint_v3_mag(BPV3 a) {
  return sqrtf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2));
}
float bpaint_v3_dot(BPV3 a, BPV3 b) {
  float dot_x = a.x * b.x;
  float dot_y = a.y * b.y;
  float dot_z = a.z * b.z;

  return dot_x + dot_y + dot_z;
}
BPV3 bpaint_v3_unit(BPV3 a) {
  float mag = bpaint_v3_mag(a);

  if (mag == 0.0 || (isinf(mag) || isnan(mag))) {
    return (BPV3){0, 0, 0};
  }

  return (BPV3){a.x / mag, a.y / mag, a.z / mag};
}
BPV3 bpaint_v3_scale(BPV3 a, float s) {
  return (BPV3){a.x * s, a.y * s, a.z * s};
}
BPV3 bpaint_v3_add(BPV3 a, BPV3 b) {
  return (BPV3){a.x + b.x, a.y + b.y, a.z + b.z};
}
BPV3 bpaint_v3_div(BPV3 a, BPV3 b) {
  return (BPV3){a.x / b.x, a.y / b.y, a.z / b.z};
}
BPV3 bpaint_v3_add_f(BPV3 a, float s) {
  return (BPV3){a.x + s, a.y + s, a.z + s};
}

BPV3 bpaint_v3_sub_f(BPV3 a, float s) {
  return (BPV3){a.x - s, a.y - s, a.z - s};
}

BPV3 bpaint_v3_sub(BPV3 a, BPV3 b) {
  return (BPV3){a.x - b.x, a.y - b.y, a.z - b.z};
}
BPV3 bpaint_v3_mul(BPV3 a, BPV3 b) {
  return (BPV3){a.x * b.x, a.y * b.y, a.z * b.z};
}

BPV3 bpaint_v3_clamp(BPV3 a, float min, float max) {
  a.x = bpaint_clamp(a.x, min, max);
  a.y = bpaint_clamp(a.y, min, max);
  a.z = bpaint_clamp(a.z, min, max);

  return a;
}

BPV3 bpaint_v3_call(BPV3 a, BPV3CallFunc func) {
  return (BPV3){ func(a.x), func(a.y), func(a.z) };
}

BPV3 bpaint_v3_pow_f(BPV3 a, float f) {
  return (BPV3){ powf(a.x, f), powf(a.y, f), powf(a.z, f) };
}

BPV3 bpaint_v3_smoothstep(float edge0, float edge1, BPV3 a) {
  a.x = bpaint_smoothstep(edge0, edge1, a.x);
  a.y = bpaint_smoothstep(edge0, edge1, a.y);
  a.z = bpaint_smoothstep(edge0, edge1, a.z);
  return a;
}

BPV3 bpaint_v3_fract(BPV3 a) {
  return bpaint_v3_call(a, bpaint_fract);
}

BPV3 bpaint_v3_floor(BPV3 a) {
  return bpaint_v3_call(a, floorf);
}

BPV3 bpaint_v3_mix(BPV3 a, BPV3 b, float f) {
  a.x = bpaint_mix(a.x, b.x, f);
  a.y = bpaint_mix(a.y, b.y, f);
  a.z = bpaint_mix(a.z, b.z, f);
  return a;
}

BPV3 bpaint_v3_cross(BPV3 a, BPV3 b) {
 return (BPV3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

BPV3 bpaint_v3_rotate(BPV3 v, BPV3 axis, float angle) {
    float cos_theta = cosf(angle);
    float sin_theta = sinf(angle);
    float dot = bpaint_v3_dot(axis, v);
    BPV3 cross = bpaint_v3_cross(axis, v);

    return (BPV3){
        v.x * cos_theta + cross.x * sin_theta + axis.x * dot * (1 - cos_theta),
        v.y * cos_theta + cross.y * sin_theta + axis.y * dot * (1 - cos_theta),
        v.z * cos_theta + cross.z * sin_theta + axis.z * dot * (1 - cos_theta)
    };
}

BPV3 bpaint_v3_reflect(BPV3 I, BPV3 N) {
  return bpaint_v3_sub(I, bpaint_v3_mul(bpaint_v3_scale(N, 2.0f), bpaint_v3_mul(I, N)));
}
