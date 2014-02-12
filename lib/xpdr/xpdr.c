#include "adapter_monitor.h"
#include "kr_xpdr.h"
#include "krad_pool.h"
#include "krad_av.h"
#include "krad_compositor_so_path.h"
#include "kr_adapter.h"

#include "adapters.c"

typedef enum {
  INVALID = -1,
  ADAPTER_CTX,
  ADAPTER_LINK
} path_type;

typedef enum {
  MIXER,
  COMPOSITOR,
  ADAPTER
} link_io_type;

typedef union {
  void *exists;
  kr_mixer_path *mixer_port;
  kr_compositor_path *compositor_port;
  kr_adapter_path *adapter_path;
} link_io;

typedef struct {
  kr_adapter *adapter;
  kr_xpdr_type adapter_type;
} xpdr_adapter;

typedef struct {
  xpdr_adapter *adapter;
  link_io_type input_type;
  link_io input;
  link_io_type output_type;
  link_io output;
  kr_audio audio;
  kr_image image;
} adapter_link;

struct kr_xpdr_path {
  union {
    xpdr_adapter adapter;
    adapter_link link;
  } state;
  path_type type;
  kr_xpdr_path_info info;
  void *user;
  kr_xpdr *xpdr;
};

typedef struct {
  path_type type;
  kr_xpdr_path_info *info;
  kr_xpdr_path *adapter_path;
  void *user;
} xpdr_path_setup;

struct kr_xpdr {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_pool *path_pool;
  kr_adapter_monitor *monitor;
  void *user;
  kr_xpdr_event_cb *event_cb;
};

static void compositor_frame(kr_frame_event *event);
static void mixer_audio(kr_audio_event *event);
static void adapter_event(kr_adapter_event *event);
static void link_io_destroy(kr_xpdr_path *path);
static void link_io_create(kr_xpdr_path *path);
static int path_destroy(kr_xpdr_path *path);
static int path_setup(kr_xpdr_path *path, xpdr_path_setup *setup);
static kr_xpdr_path *path_create(kr_xpdr *xpdr, xpdr_path_setup *setup);

static void compositor_frame(kr_frame_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  event->image = path->state.link.image;
}

static void mixer_audio(kr_audio_event *event) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)event->user;
  event->audio = path->state.link.audio;
}

static void adapter_av(kr_adapter_path_av_cb_arg *arg) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)arg->user;
  path->state.link.audio = arg->audio;
  path->state.link.image = arg->image;
}

static void adapter_event(kr_adapter_event *event) {
  /*
  kr_xpdr *xpdr;
  kr_xpdr_path *path;
  kr_adapter_info info;
  int i;
  path = (kr_xpdr_path *)event->user;
  */
  /* Created / destroyed? or is that on xpdr ? */
  if (event->type == KR_ADAPTER_DISCONNECTED) {
    printk("Adapter Disconnected");
    return;
  }
  if (event->type == KR_ADAPTER_RECONNECTED) {
    printk("Adapter Reconnected");
    return;
  }
  if (event->type != KR_ADAPTER_PROCESS) {
    printk("Wasn't a process event");
    return;
  }
  /*
  kr_adapter_get_info(event->adapter, &info);
  *//* I think we should trigger all of the mixer ports that are connected
   * to this adapter path *//*
  int test = 0;
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    if (path->info.output.type == ADAPTER) {
      if (path->info.output.type == COMPOSITOR) {
        kr_compositor_process(path->input.compositor_port);
      }
      if (path->info.input.type == MIXER) {
        kr_mixer_process(path->input.mixer_port);
      }
      test++;
    }
  }
  */
}

static void link_io_destroy(kr_xpdr_path *path) {
  link_io *io;
  link_io_type type;
  if (path->state.link.input.exists) {
    type = path->state.link.input_type;
    io = &path->state.link.input;
  } else {
    type = path->state.link.output_type;
    io = &path->state.link.output;
  }
  switch (type) {
    case ADAPTER:
      adapters[path->state.link.adapter->adapter_type].unlink(io->adapter_path);
      break;
    case MIXER:
      kr_mixer_remove(io->mixer_port);
      break;
    case COMPOSITOR:
      kr_compositor_remove(io->compositor_port);
      break;
  }
  io->exists = NULL;
}

static void link_io_create(kr_xpdr_path *path) {
  link_io *io;
  link_io_type type;
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_adapter *adapter;
  kr_mixer_port_setup mp_setup;
  kr_compositor_port_setup cp_setup;
  kr_adapter_path_setup ap_setup;
  mixer = path->xpdr->mixer;
  compositor = path->xpdr->compositor;
  adapter = path->state.link.adapter->adapter;
  if (path->state.link.output.exists) {
    type = path->state.link.input_type;
    io = &path->state.link.input;
  } else {
    type = path->state.link.output_type;
    io = &path->state.link.output;
  }
  switch (type) {
    case ADAPTER:
      ap_setup.info = path->info;
      ap_setup.av_cb = adapter_av;
      ap_setup.user = path;
      io->adapter_path = adapters[path->state.link.adapter->adapter_type].link(adapter, &ap_setup);
      if (io->adapter_path == NULL) {
        printke("adapter mkpath returned NULL");
      }
      break;
    case MIXER:
      /* mp_setup.info setup srate?channels? FIXME */
      mp_setup.audio_cb = mixer_audio;
      mp_setup.audio_user = path;
      mp_setup.control_user = path->user;
      io->mixer_port = kr_mixer_port(mixer, &mp_setup);
      if (io->mixer_port == NULL) {
        printke("mixer port returned NULL");
      }
      break;
    case COMPOSITOR:
      /* cp_setup.info setup frame size? FIXME */
      cp_setup.frame_cb = compositor_frame;
      cp_setup.frame_user = path;
      cp_setup.control_user = path->user;
      io->compositor_port = kr_compositor_port(compositor, &cp_setup);
      if (io->compositor_port == NULL) {
        printke("compositor port returned NULL");
      }
      break;
  }
}

