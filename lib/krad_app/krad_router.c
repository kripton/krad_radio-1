#include "krad_router.h"

struct kr_router {
  kr_pool *maps;
};

typedef struct {
  char *slice[4];
  size_t slices;
} address_sliced;

/*
  struct kr_route {
  char name[48];
  void *ptr;
  kr_app_address_mapper *mapper;
};
*/

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
  free(router);
  return 0;
}

kr_router *kr_router_create(kr_router_setup *setup) {
  kr_router *router;
  router = calloc(1, sizeof(kr_router));
  kr_pool_setup pool_setup;
  pool_setup.size = sizeof(kr_router_map);
  pool_setup.slices = setup->maps_max;
  pool_setup.shared = 0;
  pool_setup.overlay_sz = 0;
  router->maps = kr_pool_create(&pool_setup);
  return router;
}

void slice_address(address_sliced *sliced, char *addr) {
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

void address_slices_print(address_sliced *sliced) {
  int i;
  printk("%zu slices", sliced->slices);
  for (i = 0; i < sliced->slices; i++) {
    printk("Slice %d: %s", i, sliced->slice[i]);
  }
}

int kr_router_route(kr_router *router, kr_crate2 *crate) {
  int i;
  int len;
  address_sliced sliced;
  kr_router_map *map;
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
  //printk("method and slice count compatible");
  while ((map = kr_pool_iterate_active(router->maps, &i))) {
    //printk("Checking Mapper Prefix: %s", mapper->prefix);
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
        printk("I should call %p with %p!", map->create, map->ptr);
        return 0;
      }
      printke("hrm wtf!");
      return -4;
    }
  }
  printke("Route not found :/");
  address_slices_print(&sliced);
  return -5;
}

static void router_test(kr_router *router) {
  kr_crate2 crate;
  memset(&crate, 0, sizeof(kr_crate2));

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_PUT;
  kr_router_route(router, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_GET;
  kr_router_route(router, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_GET;
  kr_router_route(router, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_router_route(router, &crate);

  strcpy(crate.address, "/mixer/Music3");
  crate.method = KR_DELETE;
  kr_router_route(router, &crate);

  strcpy(crate.address, "/mixer");
  crate.method = KR_PUT;
  kr_router_route(router, &crate);
}

int kr_router_map_destroy(kr_router *router, kr_router_map *map) {
  return -1;
}

kr_router_map *kr_router_map_create(kr_router *router, kr_router_map_setup *setup) {
  if ((router == NULL) || (setup == NULL)) return NULL;
  //check all for null basically
  void *slice;
  slice = kr_pool_slice(router->maps);
  if (slice == NULL) return NULL;
  memcpy(slice, setup, sizeof(kr_router_map));
  printk("Added mapper for: %s", setup->prefix);
  router_test(router);
  return NULL;
}
