#ifndef KRAD_COMPOSITOR_H
#define KRAD_COMPOSITOR_H

typedef struct kr_compositor kr_compositor;
typedef struct kr_compositor_setup kr_compositor_setup;
typedef struct kr_compositor_event kr_compositor_event;
typedef struct kr_compositor_control_easers kr_compositor_control_easers;
typedef void (kr_compositor_event_cb)(kr_compositor_event *);

#include "krad_easing.h"

struct kr_compositor_control_easers {
  kr_easer x;
  kr_easer y;
  kr_easer w;
  kr_easer h;
  kr_easer rotation;
  kr_easer opacity;
};

#include "krad_compositor_common.h"
#include "krad_compositor_path.h"

#define KR_COMPOSITOR_WIDTH_DEF 640
#define KR_COMPOSITOR_HEIGHT_DEF 360

#define DEFAULT_COMPOSITOR_BUFFER_FRAMES 4
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
  void *user;
  kr_compositor_event_cb *event_cb;
};

typedef enum {
  KR_COMP_CREATE = 1,
  KR_COMP_PATCH,
  KR_COMP_DESTROY
} kr_compositor_event_type;

struct kr_compositor_event {
  kr_compositor_path *path;
  void *user;
  void *user_path;
  kr_compositor_event_type type;
  kr_compositor_path_patch patch;
  kr_compositor_path_info info;
};

int kr_compositor_destroy(kr_compositor *compositor);
kr_compositor *kr_compositor_create(kr_compositor_setup *setup);

#endif
