#include <bpaint/formats/bmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bpaint_write_to_disk__bmp(uint8_t *data, uint32_t width, uint32_t height,
                              FILE *fp) {
  if (data == 0 || width <= 0 || height <= 0 || fp == 0)
    return 0;

  unsigned char fileHeader[14] = {'B', 'M', 0, 0,  0, 0, 0,
                                  0,   0,   0, 54, 0, 0, 0};
  unsigned char imageHeader[40] = {40, 0, 0, 0, 0, 0, 0,  0,
                                   0,  0, 0, 0, 1, 0, 24, 0};
  unsigned char bmppad[3] = {0, 0, 0};

  uint32_t filesize = 54 + 3 * width * height;

  fileHeader[2] = (unsigned char)(filesize);
  fileHeader[3] = (unsigned char)(filesize >> 8);
  fileHeader[4] = (unsigned char)(filesize >> 16);
  fileHeader[5] = (unsigned char)(filesize >> 24);

  imageHeader[4] = (unsigned char)(width);
  imageHeader[5] = (unsigned char)(width >> 8);
  imageHeader[6] = (unsigned char)(width >> 16);
  imageHeader[7] = (unsigned char)(width >> 24);
  imageHeader[8] = (unsigned char)(height);
  imageHeader[9] = (unsigned char)(height >> 8);
  imageHeader[10] = (unsigned char)(height >> 16);
  imageHeader[11] = (unsigned char)(height >> 24);

  fwrite(fileHeader, 1, 14, fp);
  fwrite(imageHeader, 1, 40, fp);

  for (int i = 0; i < height; i++) {
    fwrite(data + (width * (height - i - 1) * 3), 3, width, fp);
    fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, fp);
  }

  return 1;
}
