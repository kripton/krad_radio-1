#ifndef KRAD_COMPOSITOR_COMMON_H
#define KRAD_COMPOSITOR_COMMON_H

#define KR_COMPOSITOR_WIDTH 7680
#define KR_COMPOSITOR_HEIGHT 4320

#include "krad_system.h"
#include "krad_perspective_common.h"

typedef enum {
  KR_OVERLAY = 1,
  KR_COMP_INPUT,
  KR_COMP_BUS,
  KR_COMP_OUTPUT
} kr_compositor_path_type;

typedef enum {
  KR_COMP_PATH = 1,
  KR_SPRITE,
  KR_TEXT,
  KR_VECTOR
} kr_compositor_overlay_type;

typedef struct kr_rect kr_rect;
typedef struct kr_compositor_source_info kr_compositor_source_info;
typedef struct kr_compositor_input_info kr_compositor_input_info;
typedef struct kr_compositor_bus_info kr_compositor_bus_info;
typedef struct kr_compositor_output_info kr_compositor_output_info;
typedef struct kr_compositor_path_info kr_compositor_path_info;
typedef struct kr_overlay_info kr_compositor_overlay_info;

struct kr_rect {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
};

struct kr_compositor_output_info {
  int16_t w;
  int16_t h;
  float opacity;
};

struct kr_compositor_bus_info {
  float opacity;
};

struct kr_compositor_input_info {
  kr_rect crop;
  kr_rect pos;
  kr_perspective_view view;
  uint32_t z;
  float rotation;
  float opacity;
};

struct kr_compositor_source_info {
  int16_t w;
  int16_t h;
};

#include "krad_text_common.h"
#include "krad_vector_common.h"
#include "krad_sprite_common.h"

typedef union {
  kr_text_info text;
  kr_vector_info vector;
  kr_sprite_info sprite;
} kr_overlay_type_info;

struct kr_overlay_info {
  kr_compositor_overlay_type type;
  kr_overlay_type_info info;
};

typedef union {
  kr_compositor_output_info output_info;
  kr_compositor_bus_info bus_info;
  kr_compositor_input_info input_info;
  kr_compositor_source_info source_info;
  kr_compositor_overlay_info overlay_info;
} kr_compositor_path_type_info;

struct kr_compositor_path_info {
  kr_compositor_path_type type;
  kr_compositor_path_type_info info;
};

void kr_aspect_scale(int srcw, int srch, int dstw, int dsth, int *w, int *h);

#include "gen/krad_compositor_common_to_json.h"
#include "gen/krad_compositor_common_from_json.h"
#include "gen/krad_compositor_common_to_ebml.h"
#include "gen/krad_compositor_common_from_ebml.h"
#include "gen/krad_compositor_common_to_text.h"
#include "gen/krad_compositor_common_helpers.h"

#endif
