#ifndef KR_XPDR_COMMON_H
#define KR_XPDR_COMMON_H

#include "krad_jack_common.h"
#include "krad_wayland_common.h"
#include "krad_v4l2_common.h"
#include "krad_decklink_common.h"
#include "krad_x11_common.h"
#include "krad_alsa_common.h"

typedef struct kr_xpdr_path_info kr_xpdr_path_info;

typedef enum {
  KR_JACK,
  KR_JACK_IN,
  KR_JACK_OUT,
  KR_WAYLAND,
  KR_WAYLAND_OUT,
  KR_V4L2,
  KR_V4L2_IN,
  KR_DECKLINK,
  KR_DECKLINK_IN,
  KR_X11,
  KR_X11_IN,
  KR_ALSA,
  KR_ALSA_IN,
  KR_ALSA_OUT,
  KR_AUX,
  KR_AUX_IN,
  KR_AUX_OUT,
} kr_xpdr_type;

typedef union {
  kr_jack_info jack;
  kr_jack_path_info jack_in;
  kr_jack_path_info jack_out;
  kr_wayland_info wl;
  kr_wayland_path_info wl_out;
  kr_v4l2_info v4l2;
  kr_v4l2_open_info v4l2_in;
  kr_decklink_info dl;
  kr_decklink_path_info dl_in;
  kr_x11_info x11;
  kr_x11_path_info x11_in;
  kr_alsa_info alsa;
  kr_alsa_path_info alsa_in;
  kr_alsa_path_info alsa_out;
  void *krad;
  void *krad_input;
  void *krad_output;
} kr_xpdr_type_info;

struct kr_xpdr_path_info {
  kr_xpdr_type type;
  kr_xpdr_type_info adp;
};

#include "gen/kr_xpdr_common_to_ebml.h"
#include "gen/kr_xpdr_common_from_ebml.h"
#include "gen/kr_xpdr_common_helpers.h"
#include "gen/kr_xpdr_common_to_text.h"
#include "gen/kr_xpdr_common_to_json.h"
#include "gen/kr_xpdr_common_from_json.h"

#endif
