#include "krad_router.h"

struct kr_router {
  void *user;
  kr_router_response_handler *response;
  kr_pool *maps;
  kr_pool *routes;
  kr_pool *names;
  char name[64];
};

typedef struct {
  char *slice[4];
  size_t slices;
  char address[128];
} address_sliced;

struct kr_name {
  char name[48];
  int use;
};

struct kr_route {
  void *ptr;
  kr_router_map *map;
  char name[48];
  kr_payload payload;
};

struct kr_router_map {
  char prefix[32];
  void *ptr; /* for create */
  kr_payload_type payload_type;
  kr_router_map_create_handler *create;
  kr_router_map_connect_handler *connect;
  kr_router_map_patch_handler *patch;
  kr_router_map_destroy_handler *destroy;
};

int kr_router_info_get(kr_router *router, kr_router_info *info) {
  int i;
  int p;
  int m;
  kr_router_map *map;
  if ((router == NULL) || (info == NULL)) return -1;
  i = 0;
  p = 0;
  m = sizeof(info->raml);
  p += snprintf(info->raml + p, m - p, "#%%RAML 0.8\n\n"
   "title: %s api\n", router->name);
  while ((map = kr_pool_iterate_active(router->maps, &i))) {
    p += snprintf(info->raml + p, m - p, "%s:\n", map->prefix);
    if (1) p += snprintf(info->raml + p, m - p, "  get:\n");
    if (1) p += snprintf(info->raml + p, m - p, "  /{name}:\n");
    if (1) p += snprintf(info->raml + p, m - p, "    get:\n");
    if (map->create) p += snprintf(info->raml + p, m - p, "    put:\n");
    if (map->patch) p += snprintf(info->raml + p, m - p, "    patch:\n");
    if (map->destroy) p += snprintf(info->raml + p, m - p, "    delete:\n");
    if (map->connect) {
      if (1) p += snprintf(info->raml + p, m - p, "  /{name}/{name2}:\n");
      if (1) p += snprintf(info->raml + p, m - p, "    get:\n");
      if (map->create) p += snprintf(info->raml + p, m - p, "    put:\n");
      if (map->patch) p += snprintf(info->raml + p, m - p, "    patch:\n");
      if (map->destroy) p += snprintf(info->raml + p, m - p, "    delete:\n");
    }
  }
  return 0;
}

static void router_debug(kr_router *router) {
  kr_router_info info;
  kr_router_info_get(router, &info);
  printk("\n%s\n", info.raml);
}

int kr_router_destroy(kr_router *router) {
  if (router == NULL) return -1;
  router_debug(router);
  kr_pool_destroy(router->maps);
  kr_pool_destroy(router->routes);
  kr_pool_destroy(router->names);
  free(router);
  return 0;
}

kr_router *kr_router_create(kr_router_setup *setup) {
  kr_router *router;
  kr_pool_setup pool_setup;
  router = kr_allocz(1, sizeof(kr_router));
  memcpy(router->name, setup->name, sizeof(router->name));
  router->user = setup->user;
  router->response = setup->response;
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
    } else {
      pool_setup.size = sizeof(kr_name);
      pool_setup.slices = setup->maps_max;
      pool_setup.shared = 0;
      pool_setup.overlay_sz = 0;
      router->names = kr_pool_create(&pool_setup);
      if (router->names == NULL) {
        kr_pool_destroy(router->routes);
        kr_pool_destroy(router->maps);
        free(router);
        router = NULL;
      }
    }
  }
  return router;
}

