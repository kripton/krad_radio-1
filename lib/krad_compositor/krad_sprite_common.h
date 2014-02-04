#ifndef KRAD_SPRITE_COMMON_H
#define KRAD_SPRITE_COMMON_H

typedef struct kr_sprite_info kr_sprite_info;

#include "krad_compositor_common.h"

struct kr_sprite_info {
  char filename[256];
  int32_t rate;
  int16_t w;
  int16_t h;
};

#include "gen/krad_sprite_common_to_json.h"
#include "gen/krad_sprite_common_from_json.h"
#include "gen/krad_sprite_common_to_ebml.h"
#include "gen/krad_sprite_common_from_ebml.h"
#include "gen/krad_sprite_common_to_text.h"
#include "gen/krad_sprite_common_helpers.h"

#endif
