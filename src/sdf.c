#include <bpaint/v3.h>
#include <bpaint/sdf.h>
#include <bpaint/utils.h>
#include <bpaint/alias.h>
#include <math.h>

float bpaint_sdf_line(BPV2 p, BPV2 a, BPV2 b, float thick) {
  BPV2 pa = bpaint_v2_sub(p, a);
  BPV2 ba = bpaint_v2_sub(b, a);
  float h =
      bpaint_clamp(bpaint_v2_dot(pa, ba) / bpaint_v2_dot(ba, ba), 0.00f, 1.0f);
  return fmaxf(
      0.0, 1.0f - (bpaint_v2_mag(bpaint_v2_sub(pa, bpaint_v2_scale(ba, h))) /
                   thick));
}


float bpaint_sdf_box(BPV3 p, BPV3 s) {
  p.x = fabsf(p.x) - s.x;
  p.y = fabsf(p.y) - s.y;
  p.z = fabsf(p.z) - s.z;

  BPV3 p2 = p;
  p2.x = fmaxf(0.0f, p.x);
  p2.y = fmaxf(0.0f, p.y);
  p2.z = fmaxf(0.0f, p.z);
  return bpaint_v3_mag(p2) + fminf(fmaxf(p.x, fmaxf(p.y, p.z)), 0.0f);
}
