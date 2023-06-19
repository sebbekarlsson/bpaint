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

uint32_t bpaint_float_bits_to_uint(float f) {
  union BPaintFloatBitsToUint {
    float f;
    uint32_t i;
  };

  union BPaintFloatBitsToUint converted;
  converted.f = f;
  return converted.i;
}

float bpaint_smoothstep(float edge0, float edge1, float value) {
  value = bpaint_clamp((value - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return value * value * (3.0f - 2.0 * value);
}

float bpaint_fract(float x) {
  float f = floorf(x);
  return x - f;
}

float bpaint_mix(float a, float b, float f) {
  return a + (b - a) * f;
}

float bpaint_radians(float a) {
  return a * M_PI / 180.0f;
}
