#ifndef BPAINT_BMP_H
#define BPAINT_BMP_H
#include <stdint.h>
#include <stdio.h>

int bpaint_write_to_disk__bmp(uint8_t *data, uint32_t width, uint32_t height,
                              FILE *fp);

int bpaint_write_to_buffer__bmp(uint8_t *data, uint32_t width, uint32_t height,
                              uint8_t* buffer);

#endif
