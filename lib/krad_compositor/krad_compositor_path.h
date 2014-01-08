#ifndef KRAD_COMPOSITOR_PATH_H
#define KRAD_COMPOSITOR_PATH_H

#include <cairo.h>

#include "krad_av.h"
#include "krad_convert.h"
#include "krad_perspective.h"
#include "krad_ring.h"

typedef enum {
  KR_CMP_NIL,
  KR_CMP_READY,
  KR_CMP_ACTIVE,
  KR_CMP_TERM
} compositor_path_state;

typedef struct kr_compositor_path kr_compositor_path;
typedef struct kr_compositor_io_path_setup kr_compositor_io_path_setup;
typedef struct kr_compositor_path_frame_cb_arg kr_compositor_path_frame_cb_arg;
typedef void (kr_compositor_path_frame_cb)(kr_compositor_path_frame_cb_arg *);

struct kr_compositor_path_frame_cb_arg {
  kr_image image;
  void *user;
};

struct kr_compositor_io_path_setup {
  kr_compositor_path_info info;
  void *user;
  kr_compositor_path_frame_cb *frame_cb;
};

/* internalish */
size_t kr_compositor_path_size();
void cmper_path_release(kr_compositor *compositor, kr_compositor_path *path);
int path_render(kr_compositor_path *path, kr_image *image, cairo_t *cr);
/* end internalish */

int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info);

int kr_compositor_unlink(kr_compositor_path *path);
kr_compositor_path *kr_compositor_mkio(kr_compositor *c, kr_compositor_io_path_setup *setup);
kr_compositor_path *kr_compositor_mkbus(kr_compositor *c, kr_compositor_path_info *i, void *p);
int kr_compositor_path_ctl(kr_compositor_path *path, kr_compositor_path_patch *patch);

#endif
