#ifndef BPAINT_HASH_H
#define BPAINT_HASH_H
#include <stdint.h>
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>

uint32_t bpaint_hash_permute_uint32(uint32_t x);

float bpaint_hash11(float x, float seed);

float bpaint_hash21(BPV2 v, float seed);
BPV2 bpaint_hash22(BPV2 v, float seed);
BPV3 bpaint_hash23(BPV2 v, float seed);

float bpaint_hash31(BPV3 v, float seed);
BPV2 bpaint_hash32(BPV3 v, float seed);
BPV3 bpaint_hash33(BPV3 v, float seed);
#endif
