#ifndef BPAINT_SDF_H
#define BPAINT_SDF_H
#include <bpaint/v2.h>
#include <bpaint/v3.h>
#include <bpaint/v4.h>

float bpaint_sdf_line(BPV2 p, BPV2 a, BPV2 b, float thick);

float bpaint_sdf_box(BPV3 p, BPV3 s);

#endif
