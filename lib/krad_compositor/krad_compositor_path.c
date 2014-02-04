#include <cairo.h>

#include "krad_compositor_so_path.h"

typedef enum {
  PATH_UNUSED,
  PATH_READY,
  PATH_ACTIVE,
  PATH_DESTROYING
} compositor_path_state;

struct kr_compositor_path {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
  kr_compositor_path_frame_cb *frame_cb;
  kr_compositor *compositor;
  kr_convert converter;
  kr_perspective *perspective;
  union {
    kr_vertex *vertex;
    kr_edge *edge;
    void *ptr;
  } g;
};

struct kr_compositor_path_setup {
  kr_compositor_path_info *info;
  void *control_user;
  void *frane_user;
  kr_compositor_path_frame_cb *frame_cb;
  kr_compositor_path *from;
  kr_compositor_path *to;
};

static float kr_round3(float f);
static kr_graph_type path_vertex_type(kr_compositor_path_type type);
static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup);
static void path_release(kr_compositor *compositor, kr_compositor_path *path);
static void path_output(kr_compositor_path *path, kr_image *image);
static int path_render(kr_compositor_path *path,
  kr_compositor_input_info *input_info, kr_image *image, cairo_t *cr);
static int path_info_check(kr_compositor_path_info *info);
static int path_setup_check(kr_compositor_path_setup *setup);

static float kr_round3(float f) {
  f = rintf(f * 1000.0);
  f = f / 1000.0;
  return f;
}

static kr_vertex_type path_vertex_type(kr_compositor_path_type type) {
  switch (type) {
    case KR_COM_SOURCE:
      return KR_VERTEX_SOURCE;
    case KR_COM_OVERLAY:
      return KR_VERTEX_SOURCE;
    case KR_COM_BUS:
      return KR_VERTEX_BUS;
    case KR_COM_OUTPUT:
      return KR_VERTEX_OUTPUT;
    default:
      break;
  }
  return -1;
}

static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup) {
  int ret;
  kr_vertex_type vertex_type;
  kr_compositor_event event;
  kr_compositor_path *path;
  if ((path == NULL) || (setup == NULL)) return NULL;
  ret = path_setup_check(setup);
  if (ret) {
    printke("compositor mkpath failed setup check: %d", ret);
    return NULL;
  }
  path = kr_pool_slice(comp->path_pool);
  if (path == NULL) {
    printke("compositor mkpath could not slice new path");
    return NULL;
  }
  path->type = setup->info->type;
  path->compositor = compositor;
  path->info = setup->info;
  path->frame_user = setup->frame_user;
  path->control_user = setup->control_user;
  path->frame_cb = setup->frame_cb;
  kr_image_convert_init(&path->converter);
  event.user = path->compositor->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_COMP_CREATE;
  event.info = path->info;
  path->g.p = NULL;
  if (path->type == KR_COM_INPUT) {
    path->g.edge = kr_graph_edge_create(comp->graph, setup->to->g.vertex, setup->from->g.vertex, path);
  } else {
    vertex_type = path_vertex_type(setup->info->type);
    path->g.vertex = kr_graph_vertex_create(comp->graph, vertex_type, path);
  }
  if (path->g.p == NULL) {
    kr_slice_recycle(comp->path_pool, path);
    return NULL;
  }
  path->compositor->event_cb(&event);
  return path;
}

static void path_release(kr_compositor *compositor, kr_compositor_path *path) {
  if (path->perspective != NULL) {
    kr_perspective_destroy(&path->perspective);
  }
  kr_image_convert_clear(&path->converter);
  if (path->type == KR_COM_INPUT) {
    kr_graph_edge_destroy(path->compositor->graph, path->g.edge);
  } else {
    kr_graph_vertex_destroy(path->compositor->graph, path->g.vertex);
  }
  kr_pool_recycle(path->compositor->path_pool, path);
}

static void path_output(kr_compositor_path *path, kr_image *image) {
  kr_compositor_path_frame_cb_arg cb_arg;
  cb_arg.user = path->frame_user;
  path->frame_cb(&cb_arg);
  memcpy(cb_arg.image.px, image->px, image->w * image->h * 4);
}

static int path_render(kr_compositor_path *path,
  kr_compositor_input_info *input_info, kr_image *dst, cairo_t *cr) {
  return 0;
}

