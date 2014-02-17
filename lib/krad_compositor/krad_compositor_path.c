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
  kr_compositor *compositor;
  kr_convert converter;
  kr_perspective *perspective;
  union {
    kr_graph_elem elem;
    void *ptr;
  } g;
};

typedef struct {
  kr_compositor_path_info *info;
  void *control_user;
  void *frame_user;
  kr_compositor_path *from;
  kr_compositor_path *to;
} kr_compositor_path_setup;

/*
static void path_release(kr_compositor *compositor, kr_compositor_path *path);
static float kr_round3(float f);
*/
static kr_vertex_type path_vertex_type(kr_compositor_path_type type);
static int path_info_check(kr_compositor_path_info *info);
static int path_setup_check(kr_compositor_path_setup *setup);
static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup);

#include "render.c"

/*
static void path_release(kr_compositor *compositor, kr_compositor_path *path) {
  if (path->perspective != NULL) {
    kr_perspective_destroy(&path->perspective);
  }
  kr_image_convert_clear(&path->converter);
  kr_graph_remove(path->compositor->graph, &path->g.elem);
  kr_pool_recycle(path->compositor->path_pool, path);
}

static float kr_round3(float f) {
  f = rintf(f * 1000.0);
  f = f / 1000.0;
  return f;
}
*/

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

static int path_info_check(kr_compositor_path_info *info) {
  if ((info->type != KR_COM_OVERLAY)
   && (info->type != KR_COM_SOURCE)
   && (info->type != KR_COM_INPUT)
   && (info->type != KR_COM_BUS)
   && (info->type != KR_COM_OUTPUT)) {
    return -4;
  }
  /*
  if ((info->width == 0) || (info->height == 0)) {
    return -1;
  }
  */
  return 0;
}

static int path_setup_check(kr_compositor_path_setup *setup) {
  kr_compositor_path_info *info;
  info = setup->info;
  if (setup->frame_user == NULL) {
    /* FIXME HRMMM */
  }
  if (setup->control_user == NULL) {
    /* FIXME HRMMM */
  }
  return  path_info_check(info);
}

static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup) {
  int ret;
  kr_vertex_type vertex_type;
  kr_compositor_event event;
  kr_compositor_path *path;
  if ((comp == NULL) || (setup == NULL)) return NULL;
  ret = path_setup_check(setup);
  if (ret) {
    printke("Compositor: path_setup_check failed: %d", ret);
    return NULL;
  }
  path = kr_pool_slice(comp->path_pool);
  if (path == NULL) {
    printke("Compositor: No free slices in pool");
    return NULL;
  }
  path->g.ptr = NULL;
  if (setup->info->type == KR_COM_INPUT) {
    path->g.elem = kr_graph_link(comp->graph, &setup->to->g.elem, &setup->from->g.elem, path);
  } else {
    vertex_type = path_vertex_type(setup->info->type);
    path->g.elem = kr_graph_vertex(comp->graph, vertex_type, path);
  }
  if (path->g.ptr == NULL) {
    kr_pool_recycle(comp->path_pool, path);
    return NULL;
  }
  path->compositor = comp;
  path->info = *setup->info;
  path->frame_user = setup->frame_user;
  path->control_user = setup->control_user;
  kr_image_convert_init(&path->converter);
  event.user = path->compositor->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_COM_CREATE;
  event.info = path->info;
  path->compositor->event_cb(&event);
  path->control_user = event.user_path;
  return path;
}

int kr_compositor_frame(kr_compositor_path *path, kr_image *image) {
  if ((path == NULL) || (image == NULL)) return -1;
  switch (path->info.type) {
    case KR_COM_SOURCE:
      printk("Compositor: frame submitted source port");
      break;
    case KR_COM_OUTPUT:
      printk("Compositor: frame requested from output port");
      break;
    default:
      printke("Compositor: frame submitted to incorrect path type");
      return -1;
  }
  return 0;
}

int kr_compositor_ctl(kr_compositor_path *path, kr_compositor_path_info_patch *patch) {
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

int kr_compositor_remove(kr_compositor_path *path) {
  if (path == NULL) return -1;
  /*FIXME*/
  /* do destroy event callback */
  return 0;
}

int kr_compositor_bus(kr_compositor *c, kr_compositor_path_info *i, void *user) {
  kr_compositor_path *path;
  kr_compositor_path_setup setup;
  if ((c == NULL) || (i == NULL) || (user == NULL)) return -1;
  setup.info = i;
  setup.control_user = user;
  setup.frame_user = NULL;
  path = path_create(c, &setup);
  if (path == NULL) return -2;
  return 0;
}

kr_compositor_path *kr_compositor_port(kr_compositor *com,
 kr_compositor_port_setup *port_setup) {
  kr_compositor_path_setup setup;
  if ((com == NULL) || (port_setup == NULL)) return NULL;
  setup.info = &port_setup->info;
  setup.control_user = port_setup->control_user;
  setup.frame_user = port_setup->frame_user;
  return path_create(com, &setup);
}

int kr_compositor_link(kr_compositor_path *to, kr_compositor_path *fr,
  kr_compositor_path_info *info, void *user) {
  kr_compositor_path_setup setup;
  kr_compositor_path *path;
  if ((fr == NULL) || (to == NULL) || (info == NULL) || (user == NULL)) return -1;
  if (fr->compositor != to->compositor) return -2;
  setup.info = info;
  setup.control_user = user;
  setup.frame_user = NULL;
  setup.from = fr;
  setup.to = to;
  path = path_create(to->compositor, &setup);
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