static int path_destroy(kr_xpdr_path *path) {
  int ret;
  ret = 0;
  if (path->type == ADAPTER_CTX) {
    ret = adapters[path->state.adapter.adapter_type].close(path->state.adapter.adapter);
  } else {
    link_io_destroy(path);
    link_io_destroy(path);
  }
  return ret;
}

static int path_setup(kr_xpdr_path *path, xpdr_path_setup *setup) {
  path->info = *setup->info;
  path->user = setup->user;
  path->type = setup->type;
  kr_adapter_setup adapter_setup;
  switch (path->type) {
    case INVALID: return -1;
    case ADAPTER_CTX:
      adapter_setup.info = path->info;
      adapter_setup.event_cb = adapter_event;
      adapter_setup.user = path;
      path->state.adapter.adapter_type = path->info.type;
      printk("XPDR: adapter context create");
      path->state.adapter.adapter = adapters[path->state.adapter.adapter_type].open(&adapter_setup);
      if (path->state.adapter.adapter == NULL) return -1;
      return 0;
    case ADAPTER_LINK:
      printk("XPDR: adapter path create");
      path->state.link.adapter = &setup->adapter_path->state.adapter;
      switch (xpdr_type_modes[path->info.type]) {
        case KR_VIDEO_IN:
          path->state.link.input_type = ADAPTER;
          path->state.link.output_type = COMPOSITOR;
          break;
        case KR_VIDEO_OUT:
          path->state.link.input_type = COMPOSITOR;
          path->state.link.output_type = ADAPTER;
          break;
        case KR_AUDIO_IN:
          path->state.link.input_type = ADAPTER;
          path->state.link.output_type = MIXER;
          break;
        case KR_AUDIO_OUT:
          path->state.link.input_type = MIXER;
          path->state.link.output_type = ADAPTER;
          break;
        default:
          return -2;
      }
      link_io_create(path);
      link_io_create(path);
      return 0;
  }
  return -3;
}

static kr_xpdr_path *path_create(kr_xpdr *xpdr, xpdr_path_setup *setup) {
  /*Start debug*/
  char string[8192];
  kr_xpdr_path_info_to_text(string, setup->info, sizeof(string));
  printk("XPDR: path_create:\n%s", string);
  /*End debug*/
  int ret;
  kr_xpdr_path *path;
  kr_xpdr_event event;
  path = kr_pool_slice(xpdr->path_pool);
  if (path == NULL) return NULL;
  path->xpdr = xpdr;
  ret = path_setup(path, setup);
  if (ret) {
    kr_pool_recycle(xpdr->path_pool, path);
    return NULL;
  }
  event.user = path->xpdr->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_CREATE;
  event.info = path->info;
  path->xpdr->event_cb(&event);
  path->user = event.user_path;
  return path;
}

int kr_xpdr_ctl(kr_xpdr_path *path, kr_xpdr_path_info_patch *patch) {
  int ret;
  if ((path == NULL) || (patch == NULL)) return -1;
  printk("XPDR: control");
  //ret = kr_xpdr_path_info_patch_apply(&path->info, patch);
  return ret;
}

int kr_xpdr_remove(kr_xpdr_path *path) {
  kr_xpdr *xpdr;
  kr_xpdr_event event;
  if (path == NULL) return -1;
  xpdr = path->xpdr;
  printk("XPDR: remove");
  path_destroy(path);
  event.user = path->xpdr->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_DESTROY;
  event.info = path->info;
  path->xpdr->event_cb(&event);
  kr_pool_recycle(xpdr->path_pool, path);
  return -1;
}

int kr_xpdr_link(kr_xpdr_path *path, kr_xpdr_path_info *info, void *user) {
  kr_xpdr_path *new_path;
  xpdr_path_setup path_setup;
  if ((path == NULL) || (info == NULL)) return -1;
  path_setup.info = info;
  path_setup.user = user;
  path_setup.adapter_path = path;
  path_setup.type = ADAPTER_LINK;
  new_path = path_create(path->xpdr, &path_setup);
  if (new_path == NULL) return -2;
  return 0;
}

int kr_xpdr_open(kr_xpdr *xpdr, kr_xpdr_path_info *info, void *user) {
  kr_xpdr_path *path;
  xpdr_path_setup path_setup;
  if ((xpdr == NULL) || (info == NULL)) return -1;
  path_setup.info = info;
  path_setup.user = user;
  path_setup.adapter_path = NULL;
  path_setup.type = ADAPTER_CTX;
  path = path_create(xpdr, &path_setup);
  if (path == NULL) return -2;
  return 0;
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
    if (path->type == ADAPTER_CTX) continue;
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
  return xpdr;
}
