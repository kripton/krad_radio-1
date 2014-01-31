#ifndef KRAD_V4L2_COMMON_H
#define KRAD_V4L2_COMMON_H

typedef enum {
  KR_V4L2_VOID,
  KR_V4L2_OPEN,
  KR_V4L2_CAPTURE
} kr_v4l2_state;

typedef struct kr_v4l2_mode kr_v4l2_mode;
typedef struct kr_v4l2_info kr_v4l2_info;
typedef struct kr_v4l2_open_info kr_v4l2_open_info;

#include "gen/krad_v4l2_common_to_text.h"
#include "gen/krad_v4l2_common_to_json.h"
#include "gen/krad_v4l2_common_from_json.h"
#include "gen/krad_v4l2_common_to_ebml.h"
#include "gen/krad_v4l2_common_from_ebml.h"
#include "gen/krad_v4l2_common_helpers.h"

struct kr_v4l2_mode {
  int width;
  int height;
  int num;
  int den;
  int format;
};

struct kr_v4l2_info {
  int dev;
  int priority;
  kr_v4l2_state state;
  kr_v4l2_mode mode;
};

struct kr_v4l2_open_info {
  int dev;
  int priority;
  kr_v4l2_mode mode;
};

#endif
