#include "krad_adapter.h"
#include "krad_pool.h"

typedef union {
  void *exists;
  kr_jack *jack;
  kr_v4l2 *v4l2;
  kr_wayland *wayland;
  kr_decklink *decklink;
  kr_x11 *x11;
  kr_alsa *alsa;
  void *krad;
  /*kr_encoder *encoder;*/
} kr_adapter_handle;

typedef union {
  kr_jack_path *jack;
  kr_v4l2 *v4l2;
  kr_wayland_path *wayland;
  kr_decklink *decklink;
  kr_x11_path *x11;
  kr_alsa_path *alsa;
  void *krad;
  /*kr_encoder_path *encoder;*/
} kr_adapter_api_path;

struct kr_adapter_path {
  void *user;
  kr_adapter_path_av_cb *av_cb;
  kr_adapter_path_event_cb *ev_cb;
  kr_adapter *adapter;
  kr_adapter_api_path api_path;
  kr_adapter_path_info info;
};

struct kr_adapter {
  kr_adapter_info info;
  kr_adapter_handle handle;
  kr_pool *path_pool;
  void *user;
  kr_adapter_event_cb *ev_cb;
  kr_adapter_process_function *process_function;
  kr_adapter_path_process_function *path_process_function;
  pthread_t process_thread;
};

void *adapter_process_thread(void *arg) {
  kr_adapter *adapter;
  adapter = (kr_adapter *)arg;
  adapter->process_function(adapter);
  return NULL;
}

void *adapter_path_process_thread(void *arg) {
  kr_adapter_path *path;
  path = (kr_adapter_path *)arg;
  path->adapter->path_process_function(path);
  return NULL;
}

void adapter_process_thread_start(kr_adapter *adapter) {
  pthread_create(&adapter->process_thread, NULL, adapter_process_thread, adapter);
}

void adapter_path_process_thread_start(kr_adapter_path *path) {
  pthread_create(&path->adapter->process_thread, NULL, adapter_path_process_thread, path);
}

#include "adapters/jack.c"
#include "adapters/wayland.c"
#include "adapters/v4l2.c"
#include "adapters/decklink.c"
#include "adapters/x11.c"
#include "adapters/alsa.c"
#include "adapters/krad.c"

static int path_setup_check(kr_adapter_path_setup *setup);
static void path_create(kr_adapter_path *path, kr_adapter_path_setup *setup);
static void path_destroy(kr_adapter_path *path);

static int path_setup_check(kr_adapter_path_setup *setup) {
  if (setup == NULL) return -1;
  /* FIXME check things */
  return 0;
}

static void path_create(kr_adapter_path *path, kr_adapter_path_setup *setup) {
  path->user = setup->user;
  path->ev_cb = setup->ev_cb;
  path->av_cb = setup->av_cb;
  memcpy(&path->info, &setup->info, sizeof(kr_adapter_path_info));
  printk("ok we are going to create an adapter path");
  switch (path->adapter->info.api) {
    case KR_ADP_WAYLAND:
      wayland_adapter_path_create(path);
      break;
    case KR_ADP_DECKLINK:
      decklink_adapter_path_create(path);
      break;
    case KR_ADP_V4L2:
      v4l2_adapter_path_create(path);
      break;
    case KR_ADP_JACK:
      jack_adapter_path_create(path);
      break;
    case KR_ADP_X11:
      x11_adapter_path_create(path);
      break;
    case KR_ADP_ALSA:
      x11_adapter_path_create(path);
      break;
    case KR_ADP_KRAD:
      krad_adapter_path_create(path);
      break;
    default:
      break;
  }
}

static void path_destroy(kr_adapter_path *path) {
  switch (path->info.api) {
    case KR_ADP_WAYLAND:
      wayland_adapter_path_destroy(path);
      break;
    case KR_ADP_DECKLINK:
      decklink_adapter_path_destroy(path);
      break;
    case KR_ADP_V4L2:
      v4l2_adapter_path_destroy(path);
      break;
    case KR_ADP_JACK:
      jack_adapter_path_destroy(path);
      break;
    case KR_ADP_X11:
      x11_adapter_path_destroy(path);
      break;
    case KR_ADP_ALSA:
      alsa_adapter_path_destroy(path);
      break;
    case KR_ADP_KRAD:
      krad_adapter_path_destroy(path);
      break;
    default:
      break;
  }
}

