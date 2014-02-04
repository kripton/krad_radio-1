#ifndef KRAD_VECTOR_COMMON_H
#define KRAD_VECTOR_COMMON_H

typedef struct kr_vector_info kr_vector_info;

typedef enum {
  NOTHING,
  HEX,
  CIRCLE,
  RECT,
  TRIANGLE,
  VIPER,
  METER,
  GRID,
  CURVE,
  ARROW,
  CLOCK,
  SHADOW
} kr_vector_type;

#include "krad_compositor_common.h"

struct kr_vector_info {
  kr_vector_type type;
  float red;
  float green;
  float blue;
};

#include "gen/krad_vector_common_to_json.h"
#include "gen/krad_vector_common_from_json.h"
#include "gen/krad_vector_common_to_ebml.h"
#include "gen/krad_vector_common_from_ebml.h"
#include "gen/krad_vector_common_to_text.h"
#include "gen/krad_vector_common_helpers.h"

#endif
