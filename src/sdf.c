#include <bpaint/sdf.h>
#include <bpaint/utils.h>
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
