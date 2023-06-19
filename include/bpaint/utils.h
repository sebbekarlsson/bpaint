#ifndef BPAINT_UTILS_H
#define BPAINT_UTILS_H
#include <stdint.h>
float bpaint_clamp(float v, float min, float max);
float bpaint_float_buffer_fmaxf(float *values, uint32_t length);
float bpaint_float_buffer_fabsf_fmaxf(float *values, uint32_t length);

uint32_t bpaint_float_bits_to_uint(float f);

float bpaint_smoothstep(float edge0, float edge1, float value);

float bpaint_fract(float x);

float bpaint_mix(float a, float b, float f);

float bpaint_radians(float a);
#endif
