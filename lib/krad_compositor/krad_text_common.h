#ifndef KRAD_TEXT_COMMON_H
#define KRAD_TEXT_COMMON_H

typedef struct kr_text_info kr_text_info;

#include "krad_compositor_common.h"

struct kr_text_info {
  char string[512];
  char font[256];
  float red;
  float green;
  float blue;
};

#include "gen/krad_text_common_to_json.h"
#include "gen/krad_text_common_from_json.h"
#include "gen/krad_text_common_to_ebml.h"
#include "gen/krad_text_common_from_ebml.h"
#include "gen/krad_text_common_to_text.h"
#include "gen/krad_text_common_helpers.h"

#endif
