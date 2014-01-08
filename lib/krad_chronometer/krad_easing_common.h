#ifndef KRAD_EASING_COMMON_H
#define KRAD_EASING_COMMON_H

typedef enum {
  LINEAR = 33999,
  EASEINSINE,
  EASEOUTSINE,
  EASEINOUTSINE,
  EASEINCUBIC,
  EASEOUTCUBIC,
  EASEINOUTCUBIC,
  EASEINOUTELASTIC,
/*  EASEINCIRC,
  EASEOUTCIRC,
  EASEINOUTCIRC,
  EASEINBACK,
  EASEOUTBACK,
  EASEINOUTBACK,
  EASEINBOUNCE,
  EASEOUTBOUNCE,
  EASEINOUTBOUNCE,
  EASEINEXPO,
  EASEOUTEXPO,
  EASEINOUTEXPO,
  EASEINELASTIC,
  EASEOUTELASTIC,
  EASEINOUTELASTIC,
  EASEINQUAD,
  EASEOUTQUAD,
  EASEINOUTQUAD,
  EASEINQUART,
  EASEOUTQUART,
  EASEINOUTQUART,
  EASEINQUINT,
  EASEOUTQUINT,
  EASEINOUTQUINT,
*/
} kr_easing;

#define FIRSTEASING LINEAR
#define LASTEASING EASEINOUTELASTIC

#include "gen/krad_easing_common_to_json.h"
#include "gen/krad_easing_common_from_json.h"
#include "gen/krad_easing_common_to_ebml.h"
#include "gen/krad_easing_common_from_ebml.h"
#include "gen/krad_easing_common_helpers.h"
#include "gen/krad_easing_common_to_text.h"

#endif
