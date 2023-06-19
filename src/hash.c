#include <bpaint/hash.h>
#include <bpaint/utils.h>
#include <stdlib.h>

#define BPAINT_HASH_NORMALIZE(V) (((float)V) / ((float)(0xFFFFFFFFU)))

uint32_t bpaint_hash_permute_uint32(uint32_t x) {
  uint32_t y = (~x * 403U);
  x ^= x << 17U;
  x ^= x >> 13U;
  x ^= x << 5U;
  x += (y * 1045U + x);
  x = x * 101353U;
  return x;
}

float bpaint_hash11(float ix, float seed) {
  uint32_t a = bpaint_float_bits_to_uint(ix);
  uint32_t b = bpaint_float_bits_to_uint(seed);
  uint32_t x = bpaint_hash_permute_uint32(a);
  uint32_t k = (~x) >> 3U;
  uint32_t y = bpaint_hash_permute_uint32(b + x);
  uint32_t w = (y ^ x) + (x * y + k); 
  return BPAINT_HASH_NORMALIZE(w);
}

float bpaint_hash21(BPV2 v, float seed) {
  uint32_t x = bpaint_float_bits_to_uint(v.x);
  uint32_t y = bpaint_float_bits_to_uint(v.y);
  uint32_t z = bpaint_float_bits_to_uint(seed);
  uint32_t w = bpaint_hash_permute_uint32((x + y * z));
  return BPAINT_HASH_NORMALIZE(w);
}

BPV2 bpaint_hash22(BPV2 v, float seed) {
  float a = bpaint_hash21(v, seed);
  float b = bpaint_hash21(v, seed+(seed * 52.29018f + a));
  return (BPV2){a, b};
}

BPV3 bpaint_hash23(BPV2 v, float seed) {
  BPV2 xy = bpaint_hash22(v, seed);
  float z = bpaint_hash21(v, seed+(seed * 52.29018f + xy.x * 44.82881 + xy.y));
  return (BPV3){ xy.x, xy.y, z };
}

float bpaint_hash31(BPV3 v, float seed) {
  uint32_t x = bpaint_float_bits_to_uint(v.x);
  uint32_t y = bpaint_float_bits_to_uint(v.y);
  uint32_t z = bpaint_float_bits_to_uint(v.z);
  uint32_t w = bpaint_float_bits_to_uint(seed);
  w = bpaint_hash_permute_uint32((x + w * y + z));
  return BPAINT_HASH_NORMALIZE(w);
}
BPV2 bpaint_hash32(BPV3 v, float seed) {
  float x = bpaint_hash31(v, seed);
  float y = bpaint_hash31(v, seed+(seed * 77.392981f + x));
  return (BPV2){x, y};
}
BPV3 bpaint_hash33(BPV3 v, float seed) {
  BPV2 xy = bpaint_hash32(v, seed);
  float z = bpaint_hash31(v, (seed * xy.x + 32.988181 * xy.y) + 3.29181f);
  return (BPV3){xy.x, xy.y, z};
}
