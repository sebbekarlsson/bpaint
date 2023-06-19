#ifndef BPAINT_ALIAS_H
#define BPAINT_ALIAS_H

#define V2(X, Y) ((BPV2){.x = X, .y = Y})
#define V3(X, Y, Z) ((BPV3){.x = X, .y = Y, .z = Z})
#define V4(X, Y, Z, W) ((BPV4){.x = X, .y = Y, .z = Z, .w = W})

#define MIX(a, b, f) bpaint_mix(a, b, f)
#define HASH21(a, s) bpaint_hash21(a, s)
#define HASH23(a, s) bpaint_hash23(a, s)
#define HASH31(a, s) bpaint_hash31(a, s)
#define HASH33(a, s) bpaint_hash33(a, s)

#define V2MUL(a, b) bpaint_v2_mul(a, b)
#define V2ADD(a, b) bpaint_v2_add(a, b)
#define V2SUB(a, b) bpaint_v2_sub(a, b)
#define V2SCALE(a, b) bpaint_v2_scale(a, b)
#define V2MIX(a, b, f) bpaint_v2_mix(a, b, f)

#define V3MIX(a, b, f) bpaint_v3_mix(a, b, f)
#define V3MUL(a, b) bpaint_v3_mul(a, b)
#define V3ADD(a, b) bpaint_v3_add(a, b)
#define V3SUB(a, b) bpaint_v3_sub(a, b)
#define V3SCALE(a, b) bpaint_v3_scale(a, b)
#define V3DOT(a, b) bpaint_v3_dot(a, b)
#define V3ADOT(a, b) fabsf(bpaint_v3_dot(a, b))

#endif
