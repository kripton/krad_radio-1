#ifndef KRAD_ROUTER_H
#define KRAD_ROUTER_H

#include "krad_system.h"
#include "krad_pool.h"
#include "krad_app_common.h"

typedef struct kr_router_setup kr_router_setup;
typedef struct kr_router_info kr_router_info;
typedef struct kr_router_map_setup kr_router_map_setup;
typedef struct kr_router_map_info kr_router_map_info;
typedef struct kr_router kr_router;
typedef struct kr_router_map kr_router_map;

typedef void *(kr_router_map_create_handler)(void *, void *);
typedef int (kr_router_map_patch_handler)(void *, void *);
typedef int (kr_router_map_destroy_handler)(void *);

struct kr_router_info {
  uint64_t maps;
};

struct kr_router_map_info {
  uint64_t endpoints;
};

struct kr_router_map_setup {
  char prefix[32];
  void *ptr; /* for create */
  kr_router_map_create_handler *create;
  kr_router_map_patch_handler *patch;
  kr_router_map_destroy_handler *destroy;
};

struct kr_router_setup {
  int routes_max;
  int maps_max;
};

int kr_router_handle(kr_router *router, kr_crate2 *crate);
int kr_router_map_destroy(kr_router *router, kr_router_map *map);
kr_router_map *kr_router_map_create(kr_router *router, kr_router_map_setup *setup);
int kr_router_info_get(kr_router *router, kr_router_info *info);
int kr_router_destroy(kr_router *router);
kr_router *kr_router_create(kr_router_setup *setup);

#endif