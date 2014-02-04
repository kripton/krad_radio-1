#include "krad_pool.h"
#include "krad_text.h"
#include "krad_sprite.h"
#include "krad_vector.h"
#include "krad_graph.h"
#include "krad_compositor.h"

struct kr_compositor {
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

static void overlays_free(kr_compositor *compositor);
static void overlays_create(kr_compositor *compositor);

static void overlays_free(kr_compositor *com) {
/*  int i;
  i = 0;
  void *overlay;
  while ((overlay = kr_pool_iterate_active(com->sprite_pool, &i))) {
    kr_sprite_clear(overlay);
    kr_pool_recycle(com->sprite_pool, overlay);
  }
  while ((overlay = kr_pool_iterate_active(com->text_pool, &i))) {
    kr_text_clear(com->text_pool, overlay);
    kr_pool_recycle(com->text_pool, overlay);
  }
  while ((overlay = kr_pool_iterate_active(com->vector_pool, &i))) {
    kr_pool_recycle(com->vector_pool, overlay);
  }
  kr_pool_destroy(com->sprite_pool);
  kr_pool_destroy(com->vector_pool);
  kr_pool_destroy(com->text_pool);
  FT_Done_FreeType(com->ftlib);*/
}

static void overlays_create(kr_compositor *compositor) {
  kr_pool_setup setup;
  setup.shared = 0;
  setup.overlay_sz = 0;
  setup.size = kr_sprite_size();
  setup.slices = KR_COMP_SPRITES_MAX;
  compositor->sprite_pool = kr_pool_create(&setup);
  setup.size = kr_vector_size();
  setup.slices = KR_COMP_VECTORS_MAX;
  compositor->vector_pool = kr_pool_create(&setup);
  setup.size = kr_text_size();
  setup.slices = KR_COMP_TEXTS_MAX;
  compositor->text_pool = kr_pool_create(&setup);
  FT_Init_FreeType(&compositor->ftlib);
}

int kr_compositor_destroy(kr_compositor *com) {
  if (com == NULL) return -1;
  printk("Compositor: Destroying");
  overlays_free(com);
  kr_graph_destroy(com->graph);
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
  pool_setup.slices = KR_COMP_PATHS_MAX;
  pool = kr_pool_create(&pool_setup);
  com = kr_pool_overlay_get(pool);
  memset(com, 0, sizeof(kr_compositor));
  com->path_pool = pool;
  com->user = setup->user;
  com->event_cb = setup->event_cb;
  overlays_create(com);
  memset(&graph_setup, 0, sizeof(graph_setup));
  com->graph = kr_graph_create(&graph_setup);
  printk("Compositor: Created");
  return com;
}
