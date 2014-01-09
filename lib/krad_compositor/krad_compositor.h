#ifndef KRAD_COMPOSITOR_H
#define KRAD_COMPOSITOR_H

typedef struct kr_compositor kr_compositor;
typedef struct kr_compositor_setup kr_compositor_setup;
typedef struct kr_compositor_event kr_compositor_event;
typedef struct kr_compositor_control_easers kr_compositor_control_easers;
typedef void (kr_compositor_event_cb)(kr_compositor_event *);

#include "krad_easing.h"

struct kr_compositor_event {
  void *user;
  void *user_path;
  /* method
   * info struct */
};

struct kr_compositor_control_easers {
  kr_easer x;
  kr_easer y;
  kr_easer w;
  kr_easer h;
  kr_easer rotation;
  kr_easer opacity;
};

#include "krad_pool.h"
#include "krad_image_pool.h"
#include "krad_easing.h"
#include "krad_compositor_common.h"
#include "krad_compositor_path.h"
#include "krad_text.h"
#include "krad_sprite.h"
#include "krad_vector.h"

#define KR_COMPOSITOR_WIDTH_DEF 640
#define KR_COMPOSITOR_HEIGHT_DEF 360
#define KR_COMPOSITOR_FPS_DEF 30
#define KR_COMPOSITOR_FPS_NUM_DEF KR_COMPOSITOR_FPS_DEF * 1000
#define KR_COMPOSITOR_FPS_DEN_DEF 1 * 1000

#define DEFAULT_COMPOSITOR_BUFFER_FRAMES 20
#define KC_MAX_PORTS 32
#define KC_MAX_SPRITES 32
#define KC_MAX_TEXTS 32
#define KC_MAX_VECTORS 32
#define KC_MAX_ANY MAX(KC_MAX_PORTS, MAX(KC_MAX_SPRITES, MAX(KC_MAX_TEXTS, KC_MAX_VECTORS)))
#define KC_MAX_SUBUNITS KC_MAX_PORTS * KC_MAX_SPRITES * KC_MAX_TEXTS * KC_MAX_VECTORS

#define RED 0.244 / 0.255 * 1.0, 0.0 / 0.255 * 1.0, 0.0 / 0.255 * 1.0
#define GREY 0.197 / 0.255 * 1.0, 0.203 / 0.255 * 1.0, 0.203 / 0.255 * 1.0
#define BGCOLOR_CLR 0.0, 0.0, 0.0, 1.0

struct kr_compositor_setup {
  uint32_t width;
  uint32_t height;
  uint32_t fps_num;
  uint32_t fps_den;
  void *user;
  kr_compositor_event_cb *event_cb;
};

struct kr_compositor {
  cairo_t *cr;
  cairo_surface_t *cst;
  kr_image image;
  kr_image_pool *image_pool;
  kr_compositor_info info;
  kr_pool *sprite_pool;
  kr_pool *text_pool;
  kr_pool *vector_pool;
  kr_pool *path_pool;
  FT_Library ftlib;
};

int kr_compositor_destroy(kr_compositor *compositor);
kr_compositor *kr_compositor_create(kr_compositor_setup *setup);
void kr_compositor_setup_init(kr_compositor_setup *setup);
int kr_compositor_info_get(kr_compositor *com, kr_compositor_info *info);

/* Goes away sometime soon */
int kr_compositor_process(kr_compositor *compositor);

typedef struct {


} kr_compositor_setting;

int kr_compositor_ctl(kr_compositor *com, kr_compositor_setting *setting);

#endif
