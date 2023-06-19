#include <bpaint/alias.h>
#include <bpaint/hash.h>
#include <bpaint/noise.h>
#include <bpaint/utils.h>
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>

float bpaint_noise21(BPV2 v, float seed) {
  BPV2 id = bpaint_v2_floor(v);
  BPV2 lv = bpaint_v2_smoothstep(0.0f, 1.0f, bpaint_v2_fract(v));

  return MIX(MIX(HASH21(id, seed), HASH21(V2ADD(id, V2(1.0f, 0)), seed), lv.x),
             MIX(HASH21(V2ADD(id, V2(0, 1)), seed),
                 HASH21(V2ADD(id, V2(1.0f, 1.0f)), seed), lv.x),
             lv.y);
}
float bpaint_noise21_layered(BPV2 v, float seed, float freq, int octaves) {
  float div = 0.0f;
  float amp = 1.0f;
  float n = 0.0f;

  for (int i = 0; i < octaves; i++) {
    n += amp * bpaint_noise21(V2SCALE(v, freq), seed);
    div += amp;
    amp /= 2.0f;
    freq *= 2.0f;
  }

  return n / div;
}
BPV3 bpaint_noise23(BPV2 v, float seed) {
  BPV2 id = bpaint_v2_floor(v);
  BPV2 lv = bpaint_v2_smoothstep(0.0f, 1.0f, bpaint_v2_fract(v));

  return V3MIX(
      V3MIX(HASH23(id, seed), HASH23(V2ADD(id, V2(1.0f, 0)), seed), lv.x),
      V3MIX(HASH23(V2ADD(id, V2(0, 1)), seed),
            HASH23(V2ADD(id, V2(1.0f, 1.0f)), seed), lv.x),
      lv.y);
}

BPV3 bpaint_noise23_layered(BPV2 v, float seed, float freq, int octaves) {
  float div = 0.0f;
  float amp = 1.0f;
  BPV3 n = V3(0, 0, 0);

  for (int i = 0; i < octaves; i++) {
    n = V3ADD(n, V3SCALE(bpaint_noise23(V2SCALE(v, freq), seed), amp));
    div += amp;
    amp /= 2.0f;
    freq *= 2.0f;
  }

  return bpaint_v3_div(n, V3(div, div, div));
}

float bpaint_noise31(BPV3 v, float seed) {
  BPV3 id = bpaint_v3_floor(v);
  BPV3 lv = bpaint_v3_smoothstep(0.0f, 1.0f, bpaint_v3_fract(v));

  float a =
      MIX(MIX(HASH31(id, seed), HASH31(V3ADD(id, V3(1.0f, 0, 0)), seed), lv.x),
          MIX(HASH31(V3ADD(id, V3(0, 1, 0)), seed),
              HASH31(V3ADD(id, V3(1.0f, 1.0f, 0)), seed), lv.x),
          lv.y);

  float b = MIX(MIX(HASH31(V3ADD(id, V3(0, 0, 1)), seed),
                    HASH31(V3ADD(id, V3(1.0f, 0, 1)), seed), lv.x),
                MIX(HASH31(V3ADD(id, V3(0, 1, 1)), seed),
                    HASH31(V3ADD(id, V3(1.0f, 1.0f, 1)), seed), lv.x),
                lv.y);

  return MIX(a, b, lv.z);
}
BPV3 bpaint_noise33(BPV3 v, float seed) {
  BPV3 id = bpaint_v3_floor(v);
  BPV3 lv = bpaint_v3_smoothstep(0.0f, 1.0f, bpaint_v3_fract(v));

  BPV3 a = V3MIX(
      V3MIX(HASH33(id, seed), HASH33(V3ADD(id, V3(1.0f, 0, 0)), seed), lv.x),
      V3MIX(HASH33(V3ADD(id, V3(0, 1, 0)), seed),
            HASH33(V3ADD(id, V3(1.0f, 1.0f, 0)), seed), lv.x),
      lv.y);

  BPV3 b = V3MIX(V3MIX(HASH33(V3ADD(id, V3(0, 0, 1)), seed),
                       HASH33(V3ADD(id, V3(1.0f, 0, 1)), seed), lv.x),
                 V3MIX(HASH33(V3ADD(id, V3(0, 1, 1)), seed),
                       HASH33(V3ADD(id, V3(1.0f, 1.0f, 1)), seed), lv.x),
                 lv.y);

  return V3MIX(a, b, lv.z);
}
