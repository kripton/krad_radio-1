#include "adapter_monitor.h"
#include "kr_xpdr.h"
#include "krad_pool.h"
#include "krad_av.h"
#include "krad_compositor_so_path.h"
#include "kr_adapter.h"

#include "adapters.c"

struct kr_xpdr_path {
  union {
    kr_adapter adapter;
    struct {
      kr_adapter *adapter;
      kr_adapter_path adapter_path;
      union {
        kr_mixer_path *mixer_port;
        kr_compositor_path *compositor_port;
      };
    } link;
  };
  kr_xpdr_path_mode mode;
  kr_xpdr_path_info info;
  const kr_adapter_spec *adapter_func;
  kr_xpdr_type adapter_type;
  void *user;
  kr_xpdr *xpdr;
};

struct kr_xpdr {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_pool *path_pool;
  kr_adapter_monitor *monitor;
  void *user;
  kr_xpdr_event_cb *event_cb;
};

static int link_destroy(kr_xpdr_path *path);
static int link_create(kr_xpdr_path *path);
static int path_create(kr_xpdr *xpdr, kr_xpdr_path *ap, kr_xpdr_path_info *info, void *user);

#include "avio.c"

static int link_destroy(kr_xpdr_path *path) {
  int ret;
  int r;
  ret = 0;
  if ((path->mode == KR_AUDIO_IN) || (path->mode == KR_VIDEO_IN)) {
    r = path->adapter_func->unlink(&path->link.adapter_path);
    if (r != 0) {
      printke("XPDR: Problem unlinking adapter path");
      ret += r;
    }
  }
  if ((path->mode == KR_AUDIO_IN) || (path->mode == KR_AUDIO_OUT)) {
    r = kr_mixer_remove(path->link.mixer_port);
    if (r != 0) {
      printke("XPDR: Problem removing mixer port");
      ret += r;
    }
  }
  if ((path->mode == KR_VIDEO_IN) || (path->mode == KR_VIDEO_OUT)) {
    r = kr_compositor_remove(path->link.compositor_port);
    if (r != 0) {
      printke("XPDR: Problem removing compositor port");
      ret += r;
    }
  }
  if ((path->mode == KR_AUDIO_OUT) || (path->mode == KR_VIDEO_OUT)) {
    r = path->adapter_func->unlink(&path->link.adapter_path);
    if (r != 0) {
      printke("XPDR: Problem unlinking adapter path");
      ret += r;
    }
  }
  return ret;
}

static int link_create(kr_xpdr_path *path) {
  int ret;
  int r;
  kr_mixer_port_setup mp_setup;
  kr_compositor_port_setup cp_setup;
  ret = 0;
  path->link.adapter_path.av_handler = adapter_avio;
  path->link.adapter_path.user = path;
  path->link.adapter_path.info = &path->info;
  printk("XPDR: link create");
  if ((path->mode == KR_AUDIO_IN) || (path->mode == KR_AUDIO_OUT)) {
    if (path->mode == KR_AUDIO_IN) {
      mp_setup.info.type = KR_MXR_SOURCE;
    } else {
      mp_setup.info.type = KR_MXR_OUTPUT;
    }
    mp_setup.audio_cb = mixer_audio;
    mp_setup.audio_user = path;
    mp_setup.control_user = path->user;
    path->link.mixer_port = kr_mixer_port(path->xpdr->mixer, &mp_setup);
    if (path->link.mixer_port == NULL) {
      printke("XPDR: Creating mixer port returned NULL");
      ret += -1;
    }
  }
  if ((path->mode == KR_VIDEO_IN) || (path->mode == KR_VIDEO_OUT)) {
    if (path->mode == KR_VIDEO_IN) {
      cp_setup.info.type = KR_COM_SOURCE;
    } else {
      cp_setup.info.type = KR_COM_OUTPUT;
    }
    cp_setup.frame_user = path;
    cp_setup.control_user = path->user;
    path->link.compositor_port = kr_compositor_port(path->xpdr->compositor, &cp_setup);
    if (path->link.compositor_port == NULL) {
      printke("XPDR: Creating compositor port returned NULL");
      ret += -1;
    }
  }
  r = path->adapter_func->link(path->link.adapter, &path->link.adapter_path);
  if (r != 0) {
    printke("XPDR: Adapter create link problem");
    ret += r;
  }
  printk("XPDR: link create done");
  return ret;
}

