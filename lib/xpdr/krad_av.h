#ifndef KRAD_AV_H
#define KRAD_AV_H

#include <inttypes.h>

typedef struct kr_av_event kr_av_event;

typedef int (kr_av_release_cb)(void *ptr);
typedef int (kr_av_handler)(kr_av_event *);

typedef struct {
  uint8_t *px;
  uint8_t *ppx[4];
  int32_t pps[4];
  int32_t size[4];
  uint32_t w;
  uint32_t h;
  int32_t fmt;
  union {
    uint64_t tc;
    struct timeval ts;
  };
  void *owner;
  int32_t refs;
  kr_av_release_cb *release;
  kr_av_handler *ready;
} kr_image;

typedef struct {
  float *samples[8];
  uint32_t channels;
  int32_t count;
  int32_t rate;
  uint64_t stc;
  void *owner;
} kr_audio;

struct kr_av_event {
  union {
    kr_image *image;
    kr_audio *audio;
  };
  void *user;
};

int kr_image_ref(kr_image *image);
int kr_image_unref(kr_image *image);

#endif
