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
typedef struct kr_route_setup kr_route_setup;
typedef struct kr_route kr_route;
typedef struct kr_name kr_name;
typedef struct kr_variant kr_variant;
typedef struct kr_variant kr_var;

typedef int (kr_router_map_create_handler)(void *, void *, void *);
typedef int (kr_router_map_create2_handler)(void *, void *, void *, void *, void *);
typedef int (kr_router_map_patch_handler)(void *, void *);
typedef int (kr_router_map_destroy_handler)(void *);
typedef int (kr_router_response_handler)(void *, kr_crate2 *crate);

typedef enum {
  KR_FLOAT = 1,
  KR_INTEGER,
} kr_value_type;

typedef union {
  int integer;
  float real;
  char str[8];
} kr_value;

struct kr_variant {
  kr_value value;
  kr_value_type type;
};

struct kr_router_info {
  uint64_t maps;
};

struct kr_router_map_info {
  uint64_t endpoints;
};

struct kr_router_map_setup {
  const char *prefix;
  void *ptr; /* for create */
  kr_router_map_create_handler *create;
  kr_router_map_create2_handler *create2;
  kr_router_map_patch_handler *patch;
  kr_router_map_destroy_handler *destroy;
};

struct kr_route_setup {
  kr_name *name;
  void *ptr; /* for map */
  void *ctx;
  kr_radio_payload_type payload_type;
  kr_radio_payload payload;
};

struct kr_router_setup {
  int routes_max;
  int maps_max;
  void *user;
  kr_router_response_handler *response;
};

kr_route *kr_route_create(kr_router *router, kr_route_setup *setup);
int kr_router_handle(kr_router *router, kr_crate2 *crate);
int kr_router_map_destroy(kr_router *router, kr_router_map *map);
kr_router_map *kr_router_map_create(kr_router *router, kr_router_map_setup *setup);
int kr_router_info_get(kr_router *router, kr_router_info *info);
int kr_router_destroy(kr_router *router);
kr_router *kr_router_create(kr_router_setup *setup);

#endif
