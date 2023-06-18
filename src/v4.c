#include <bpaint/v4.h>
#include <math.h>

float bpaint_v4_mag(BPV4 a) {
  return sqrtf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2) + powf(a.w, 2.0f));
}
float bpaint_v4_dot(BPV4 a, BPV4 b) {
  float dot_x = a.x * b.x;
  float dot_y = a.y * b.y;
  float dot_z = a.z * b.z;
  float dot_w = a.w * b.w;

  return dot_x + dot_y + dot_z + dot_w;
}
BPV4 bpaint_v4_unit(BPV4 a) {
  float mag = bpaint_v4_mag(a);

  if (mag == 0.0 || (isinf(mag) || isnan(mag))) {
    return (BPV4){0, 0, 0};
  }

  return (BPV4){a.x / mag, a.y / mag, a.z / mag, a.w / mag};
}
BPV4 bpaint_v4_scale(BPV4 a, float s) {
  return (BPV4){a.x * s, a.y * s, a.z * s, a.w * s};
}
BPV4 bpaint_v4_add(BPV4 a, BPV4 b) {
  return (BPV4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
BPV4 bpaint_v4_sub(BPV4 a, BPV4 b) {
  return (BPV4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
BPV4 bpaint_v4_mul(BPV4 a, BPV4 b) {
  return (BPV4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}
