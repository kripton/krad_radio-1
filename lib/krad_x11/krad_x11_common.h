#ifndef KRAD_X11_COMMON_H
#define KRAD_X11_COMMON_H

typedef struct kr_x11_info kr_x11_info;
typedef struct kr_x11_path_info kr_x11_path_info;

#include "gen/krad_x11_common_to_ebml.h"
#include "gen/krad_x11_common_from_ebml.h"
#include "gen/krad_x11_common_to_text.h"
#include "gen/krad_x11_common_to_json.h"
#include "gen/krad_x11_common_from_json.h"
#include "gen/krad_x11_common_helpers.h"

struct kr_x11_info {
  char display[64];
};

struct kr_x11_path_info {
  char display[64];
  uint32_t width;
  uint32_t height;
  uint32_t num;
  uint32_t den;
  int x;
  int y;
};

#endif
