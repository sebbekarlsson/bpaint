#include <bpaint/tonemap.h>
#include <bpaint/macros.h>
#include <bpaint/v3.h>
#include <bpaint/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

BPV3 bpaint_tonemap(BPV3 c, BPaintTonemapType tone, float gamma) {
  if (gamma <= 0.000001f) gamma = 2.2f;
  
  switch (tone) {
  case BPAINT_TONEMAP_ACES: {
    return bpaint_tonemap_aces(bpaint_v3_pow_f(c, 1.0f / gamma));
  } break;
  case BPAINT_TONEMAP_UNREAL: return bpaint_tonemap_unreal(c); break;
  default: {
    BPAINT_WARNING_RETURN(c, stderr, "Invalid tonemap.\n");
  }; break;
  }

  // not reached
  BPAINT_ASSERT_PANIC(true == false);
}

BPV3 bpaint_tonemap_aces(BPV3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;

  BPV3 ax = bpaint_v3_scale(x, a);
  BPV3 cx = bpaint_v3_scale(x, c);
  
  return bpaint_v3_clamp(bpaint_v3_div(bpaint_v3_mul(x, bpaint_v3_add_f(ax, b)), bpaint_v3_add_f(bpaint_v3_mul(x, bpaint_v3_add_f(cx, d)), e)), 0.0, 1.0);
}
BPV3 bpaint_tonemap_unreal(BPV3 x) {
  return bpaint_v3_div(x, bpaint_v3_scale(bpaint_v3_add_f(x, 0.155f), 1.019f));
}
