#include "krad_pool.h"
#include "krad_image_pool.h"
#include "krad_easing.h"
#include "krad_text.h"
#include "krad_sprite.h"
#include "krad_vector.h"
#include "krad_graph.h"

#include "krad_compositor.h"

struct kr_compositor {
  cairo_t *cr;
  cairo_surface_t *cst;
  kr_image image;
  kr_image_pool *image_pool;
  kr_pool *sprite_pool;
  kr_pool *text_pool;
  kr_pool *vector_pool;
  kr_pool *path_pool;
  void *user;
  kr_compositor_event_cb *event_cb;
  FT_Library ftlib;
  kr_graph *graph;
};

#include "krad_compositor_path.c"

static void setup(kr_compositor *compositor);
static void composite(kr_compositor *compositor);
static void output(kr_compositor *compositor);
static void cleanup(kr_compositor *compositor);

static void subunits_free(kr_compositor *compositor);
static void subunits_create(kr_compositor *compositor);
static void subunits_state_update(kr_compositor *compositor);

static void kr_compositor_clear_frame(kr_compositor *compositor) {
  cairo_save(compositor->cr);
  cairo_set_source_rgba(compositor->cr, BGCOLOR_CLR);
  cairo_set_operator(compositor->cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(compositor->cr);
  cairo_restore(compositor->cr);
}

static void setup(kr_compositor *compositor) {
  while (!kr_image_pool_getimage(compositor->image_pool, &compositor->image)) {
    printke("Compositor wanted a frame but could not get one right away!");
    usleep(5000);
  }
  compositor->cst = cairo_image_surface_create_for_data(compositor->image.px,
   CAIRO_FORMAT_ARGB32, compositor->image.w, compositor->image.h, compositor->image.pps[0]);
  compositor->cr = cairo_create(compositor->cst);
  cairo_surface_destroy(compositor->cst);
}

static void composite(kr_compositor *com) {
  int i;
  kr_compositor_path *path;
  void *overlay;
  i = 0;
  kr_compositor_clear_frame(com);
  while ((path = kr_pool_iterate_active(com->path_pool, &i))) {
    if (path_type_get(path) == KR_CMP_INPUT) {
      path_render(path, &com->image, com->cr);
    }
  }
  while ((overlay = kr_pool_iterate_active(com->sprite_pool, &i))) {
    kr_sprite_render(overlay, com->cr);
  }
  while ((overlay = kr_pool_iterate_active(com->text_pool, &i))) {
    kr_text_render(overlay, com->cr);
  }
  while ((overlay = kr_pool_iterate_active(com->vector_pool, &i))) {
    kr_vector_render(overlay, com->cr);
  }
}

static void output(kr_compositor *compositor) {

  kr_compositor_path *path;
  int i;

  i = 0;

  while ((path = kr_pool_iterate_active(compositor->path_pool, &i))) {
    if (path->info.type == KR_CMP_OUTPUT) {
      path_output(path, &compositor->image);
    }
  }
}

static void cleanup(kr_compositor *compositor) {
  kr_pool_recycle(compositor->image_pool, compositor->image.px);
  cairo_destroy(compositor->cr);
  subunits_state_update(compositor);
}

/*
int kr_compositor_process(kr_compositor *compositor) {
  if (compositor == NULL) return -1;
  setup(compositor);
  composite(compositor);
  output(compositor);
  cleanup(compositor);
  return 0;
}
*/
static void subunits_state_update(kr_compositor *compositor) {
  /*
  int i;
  for (i = 0; i < KC_MAX_PORTS; i++) {
    if (compositor->path[i].subunit.active == 2) {
        cmper_path_release(compositor, &compositor->path[i]);
        krad_compositor_subunit_reset(&compositor->path[i].subunit);
        compositor->path[i].subunit.active = 0;
    }
  }
  for (i = 0; i < KC_MAX_SPRITES; i++) {
    if (compositor->sprite[i].subunit.active == 2) {
      kr_sprite_clear(&compositor->sprite[i]);
      compositor->active_sprites--;
      compositor->sprite[i].subunit.active = 0;
    }
  }
  for (i = 0; i < KC_MAX_TEXTS; i++) {
    if (compositor->text[i].subunit.active == 2) {
      kr_text_reset(&compositor->text[i]);
      compositor->active_texts--;
      compositor->text[i].subunit.active = 0;
    }
  }
  for (i = 0; i < KC_MAX_VECTORS; i++) {
    if (compositor->vector[i].subunit.active == 2) {
      kr_vector_reset(&compositor->vector[i]);
      compositor->active_vectors--;
      compositor->vector[i].subunit.active = 0;
    }
  }
  */
}

static void subunits_free(kr_compositor *com) {
  int i;
  i = 0;
  void *overlay;
  while ((overlay = kr_pool_iterate_active(com->sprite_pool, &i))) {
    kr_sprite_clear(overlay);
    kr_pool_recycle(com->sprite_pool, overlay);
  }
  while ((overlay = kr_pool_iterate_active(com->text_pool, &i))) {
  }
  while ((overlay = kr_pool_iterate_active(com->vector_pool, &i))) {
  }
  kr_pool_destroy(com->sprite_pool);
  kr_pool_destroy(com->text_pool);
  kr_pool_destroy(com->vector_pool);
}

static void subunits_create(kr_compositor *compositor) {
  kr_pool_setup setup;
  setup.shared = 0;
  setup.overlay_sz = 0;
  setup.size = kr_sprite_size();
  setup.slices = KC_MAX_SPRITES;
  compositor->sprite_pool = kr_pool_create(&setup);
  setup.size = kr_text_size();
  setup.slices = KC_MAX_TEXTS;
  compositor->text_pool = kr_pool_create(&setup);
  setup.size = kr_vector_size();
  setup.slices = KC_MAX_VECTORS;
  compositor->vector_pool = kr_pool_create(&setup);
}

int kr_compositor_destroy(kr_compositor *com) {
  if (com == NULL) return -1;
  printk("Compositor: Destroying");
  subunits_free(com);
  FT_Done_FreeType(com->ftlib);
  kr_graph_destroy(com->graph);
  kr_pool_destroy(com->image_pool);
  kr_pool_destroy(com->path_pool);
  printk("Compositor: Destroyed");
  return 0;
}

kr_compositor *kr_compositor_create(kr_compositor_setup *setup) {
  kr_compositor *com;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_graph_setup graph_setup;
  if (setup == NULL) return NULL;
  printk("Compositor: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_compositor);
  pool_setup.size = kr_compositor_path_size();
  pool_setup.slices = KC_MAX_PORTS;
  pool = kr_pool_create(&pool_setup);
  com = kr_pool_overlay_get(pool);
  memset(com, 0, sizeof(kr_compositor));
  com->path_pool = pool;
  com->user = setup->user;
  com->event_cb = setup->event_cb;
  FT_Init_FreeType(&com->ftlib);
  subunits_create(com);
  com->image.w = KR_COMPOSITOR_WIDTH_DEF;
  com->image.h = KR_COMPOSITOR_HEIGHT_DEF;
  com->image.pps[0] = com->image.w * 4;
  com->image.fmt = PIX_FMT_RGB32;
  com->image_pool = kr_image_pool_create(&com->image, DEFAULT_COMPOSITOR_BUFFER_FRAMES);
  memset(&graph_setup, 0, sizeof(graph_setup));
  com->graph = kr_graph_create(&graph_setup);
  printk("Compositor: Created");
  return com;
}