static int path_info_check(kr_compositor_path_info *info) {
  if ((info->type != KR_OVERLAY)
   && (info->type != KR_COMP_INPUT)
   && (info->type != KR_COMP_BUS)
   && (info->type != KR_COMP_OUTPUT)) {
    return -4;
  }

  /* FIXME check info->info */

  return 0;
}

static int path_setup_check(kr_compositor_path_setup *setup) {
  kr_compositor_path_info *info;
  info = &setup->info;
  if ((setup->frame_user == NULL) || (setup->frame_cb == NULL)) {
    /* FIXME HRMMM */
  }
  if (setup->control_user == NULL) {
    /* FIXME HRMMM */
  }
/*  if ((info->width == 0) || (info->height == 0)) {
    return -1;
  }*/
  if ((info->type != KR_COMP_INPUT) && (info->type != KR_COMP_OUTPUT)) {
    return -2;
  }
  /* FIXME check more things out */
  return 0;
}

int kr_compositor_process(kr_compositor_path *path) {
  if (path == NULL) return -1;

  return 0;
}

int kr_compositor_path_ctl(kr_compositor_path *path, kr_compositor_path_info_patch *patch) {
  int ret;
  if ((path == NULL) || (patch == NULL)) return -1;
  ret = kr_compositor_path_info_patch_apply(&path->info, patch);
/*  switch (s->control) {
    case KR_NO:
      return -1;
    case KR_X:
      break;
    case KR_Y:
      break;
    case KR_Z:
     break;
    case KR_WIDTH:
      break;
    case KR_HEIGHT:
      break;
    case KR_ROTATION:
      break;
    case KR_OPACITY:
      break;
    case KR_RED:
    case KR_GREEN:
    case KR_BLUE:
    case KR_ALPHA:
    case KR_CROP_X:
      break;
    case KR_CROP_Y:
      break;
    case KR_CROP_W:
      break;
    case KR_CROP_H:
      break;
    case KR_VIEW_TL_X:
      p->info.view.top_left.x = s->real;
      break;
    case KR_VIEW_TL_Y:
      p->info.view.top_left.y = s->real;
      break;
    case KR_VIEW_TR_X:
      p->info.view.top_right.x = s->real;
      break;
    case KR_VIEW_TR_Y:
      p->info.view.top_right.y = s->real;
      break;
    case KR_VIEW_BL_X:
      p->info.view.bottom_left.x = s->real;
      break;
    case KR_VIEW_BL_Y:
      p->info.view.bottom_left.y = s->real;
      break;
    case KR_VIEW_BR_X:
      p->info.view.bottom_right.x = s->real;
      break;
    case KR_VIEW_BR_Y:
      p->info.view.bottom_right.y = s->real;
      break;
    default:
      break;
  }*/
  return ret;
}

int kr_compositor_unlink(kr_compositor_path *path) {
  if (path == NULL) return -1;
  /*FIXME*/
  /* do destroy event callback */
  return 0;
}

int kr_compositor_mkbus(kr_compositor *c, kr_compositor_path_info *i, void *user) {
  kr_compositor_path *path;
  kr_compositor_path_setup setup;
  kr_compositor_path *path;
  if ((compositor == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = info;
  setup.control_user = user;
  setup.frame_user = NULL;
  setup.frame_cb = NULL;
  path = path_create(compositor, &setup);
  if (path == NULL) return -2;
  return 0;
}

kr_compositor_path *kr_compositor_mkso(kr_compositor *compositor,
 kr_compositor_io_path_setup *setup) {  kr_compositor_path_setup path_setup;
  if ((compositor == NULL) || (setup == NULL)) return NULL;
  path_setup.info = &setup->info;
  setup.control_user = setup->control_user;
  setup.frame_user = setup->frame_user;
  setup.frame_cb = setup->frame_cb;
  return path_create(compositor, &path_setup);
}

int kr_compositor_mkinput(kr_compositor_path *output, kr_compositor_path *from,
  kr_compositor_input_info *info, void *user) {
  kr_compositor_path_setup setup;
  kr_compositor_path *path;
  if ((compositor == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = info;
  setup.control_user = user;
  setup.frame_user = NULL;
  setup.frame_cb = NULL;
  setup.from = from;
  setup.to = to;
  path = path_create(compositor, &setup);
  if (path == NULL) return -2;
  return 0;
}

int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info) {
 if ((path == NULL) || (info == NULL)) return -1;
 *info = path->info;
 return 0;
}

size_t kr_compositor_path_size() {
  return sizeof(kr_compositor_path);
}
