#include "krad_jack.h"
#include "krad_wayland.h"
#include "krad_v4l2.h"
#include "krad_decklink.h"
#include "krad_x11.h"
#include "krad_alsa.h"
#include "kr_aux.h"

#include "kr_adapter.h"

/* Must match the kr_xpdr_type enum */

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
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },{ /* KR_V4L2 */
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
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
