#include "krad_jack.h"
#include "krad_wayland.h"
#include "krad_v4l2.h"
#include "krad_decklink.h"
#include "krad_x11.h"
#include "krad_alsa.h"

#include "kr_adapter.h"

/* Must match the kr_xpdr_type enum */

static const kr_adapter_spec adapters[] = {
  { /* First is NULL */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_JACK */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_WAYLAND */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_V4L2 */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_DECKLINK */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_X11 */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_ALSA */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_AUX */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_ */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_ */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  },{ /* KR_ */
    .pctl = NULL,
    .rmpath = NULL,
    .mkpath = NULL,
    .ctl = NULL,
    .destroy = NULL,
    .create = NULL
  }
};
