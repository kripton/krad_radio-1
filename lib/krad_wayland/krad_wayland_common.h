#ifndef KRAD_WAYLAND_COMMON_H
#define KRAD_WAYLAND_COMMON_H

typedef enum {
  KR_WL_DISCONNECTED = 1,
  KR_WL_CONNECTED
} kr_wayland_state;

typedef struct {
  char display_name[128];
  kr_wayland_state state;
} kr_wayland_info;

typedef struct {
  int width;
  int height;
  int fullscreen;
} kr_wayland_path_info;

#include "gen/krad_wayland_common_to_ebml.h"
#include "gen/krad_wayland_common_from_ebml.h"
#include "gen/krad_wayland_common_to_text.h"
#include "gen/krad_wayland_common_to_json.h"
#include "gen/krad_wayland_common_from_json.h"
#include "gen/krad_wayland_common_helpers.h"

#endif