static void slice_address(address_sliced *sliced, char *addr) {
  char *slice;
  memset(sliced, 0, sizeof(address_sliced));
  strncpy(sliced->address, addr, sizeof(sliced->address));
  slice = sliced->address;
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

static kr_name *find_name(kr_router *router, char *name) {
  int i;
  kr_name *space;
  i = 0;
  int len;
  int namelen;
  namelen = strlen(name);
  while ((space = kr_pool_iterate_active(router->names, &i))) {
    len = strlen(space->name);
    if ((namelen == len) && (memcmp(space->name, name, len) == 0)) {
      return space;
    }
  }
  return NULL;
}

static kr_name *create_name(kr_router *router, char *name) {
  kr_name *space;
  space = find_name(router, name);
  if (space != NULL) return NULL;
  space = (kr_name *)kr_pool_slice(router->names);
  if (space == NULL) return NULL;
  memset(space, 0, sizeof(kr_name));
  strncpy(space->name, name, sizeof(space->name));
  printk("Created name: %s", space->name);
  return space;
}

/*
static int destroy_name(kr_router *router, kr_name *name) {
  int ret;
  if ((router == NULL) || (name == NULL)) return -1;
  ret = kr_pool_recycle(router->names, name);
  return ret;
}
*/

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

static kr_router_map *map_from_ptr(kr_router *router, void *ptr) {
  int i;
  kr_router_map *map;
  i = 0;
  while ((map = kr_pool_iterate_active(router->maps, &i))) {
    if (map->ptr == ptr) return map;
  }
  return NULL;
}

kr_route *kr_route_create(kr_router *router, kr_route_setup *setup) {
  kr_route *route;
  kr_router_map *map;
  if ((router == NULL) || (setup == NULL)) return NULL;
  map = map_from_ptr(router, setup->ptr);
  if (map == NULL) return NULL;
  route = find_route(router, map, setup->name->name);
  if (route != NULL) return NULL;
  route = (kr_route *)kr_pool_slice(router->routes);
  if (route == NULL) return NULL;
  memset(route, 0, sizeof(kr_route));
  route->map = map;
  route->ptr = setup->ctx; /* tricky ! */
  strncpy(route->name, setup->name->name, sizeof(route->name));
  setup->name->use++;
  //route->payload_type = setup->payload_type;
  route->payload = setup->payload;
  printk("Created route: %s/%s", map->prefix, route->name);
  return route;
}

/*
static int destroy_route(kr_router *router, kr_route *route) {
  int ret;
  if ((router == NULL) || (route == NULL)) return -1;
  ret = kr_pool_recycle(router->routes, route);
  return ret;
}
*/

int kr_router_handle(kr_router *router, kr_crate *crate) {
  int i;
  int len;
  int ret;
  kr_crate outcrate;
  address_sliced sliced;
  kr_router_map *map;
  kr_route *route;
  kr_route *route2;
  kr_name *name;
  char temp[64];
  if ((router == NULL) || (crate == NULL)) return -1;
  i = 0;
  printk("Routing: %s", crate->address);
  slice_address(&sliced, crate->address);
  if (sliced.slices < 1) return -1;
  if (sliced.slices > 3) {
    printke("to many slices in address");
    return -2;
  }
  if ((sliced.slices == 1) && (crate->method != KR_GET)) {
    printk("incompatible slice count and method");
    return -3;
  }
  while ((map = kr_pool_iterate_active(router->maps, &i))) {
    len = strlen(map->prefix + 1);

    //printk(" %s - %s ", map->prefix + 1, sliced.slice[0]);

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
        route = find_route(router, map, sliced.slice[1]);
        if (route == NULL) return 0;
        outcrate.method = crate->method;
        strcpy(outcrate.address, crate->address);
        outcrate.payload_type = map->payload_type;
        outcrate.payload = route->payload;
        router->response(router->user, &outcrate);
        return 0;
      }
      if ((crate->method == KR_GET) && (sliced.slices == 3)) {
        printk("Found route! GET %s from %s %s!", sliced.slice[2],
         sliced.slice[1], sliced.slice[0]);
        snprintf(temp, sizeof(temp), "%s/%s", sliced.slice[1], sliced.slice[2]);
        route = find_route(router, map, temp);
        if (route == NULL) return 0;
        outcrate.method = crate->method;
        strcpy(outcrate.address, crate->address);
        outcrate.payload_type = map->payload_type;
        outcrate.payload = route->payload;
        router->response(router->user, &outcrate);
        return 0;
      }
      if ((crate->method == KR_PATCH) && (sliced.slices == 2)) {
        printk("Found route! PATCH for %s on %s!", sliced.slice[1],
         sliced.slice[0]);
        route = find_route(router, map, sliced.slice[1]);
        if (route == NULL) return 0;
        ret = map->patch(route->ptr, (void *)&crate->payload);
        return ret;
      }
      if ((crate->method == KR_DELETE) && (sliced.slices == 2)) {
        printk("Found route! DELETE %s from %s!", sliced.slice[1],
         sliced.slice[0]);
        route = find_route(router, map, sliced.slice[1]);
        if (route == NULL) return 0;
        ret = map->destroy(route->ptr);
        //ret = destroy_route(router, route);
        return ret;
      }
      if ((crate->method == KR_PUT) && (sliced.slices == 2)) {
        printk("Found route! PUT %s in %s!", sliced.slice[1],
         sliced.slice[0]);
        //printk("I will call %p with %p!", map->create, map->ptr);
        if (sliced.slice[1] == NULL) return -9;
        name = create_name(router, sliced.slice[1]);
        if (!name) return -6;
        ret = map->create(map->ptr, (void *)&crate->payload, name);
        return ret;
      }
      if ((crate->method == KR_PUT) && (sliced.slices == 3)) {
        printk("Found route! put %s in %s from %s!", sliced.slice[2],
         sliced.slice[1], sliced.slice[0]);
        route = find_route(router, map, sliced.slice[1]);
        if (route == NULL) return 0;
        route2 = find_route(router, map, sliced.slice[2]);
        if (route2 == NULL) return 0;
        snprintf(temp, sizeof(temp), "%s/%s", sliced.slice[1], sliced.slice[2]);
        name = create_name(router, temp);
        if (!name) return -6;
        //printk("I will call %p with %p - %p - %p - %p!", map->connect, map->ptr, route->ptr, route2->ptr, name);
        ret = map->connect(map->ptr, (void *)&crate->payload, route->ptr, route2->ptr, name);
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
  kr_router_map *map;
  if ((router == NULL) || (setup == NULL)) return NULL;
  //check all for null basically
  map = (kr_router_map *)kr_pool_slice(router->maps);
  if (map == NULL) return NULL;
  strncpy(map->prefix, setup->prefix, sizeof(map->prefix));
  map->ptr = setup->ptr;
  map->create = setup->create;
  map->connect = setup->connect;
  map->patch = setup->patch;
  map->destroy = setup->destroy;
  map->payload_type = setup->payload_type;
  printk("Router: Added map for: %s", map->prefix);
  return map;
}
