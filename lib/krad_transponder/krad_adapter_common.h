#ifndef KRAD_ADAPTER_COMMON_H
#define KRAD_ADAPTER_COMMON_H

typedef struct kr_adapter_info kr_adapter_info;
typedef struct kr_adapter_path_info kr_adapter_path_info;

#include "krad_jack_common.h"
#include "krad_wayland_common.h"
#include "krad_v4l2_common.h"
#include "krad_decklink_common.h"
#include "krad_x11_common.h"
#include "krad_alsa_common.h"

typedef enum {
  KR_ADP_PATH_INPUT = 1,
  KR_ADP_PATH_OUTPUT
} kr_adapter_path_direction;

typedef enum {
  KR_ADP_JACK,      /*   A  M  IO  */
  KR_ADP_WAYLAND,   /*  V   M   O  */
  KR_ADP_V4L2,      /*  V   S  I   */
  KR_ADP_DECKLINK,  /*  VA  M  I   */
  KR_ADP_X11,       /*  V   M  I   */
  KR_ADP_ALSA,      /*   A  M  IO  */
  KR_ADP_KRAD,      /*  VA  M  IO  */
  KR_ADP_ENCODER,   /*  VA  S  O   */
} kr_adapter_api;

typedef union {
  kr_jack_info jack;
  kr_wayland_info wayland;
  kr_v4l2_info v4l2;
  kr_decklink_info decklink;
  kr_x11_info x11;
  kr_alsa_info alsa;
  void *krad;
} kr_adapter_api_info;

typedef union {
  kr_jack_path_info jack;
  kr_wayland_path_info wayland;
  kr_v4l2_open_info v4l2;
  kr_decklink_path_info decklink;
  kr_x11_path_info x11;
  kr_alsa_path_info alsa;
} kr_adapter_api_path_info;

struct kr_adapter_info {
  kr_adapter_api api;
  kr_adapter_api_info api_info;
};

struct kr_adapter_path_info {
  char name[64];
  kr_adapter_path_direction dir;
  kr_adapter_api api;
  kr_adapter_api_path_info info;
};

#include "gen/krad_adapter_common_to_ebml.h"
#include "gen/krad_adapter_common_from_ebml.h"
#include "gen/krad_adapter_common_to_text.h"
#include "gen/krad_adapter_common_to_json.h"
#include "gen/krad_adapter_common_from_json.h"
#include "gen/krad_adapter_common_helpers.h"

#endif
