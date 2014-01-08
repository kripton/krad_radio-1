#ifndef KRAD_COMPOSITOR_COMMON_H
#define KRAD_COMPOSITOR_COMMON_H

#define KRAD_COMPOSITOR_SUBUNIT_DEFAULT_TICKRATE 4

#include "krad_system.h"
#include "krad_perspective.h"

typedef enum {
  KR_CMP_OUTPUT = 666,
  KR_CMP_INPUT,
  KR_CMP_BUS
} kr_compositor_path_type;

typedef enum {
  KR_VIDEOPORT = 31,
  KR_SPRITE,
  KR_TEXT,
  KR_VECTOR
} kr_compositor_subunit_type;

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

typedef enum {
  KR_NO,
  KR_X,
  KR_Y,
  KR_Z,
  KR_WIDTH,
  KR_HEIGHT,
  KR_ROTATION,
  KR_OPACITY,
  KR_RED,
  KR_GREEN,
  KR_BLUE,
  KR_ALPHA,
  KR_TICKRATE,
  KR_CROP_X,
  KR_CROP_Y,
  KR_CROP_W,
  KR_CROP_H,
  KR_VIEW_TL_X,
  KR_VIEW_TL_Y,
  KR_VIEW_TR_X,
  KR_VIEW_TR_Y,
  KR_VIEW_BL_X,
  KR_VIEW_BL_Y,
  KR_VIEW_BR_X,
  KR_VIEW_BR_Y
} kr_compositor_control;

typedef struct kr_text_info kr_text_info;
typedef struct kr_sprite_info kr_sprite_info;
typedef struct kr_vector_info kr_vector_info;
typedef struct kr_compositor_path_info kr_compositor_path_info;
typedef struct kr_compositor_controls kr_compositor_controls;
typedef struct kr_compositor_info kr_compositor_info;

#include "gen/krad_compositor_common_to_json.h"
#include "gen/krad_compositor_common_from_json.h"
#include "gen/krad_compositor_common_to_ebml.h"
#include "gen/krad_compositor_common_from_ebml.h"
#include "gen/krad_compositor_common_to_text.h"
#include "gen/krad_compositor_common_helpers.h"

struct kr_compositor_controls {
  int32_t x;
  int32_t y;
  uint32_t z;
  uint32_t w;
  uint32_t h;
  float rotation;
  float opacity;
};

struct kr_sprite_info {
  char filename[256];
  int32_t rate;
  kr_compositor_controls controls;
};

struct kr_text_info {
  char string[512];
  char font[256];
  float red;
  float green;
  float blue;
  kr_compositor_controls controls;
};

struct kr_vector_info {
  kr_vector_type type;
  float red;
  float green;
  float blue;
  kr_compositor_controls controls;
};

struct kr_compositor_path_info {
  char name[128];
  kr_compositor_path_type type;
  /* Source / Dest res */
  uint32_t width;
  uint32_t height;
  uint32_t crop_x;
  uint32_t crop_y;
  uint32_t crop_width;
  uint32_t crop_height;
  kr_perspective_view view;
  kr_compositor_controls controls;
};

struct kr_compositor_info {
  uint32_t width; /* 640, 320, 1920 */
  uint32_t height; /* 360, 240, 1080 */
  uint32_t fps_numerator; /* 30, 20, 120 */
  uint32_t fps_denominator; /* 1, 1, 1 */
  uint32_t sprites; /* 0, 0, 128 */
  uint32_t vectors; /* 0, 0, 128 */
  uint32_t texts; /* 0, 0, 128 */
  uint32_t inputs; /* 0, 0, 32 */
  uint32_t outputs; /* 0, 0, 32 */
  uint64_t frames;
  uint64_t timecode; 
};

void kr_aspect_scale(int srcw, int srch, int dstw, int dsth, int *w, int *h);

#endif
