#include <bpaint/v3.h>
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
BPV3 bpaint_v3_sub(BPV3 a, BPV3 b) {
  return (BPV3){a.x - b.x, a.y - b.y, a.z - b.z};
}
BPV3 bpaint_v3_mul(BPV3 a, BPV3 b) {
  return (BPV3){a.x * b.x, a.y * b.y, a.z * b.z};
}
