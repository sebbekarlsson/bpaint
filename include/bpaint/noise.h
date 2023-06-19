#ifndef BPAINT_NOISE_H
#define BPAINT_NOISE_H
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>

float bpaint_noise21(BPV2 v, float seed);
float bpaint_noise21_layered(BPV2 v, float seed, float freq, int octaves);
BPV3 bpaint_noise23(BPV2 v, float seed);
BPV3 bpaint_noise23_layered(BPV2 v, float seed, float freq, int octaves);

float bpaint_noise31(BPV3 v, float seed);
BPV3 bpaint_noise33(BPV3 v, float seed);

#endif
