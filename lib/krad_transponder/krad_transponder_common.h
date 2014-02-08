#ifndef KRAD_TRANSPONDER_COMMON_H
#define KRAD_TRANSPONDER_COMMON_H

#include "krad_jack_common.h"
#include "krad_wayland_common.h"
#include "krad_v4l2_common.h"
#include "krad_decklink_common.h"
#include "krad_x11_common.h"
#include "krad_alsa_common.h"

typedef struct kr_transponder_path_info kr_transponder_path_info;

typedef enum {
  KR_XR_JACK,
  KR_XR_JACK_INPUT,
  KR_XR_JACK_OUTPUT,
  KR_XR_WAYLAND,
  KR_XR_WAYLAND_OUTPUT,
  KR_XR_V4L2,
  KR_XR_V4L2_INPUT,
  KR_XR_DECKLINK,
  KR_XR_DECKLINK_INPUT,
  KR_XR_X11,
  KR_XR_X11_INPUT,
  KR_XR_ALSA,
  KR_XR_ALSA_INPUT,
  KR_XR_ALSA_OUTPUT,
  KR_XR_KRAD,
  KR_XR_KRAD_INPUT,
  KR_XR_KRAD_OUTPUT,
} kr_transponder_path_type;

typedef union {
  kr_jack_info jack;
  kr_jack_path_info jack_input;
  kr_jack_path_info jack_output;
  kr_wayland_info wayland;
  kr_wayland_path_info wayland_output;
  kr_v4l2_info v4l2;
  kr_v4l2_open_info v4l2_input;
  kr_decklink_info decklink;
  kr_decklink_path_info decklink_input;
  kr_x11_info x11;
  kr_x11_path_info x11_input;
  kr_alsa_info alsa;
  kr_alsa_path_info alsa_input;
  kr_alsa_path_info alsa_output;
  void *krad;
  void *krad_input;
  void *krad_output;
} kr_transponder_path_type_info;

struct kr_transponder_path_info {
  kr_transponder_path_type type;
  kr_transponder_path_type_info aio;
};

#include "gen/krad_transponder_common_to_ebml.h"
#include "gen/krad_transponder_common_from_ebml.h"
#include "gen/krad_transponder_common_helpers.h"
#include "gen/krad_transponder_common_to_text.h"
#include "gen/krad_transponder_common_to_json.h"
#include "gen/krad_transponder_common_from_json.h"

#endif
