#ifndef KRAD_V4L2_COMMON_H
#define KRAD_V4L2_COMMON_H

typedef enum {
  KR_V4L2_VOID,
  KR_V4L2_OPEN,
  KR_V4L2_CAPTURE
} kr_v4l2_state;

typedef struct {
  int width;
  int height;
  int num;
  int den;
  int format;
} kr_v4l2_path_info;

typedef struct {
  int dev;
  int priority;
  kr_v4l2_state state;
} kr_v4l2_info;

#include "gen/krad_v4l2_common_to_text.h"
#include "gen/krad_v4l2_common_to_json.h"
#include "gen/krad_v4l2_common_from_json.h"
#include "gen/krad_v4l2_common_to_ebml.h"
#include "gen/krad_v4l2_common_from_ebml.h"
#include "gen/krad_v4l2_common_helpers.h"

#endif