static int path_create(kr_xpdr *xpdr, kr_xpdr_path *ap, kr_xpdr_path_info *info, void *user) {
  /*Start debug*/
  char string[8192];
  kr_xpdr_path_info_to_text(string, info, sizeof(string));
  printk("XPDR: path_create:\n%s", string);
  /*End debug*/
  int ret;
  kr_xpdr_path *path;
  kr_xpdr_event event;
  path = kr_pool_slice(xpdr->path_pool);
  if (path == NULL) return -1;
  path->xpdr = xpdr;
  path->info = *info;
  path->user = user;
  path->link.adapter = &ap->adapter;
  path->mode = xpdr_type_modes[path->info.type];
  path->adapter_type = adapter_path_adapter_type[path->info.type];
  path->adapter_func = &adapters[path->adapter_type];
  if (path->mode == KR_ADAPTER_CTX) {
    path->adapter.info = &path->info;
    path->adapter.event_cb = adapter_event;
    path->adapter.user = path;
    if ((path->info.type < 1)
     || (path->info.type >= nadapter_types)) {
      printke("XPDR: Adapter type invalid.");
      ret = -2;
    } else {
      if (adapters[path->adapter_type].open == NULL) {
        printke("XPDR: Adapter type is not supported on this system.");
        ret = -3;
      } else {
        printk("XPDR: adapter context create");
        ret = path->adapter_func->open(&path->adapter);
      }
    }
  } else {
    ret = link_create(path);
  }
  if (ret) {
    kr_pool_recycle(xpdr->path_pool, path);
    return -4;
  }
  event.user = path->xpdr->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_CREATE;
  event.info = path->info;
  path->xpdr->event_cb(&event);
  path->user = event.user_path;
  printk("XPDR: path create done");
  return ret;
}

int kr_xpdr_ctl(kr_xpdr_path *path, kr_xpdr_path_info_patch *patch) {
  int ret;
  ret = -1;
  if ((path == NULL) || (patch == NULL)) return -1;
  printk("XPDR: control");
  /*ret = kr_xpdr_path_info_patch_apply(&path->info, patch);*/
  return ret;
}

int kr_xpdr_remove(kr_xpdr_path *path) {
  int ret;
  ret = -1;
  kr_xpdr *xpdr;
  kr_xpdr_event event;
  if (path == NULL) return -1;
  xpdr = path->xpdr;
  printk("XPDR: remove");
  if (path->mode == KR_ADAPTER_CTX) {
    ret = path->adapter_func->close(&path->adapter);
    if (ret != 0) {
      printke("XPDR: Problem closing adapter ctx");
    }
  } else {
    ret = link_destroy(path);
  }
  event.user = path->xpdr->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_DESTROY;
  event.info = path->info;
  path->xpdr->event_cb(&event);
  kr_pool_recycle(xpdr->path_pool, path);
  return ret;
}

int kr_xpdr_link(kr_xpdr_path *path, kr_xpdr_path_info *info, void *user) {
  int ret;
  if ((path == NULL) || (info == NULL)) return -1;
  ret = path_create(path->xpdr, path, info, user);
  return ret;
}

int kr_xpdr_open(kr_xpdr *xpdr, kr_xpdr_path_info *info, void *user) {
  int ret;
  if ((xpdr == NULL) || (info == NULL)) return -1;
  ret = path_create(xpdr, NULL, info, user);
  return ret;
}

int kr_xpdr_monitor(kr_xpdr *xpdr, int on) {
  if (xpdr == NULL) return -1;
  if ((on != 0) && (on != 1)) return -2;
  if (on == 1) {
    if (xpdr->monitor != NULL) {
      printke("XPDR: Monitor was already enabled");
      return -3;
    }
    xpdr->monitor = kr_adapter_monitor_create();
    printk("XPDR: Enabled adapter monitor");
    kr_adapter_monitor_wait(xpdr->monitor, 0);
  } else {
    if (xpdr->monitor == NULL) {
      printke("XPDR: Monitor was already disabled");
      return -4;
    }
    kr_adapter_monitor_destroy(xpdr->monitor);
    xpdr->monitor = NULL;
    printk("XPDR: Disabled adapter monitor");
  }
  return 0;
}

int kr_xpdr_destroy(kr_xpdr *xpdr) {
  int i;
  int ret;
  kr_xpdr_path *path;
  if (xpdr == NULL) return -1;
  printk("XPDR: Destroying");
  kr_xpdr_monitor(xpdr, 0);
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    if (path->mode == KR_ADAPTER_CTX) continue;
    ret = kr_xpdr_remove(path);
    if (ret) printke("XPDR: Failure removing an adapter context");
  }
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    ret = kr_xpdr_remove(path);
    if (ret) printke("XPDR: Failure removing an adapter path");
  }
  kr_pool_destroy(xpdr->path_pool);
  printk("XPDR: Destroyed");
  return 0;
}

kr_xpdr *kr_xpdr_create(kr_xpdr_setup *setup) {
  kr_xpdr *xpdr;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (setup == NULL) return NULL;
  printk("XPDR: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_xpdr);
  pool_setup.size = sizeof(kr_xpdr_path);
  pool_setup.slices = setup->path_count;
  pool = kr_pool_create(&pool_setup);
  xpdr = kr_pool_overlay_get(pool);
  memset(xpdr, 0, sizeof(kr_xpdr));
  xpdr->path_pool = pool;
  xpdr->mixer = setup->mixer;
  xpdr->compositor = setup->compositor;
  xpdr->user = setup->user;
  xpdr->event_cb = setup->event_cb;
  printk("XPDR: Created");
  kr_adapters_debug_info();
  return xpdr;
}