int kr_adapter_unlink(kr_adapter_path *path) {
  kr_adapter *adapter;
  if (path == NULL) return -1;
  adapter = path->adapter;
  path_destroy(path);
  kr_pool_recycle(adapter->path_pool, path);
  return 0;
}

kr_adapter_path *kr_adapter_mkpath(kr_adapter *adapter,
 kr_adapter_path_setup *setup) {
  kr_adapter_path *path;
  path = NULL;
  if (adapter == NULL) return NULL;
  if (setup == NULL) return NULL;
  if (adapter->handle.exists == NULL) return NULL;
  if (adapter->info.api != setup->info.api) return NULL;
  if (path_setup_check(setup)) return NULL;
  path = kr_pool_slice(adapter->path_pool);
  if (path == NULL) return NULL;
  path->adapter = adapter;
  path_create(path, setup);
  return path;
}

int kr_adapter_get_info(kr_adapter *adapter, kr_adapter_info *info) {
  if (adapter == NULL) return -1;
  if (info == NULL) return -2;
  memcpy(info, &adapter->info, sizeof(kr_adapter_info));
  return 0;
}

int kr_adapter_destroy(kr_adapter *adapter) {
  int i;
  kr_adapter_path *path;
  if (adapter == NULL) return -1;
  printk("Adapter destroy started");
  i = 0;
  while ((path = kr_pool_iterate_active(adapter->path_pool, &i))) {
    kr_adapter_unlink(path);
  }
  switch (adapter->info.api) {
    case KR_ADP_WAYLAND:
      wayland_adapter_destroy(adapter);
      break;
    case KR_ADP_DECKLINK:
      decklink_adapter_destroy(adapter);
      break;
    case KR_ADP_V4L2:
      v4l2_adapter_destroy(adapter);
      break;
    case KR_ADP_JACK:
      jack_adapter_destroy(adapter);
      break;
    case KR_ADP_X11:
      x11_adapter_destroy(adapter);
      break;
    case KR_ADP_ALSA:
      alsa_adapter_destroy(adapter);
      break;
    case KR_ADP_KRAD:
      krad_adapter_destroy(adapter);
      break;
    default:
      break;
  }
  kr_pool_destroy(adapter->path_pool);
  printk("Adapter destroy completed");
  return 0;
}

kr_adapter *kr_adapter_create(kr_adapter_setup *setup) {
  kr_adapter *adapter;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (setup == NULL) return NULL;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*adapter);
  pool_setup.size = sizeof(kr_adapter_path);
  pool_setup.slices = KR_ADAPTER_PATHS_MAX;
  pool = kr_pool_create(&pool_setup);
  adapter = kr_pool_overlay_get(pool);
  memset(adapter, 0, sizeof(*adapter));
  adapter->path_pool = pool;
  adapter->user = setup->user;
  adapter->ev_cb = setup->ev_cb;
  memcpy(&adapter->info, &setup->info, sizeof(kr_adapter_info));
  switch (adapter->info.api) {
    case KR_ADP_WAYLAND:
      wayland_adapter_create(adapter);
      return adapter;
    case KR_ADP_DECKLINK:
      decklink_adapter_create(adapter);
      return adapter;
    case KR_ADP_V4L2:
      v4l2_adapter_create(adapter);
      return adapter;
    case KR_ADP_JACK:
      jack_adapter_create(adapter);
      return adapter;
    case KR_ADP_X11:
      x11_adapter_create(adapter);
      return adapter;
    case KR_ADP_ALSA:
      alsa_adapter_create(adapter);
      return adapter;
    case KR_ADP_KRAD:
      krad_adapter_create(adapter);
      return adapter;
    default:
      break;
  }
  /* Should never make it this far. */
  return NULL;
}
