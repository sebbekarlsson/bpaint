#include <bpaint/utils.h>
#include <math.h>

float bpaint_clamp(float v, float min, float max) {
  return fminf(max, fmaxf(min, v));
}

float bpaint_float_buffer_fmaxf(float *values, uint32_t length) {
  if (!values || length <= 0)
    return 0;

  float maxv = 0.0f;

  for (uint32_t i = 0; i < length; i++) {
    maxv = fmaxf(maxv, values[i]);
  }

  return maxv;
}

float bpaint_float_buffer_fabsf_fmaxf(float *values, uint32_t length) {
  if (!values || length <= 0)
    return 0;

  float maxv = 0.0f;

  for (uint32_t i = 0; i < length; i++) {
    maxv = fmaxf(maxv, fabsf(values[i]));
  }

  return maxv;
}
