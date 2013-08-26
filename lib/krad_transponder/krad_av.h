#ifndef KRAD_AV_H
#define KRAD_AV_H

#include <inttypes.h>

typedef struct kr_image kr_image;
typedef struct kr_audio kr_audio;

struct kr_image {
  uint8_t *px;
  uint8_t *ppx[4];
  int32_t pps[4];
  int32_t size[4];
  uint32_t w;
  uint32_t h;
  int32_t fmt;
  uint64_t tc;
};

struct kr_audio {
  float *samples[8];
  uint32_t channels;
  int32_t count;
  int32_t rate;
  uint64_t stc;
};

#endif
