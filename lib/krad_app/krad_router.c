#include "krad_router.h"

struct kr_router {
  kr_pool *maps;
  kr_pool *routes;
};

typedef struct {
  char *slice[4];
  size_t slices;
} address_sliced;


struct kr_route {
  char name[48];
  void *ptr;
  kr_router_map *map;
};

struct kr_router_map {
  char prefix[32];
  void *ptr; /* for create */
  kr_router_map_create_handler *create;
  kr_router_map_patch_handler *patch;
  kr_router_map_destroy_handler *destroy;
};

int kr_router_info_get(kr_router *router, kr_router_info *info) {
  if ((router == NULL) || (info == NULL)) return -1;
  info->maps = 666;
  return 0;
}

int kr_router_destroy(kr_router *router) {
  if (router == NULL) return -1;
  kr_pool_destroy(router->maps);
  kr_pool_destroy(router->routes);
  free(router);
  return 0;
}

kr_router *kr_router_create(kr_router_setup *setup) {
  kr_router *router;
  router = calloc(1, sizeof(kr_router));
  kr_pool_setup pool_setup;
  pool_setup.size = sizeof(kr_route);
  pool_setup.slices = setup->maps_max;
  pool_setup.shared = 0;
  pool_setup.overlay_sz = 0;
  router->routes = kr_pool_create(&pool_setup);
  if (router->routes == NULL) {
    free(router);
    router = NULL;
  } else {
    pool_setup.size = sizeof(kr_router_map);
    pool_setup.slices = setup->maps_max;
    pool_setup.shared = 0;
    pool_setup.overlay_sz = 0;
    router->maps = kr_pool_create(&pool_setup);
    if (router->maps == NULL) {
      kr_pool_destroy(router->routes);
      free(router);
      router = NULL;
    }
  }
  return router;
}

static void slice_address(address_sliced *sliced, char *addr) {
  char *slice;
  memset(sliced, 0, sizeof(address_sliced));
  slice = addr;
  while ((slice = strchr(slice, '/'))) {
    sliced->slice[sliced->slices] = slice + 1;
    sliced->slices++;
    slice[0] = '\0';
    slice += 1;
    if (!strlen(slice)) {
      sliced->slices = 0;
      printke("zero length address slice");
      return;
    }
    if (sliced->slices == 3) break;
  }
}

static void address_slices_print(address_sliced *sliced) {
  int i;
  printk("%zu slices", sliced->slices);
  for (i = 0; i < sliced->slices; i++) {
    printk("Slice %d: %s", i, sliced->slice[i]);
  }
}

static kr_route *find_route(kr_router *router, kr_router_map *map, char *name) {
  int i;
  kr_route *route;
  i = 0;
  int len;
  int namelen;
  namelen = strlen(name);
  while ((route = kr_pool_iterate_active(router->routes, &i))) {
    if (route->map != map) continue;
    len = strlen(route->name);
    if ((namelen == len) && (memcmp(route->name, name, len) == 0)) {
      return route;
    }
  }
  return NULL;
}

static kr_route *create_route(kr_router *router, kr_router_map *map, char *name) {
  kr_route *route;
  route = find_route(router, map, name);
  if (route != NULL) return NULL;
  route = (kr_route *)kr_pool_slice(router->routes);
  if (route == NULL) return NULL;
  route->map = map;
  strncpy(route->name, name, sizeof(route->name));
  printk("Created route: %s/%s", map->prefix, route->name);
  return route;
}

static int destroy_route(kr_router *router, kr_route *route) {
  int ret;
  if ((router == NULL) || (route == NULL)) return -1;
  ret = kr_pool_recycle(router->routes, route);
  return ret;
}

int kr_router_handle(kr_router *router, kr_crate2 *crate) {
  int i;
  int len;
  int ret;
  address_sliced sliced;
  kr_router_map *map;
  kr_route *route;
  if ((router == NULL) || (crate == NULL)) return -1;
  i = 0;
  printk("Routing: %s", crate->address);
  slice_address(&sliced, crate->address);
  if (sliced.slices < 1) return -1;
  if (sliced.slices > 2) {
    printke("to many slices in address");
    return -2;
  }
  if ((sliced.slices == 1) && (crate->method != KR_GET)) {
    printk("incompatible slice count and method");
    return -3;
  }
  while ((map = kr_pool_iterate_active(router->maps, &i))) {
    len = strlen(map->prefix + 1);
    if (((strlen(sliced.slice[0])) == len)
     && (memcmp(map->prefix + 1, sliced.slice[0], len) == 0)) {
      /*
      if ((crate->method == KR_POST) && (sliced.slices == 1)) {
        printk("Found route! Its a post to %s!", sliced.slice[0]);
        return 0;
      }
      */
      if ((crate->method == KR_GET) && (sliced.slices == 1)) {
        printk("Found route! GET list of %s!", sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_GET) && (sliced.slices == 2)) {
        printk("Found route! GET %s from %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_PATCH) && (sliced.slices == 2)) {
        printk("Found route! PATCH for %s on %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_DELETE) && (sliced.slices == 2)) {
        printk("Found route! DELETE %s from %s!", sliced.slice[1],
         sliced.slice[0]);
        return 0;
      }
      if ((crate->method == KR_PUT) && (sliced.slices == 2)) {
        printk("Found route! PUT %s in %s!", sliced.slice[1],
         sliced.slice[0]);
        //printk("I will call %p with %p!", map->create, map->ptr);
        /* FIXME replace null with route ptr */
        if (sliced.slice[1] == NULL) return -9;
        route = create_route(router, map, sliced.slice[1]);
        if (!route) return -6;
        ret = map->create(map->ptr, (void *)&crate->payload, route);
        return ret;
      }
      printke("hrm wtf!");
      return -4;
    }
  }
  printke("Route not found :/");
  address_slices_print(&sliced);
  return -5;
}

int kr_router_map_destroy(kr_router *router, kr_router_map *map) {
  int ret;
  if ((router) || (map == NULL)) return -1;
  ret = kr_pool_recycle(router->maps, map);
  return ret;
}

kr_router_map *kr_router_map_create(kr_router *router, kr_router_map_setup *setup) {
  if ((router == NULL) || (setup == NULL)) return NULL;
  //check all for null basically
  void *slice;
  slice = kr_pool_slice(router->maps);
  if (slice == NULL) return NULL;
  memcpy(slice, setup, sizeof(kr_router_map));
  printk("Krad Router: Added map for: %s", setup->prefix);
  return slice;
}
