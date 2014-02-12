#include "krad_jack.h"
#include "krad_wayland.h"
#include "krad_v4l2.h"
#include "krad_decklink.h"
#include "krad_x11.h"
#include "krad_alsa.h"
#include "kr_aux.h"

#include "kr_adapter.h"

/* Must match the kr_xpdr_type enum */

typedef enum {
  KR_VIDEO_IN = 1,
  KR_VIDEO_OUT,
  KR_AUDIO_IN,
  KR_AUDIO_OUT
} kr_xpdr_link_mode;

static const kr_xpdr_link_mode xpdr_type_modes[] = {
  0,            /* 0 is 0 */
  0,            /* KR_JACK = 1, */
  0,            /* KR_WAYLAND, */
  0,            /* KR_V4L2, */
  0,            /* KR_DECKLINK, */
  0,            /* KR_X11, */
  0,            /* KR_ALSA, */
  0,            /* KR_AUX, */
  KR_AUDIO_IN,  /* KR_JACK_IN, */
  KR_AUDIO_OUT, /* KR_JACK_OUT, */
  KR_VIDEO_OUT, /* KR_WAYLAND_OUT, */
  KR_VIDEO_IN,  /* KR_V4L2_IN, */
  KR_VIDEO_IN,  /* KR_DECKLINK_VIDEO_IN, */
  KR_AUDIO_IN,  /* KR_DECKLINK_AUDIO_IN, */
  KR_VIDEO_IN,  /* KR_X11_IN, */
  KR_AUDIO_IN,  /* KR_ALSA_IN, */
  KR_AUDIO_OUT, /* KR_ALSA_OUT, */
  KR_VIDEO_IN,  /* KR_AUX_VIDEO_IN, */
  KR_VIDEO_OUT, /* KR_AUX_VIDEO_OUT, */
  KR_AUDIO_IN,  /* KR_AUX_AUDIO_IN, */
  KR_AUDIO_OUT  /* KR_AUX_AUDIO_OUT */
};

static const kr_adapter_spec adapters[] = {
  { /* First is NULL */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_JACK */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_WAYLAND */
    .lctl = kr_wl_lctl,
    .unlink = kr_wl_unlink,
    .link = kr_wl_link,
    .ctl = kr_wl_ctl,
    .close = kr_wl_close,
    .open = kr_wl_open
  },{ /* KR_V4L2 */
    .lctl = kr_v4l2_lctl,
    .unlink = kr_v4l2_unlink,
    .link = kr_v4l2_link,
    .ctl = kr_v4l2_ctl,
    .close = kr_v4l2_close,
    .open = kr_v4l2_open
  },{ /* KR_DECKLINK */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_X11 */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_ALSA */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_AUX */
    .lctl = kr_aux_lctl,
    .unlink = kr_aux_unlink,
    .link = kr_aux_link,
    .ctl = kr_aux_ctl,
    .close = kr_aux_close,
    .open = kr_aux_open
  },{ /* KR_ */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_ */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_ */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  }
};
