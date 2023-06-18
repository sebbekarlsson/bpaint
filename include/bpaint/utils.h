#ifndef BPAINT_UTILS_H
#define BPAINT_UTILS_H
#include <stdint.h>
float bpaint_clamp(float v, float min, float max);
float bpaint_float_buffer_fmaxf(float *values, uint32_t length);
float bpaint_float_buffer_fabsf_fmaxf(float *values, uint32_t length);
#endif
