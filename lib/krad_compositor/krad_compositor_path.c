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
  kr_frame_cb *frame_cb;
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
  kr_frame_cb *frame_cb;
  kr_compositor_path *from;
  kr_compositor_path *to;
} kr_compositor_path_setup;

static float kr_round3(float f);
static kr_vertex_type path_vertex_type(kr_compositor_path_type type);
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
  if ((comp == NULL) || (setup == NULL)) return NULL;
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
  path->frame_cb = setup->frame_cb;
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

static void path_release(kr_compositor *compositor, kr_compositor_path *path) {
  if (path->perspective != NULL) {
    kr_perspective_destroy(&path->perspective);
  }
  kr_image_convert_clear(&path->converter);
  kr_graph_remove(path->compositor->graph, &path->g.elem);
  kr_pool_recycle(path->compositor->path_pool, path);
}

static void path_output(kr_compositor_path *path, kr_image *image) {
  kr_frame_event event;
  event.user = path->frame_user;
  path->frame_cb(&event);
  memcpy(event.image.px, image->px, image->w * image->h * 4);
}

static int path_render(kr_compositor_path *path,
  kr_compositor_input_info *input_info, kr_image *dst, cairo_t *cr) {
  return 0;
}

static int path_info_check(kr_compositor_path_info *info) {
  if ((info->type != KR_COM_OVERLAY)
   && (info->type != KR_COM_INPUT)
   && (info->type != KR_COM_BUS)
   && (info->type != KR_COM_OUTPUT)) {
    return -4;
  }

  /* FIXME check info->info */

  return 0;
}

static int path_setup_check(kr_compositor_path_setup *setup) {
  kr_compositor_path_info *info;
  info = setup->info;
  if ((setup->frame_user == NULL) || (setup->frame_cb == NULL)) {
    /* FIXME HRMMM */
  }
  if (setup->control_user == NULL) {
    /* FIXME HRMMM */
  }
/*  if ((info->width == 0) || (info->height == 0)) {
    return -1;
  }*/
  if ((info->type != KR_COM_INPUT) && (info->type != KR_COM_OUTPUT)) {
    return -2;
  }
  /* FIXME check more things out */
  return 0;
}


static void output_source_render(kr_compositor_path *output, kr_compositor_path *source) {
  void ***user_chains;
  int n;
  int i;
  user_chains = alloca(sizeof(void**)*16);
  for (i = 0; i < 16; i++) {
    user_chains[i] = alloca(sizeof(void*)*16);
  }
  n = kr_graph_chains(output->compositor->graph, &output->g.elem, &source->g.elem, user_chains, 16, 16);
  for (i = 0; i < n; i++) {
    //transform = get_transform(user_chains, len);
    //render(output, source, transform);
  }
}

static void output_render(kr_compositor_path *output) {
  void *source_users[16];
  kr_compositor_path *source;
  int i;
  int n;
  n = kr_graph_sources(output->compositor->graph,
    &output->g.elem, source_users, 16);
  for (i = 0; i < n; i++) {
    source = (kr_compositor_path *)source_users[i];
    output_source_render(output, source);
  }
}

int kr_compositor_process(kr_compositor_path *path) {
  void *output_users[16];
  kr_compositor_path *output;
  int i;
  int n;
  if (path == NULL) return -1;
  if (path->info.type == KR_COM_OUTPUT) {
    //if (path->writeable) {
      output_render(path);
    //}
    return 0;
  } else {
    n = kr_graph_outputs(path->compositor->graph,
        &path->g.elem, output_users, 16);
    for (i = 0; i < n; i++) {
      output = (kr_compositor_path *)output_users[i];
      //if (output->writeable) {
      output_render(output);
      //}
    }
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
  setup.frame_cb = NULL;
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
  setup.frame_cb = port_setup->frame_cb;
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
  setup.frame_cb = NULL;
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
