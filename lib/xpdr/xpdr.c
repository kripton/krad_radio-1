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
  xpdr_adapter adapter;
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
  kr_adapter_monitor *adapter_mon;
  void *user;
  kr_xpdr_event_cb *event_cb;
};

static void compositor_frame(kr_frame_event *event);
static void mixer_audio(kr_audio_event *event);
static void adapter_event(kr_adapter_event *event);
static void link_io_destroy(link_io *io, link_io_type type);
static void link_io_create(kr_xpdr_path *path);
static int path_setup(kr_xpdr_path *path);
static int path_destroy(kr_xpdr_path *path);
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

/*
static void adapter_av(kr_adapter_path_av_cb_arg *arg) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)arg->user;
  path->audio = arg->audio;
  path->image = arg->image;
}
*/

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

static void link_io_destroy(link_io *io, link_io_type type) {
  switch (type) {
    case ADAPTER:
      //kr_adapter_unlink(io->adapter_path);
      //ret = adapters[path->state.adapter.adapter_type].close(path->state.link.adapter);
      break;
    case MIXER:
      kr_mixer_remove(io->mixer_port);
      break;
    case COMPOSITOR:
      kr_compositor_remove(io->compositor_port);
      break;
  }
}

static void link_io_create(kr_xpdr_path *path) {
  link_io *io;
  link_io_type type;
  kr_mixer *mixer;
  kr_compositor *compositor;
  /*kr_adapter *adapter;*/
  kr_mixer_port_setup mp_setup;
  kr_compositor_port_setup cp_setup;
  mixer = path->xpdr->mixer;
  compositor = path->xpdr->compositor;
  if (path->state.link.output.exists) {
    type = path->state.link.input_type;
    io = &path->state.link.input;
  } else {
    type = path->state.link.output_type;
    io = &path->state.link.output;
  }
  switch (type) {
    case ADAPTER:
      /*
      memcpy(&ap_setup.info, &info->info.adapter_path_info,
       sizeof(kr_adapter_path_info));
      if (io == &path->input) {
        ap_setup.info.dir = KR_ADP_PATH_INPUT;
      } else {
        ap_setup.info.dir = KR_ADP_PATH_OUTPUT;
      }
      ap_setup.ev_cb = xpdr_adapter_path_event_cb;
      ap_setup.av_cb = xpdr_adapter_path_av_cb;
      ap_setup.user = path;
      //adapter = adapter_get(path->xpdr, &ap_setup);
      if (adapter == NULL) {
        printke("get_adapter returned NULL");
      } else {
        io->adapter_path = kr_adapter_mkpath(adapter, &ap_setup);
        if (io->adapter_path == NULL) {
          printke("adapter mkpath returned NULL");
        }
      }
      */
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

static int path_setup(kr_xpdr_path *path) {
  /*
  char string[8192];
  kr_xpdr_path_info_to_text(string, &path->info, sizeof(string));
  printk("XPDR: path setup\n%s", string);
  switch (path->info.type) {
    case KR_JACK:
    case KR_WAYLAND:
    case KR_V4L2:
    case KR_X11:
    case KR_ALSA:
    case KR_DECKLINK:
    case KR_AUX:
      path->adapter_type = path->info.type;
      path->type = ADAPTER_CTX;
      return 0;
    case KR_DECKLINK_IN:
    case KR_X11_IN:
    case KR_V4L2_IN:
      path->input_type = ADAPTER;
      path->output_type = COMPOSITOR;
      path->type = ADAPTER_LINK;
      return 0;
    case KR_ALSA_IN:
    case KR_JACK_IN:
    case KR_AUX_IN:
      path->input_type = ADAPTER;
      path->output_type = MIXER;
      path->type = ADAPTER_LINK;
      return 0;
    case KR_JACK_OUT:
    case KR_ALSA_OUT:
    case KR_AUX_OUT:
      path->input_type = MIXER;
      path->output_type = ADAPTER;
      path->type = ADAPTER_LINK;
    case KR_WAYLAND_OUT:
      path->input_type = COMPOSITOR;
      path->output_type = ADAPTER;
      path->type = ADAPTER_LINK;
      return 0;
    default:
      return -2;
  }
  */
  return -3;
}

static int path_destroy(kr_xpdr_path *path) {
  int ret;
  ret = 0;
  if (path->type == ADAPTER_CTX) {
    ret = adapters[path->state.adapter.adapter_type].close(path->state.adapter.adapter);
  } else {
    link_io_destroy(&path->state.link.input, path->state.link.input_type);
    link_io_destroy(&path->state.link.output, path->state.link.output_type);
  }
  return ret;
}

static kr_xpdr_path *path_create(kr_xpdr *xpdr, xpdr_path_setup *setup) {
  /*
  char string[8192];
  kr_xpdr_path_info_to_text(string, info, sizeof(string));
  printk("XPDR: mkpath-\n%s", string);
  int ret;
  kr_xpdr_path *path;
  path = kr_pool_slice(xpdr->path_pool);
  if (path == NULL) return -3;
  path->info = *info;
  path->type = ADAPTER_CTX;
  path->xpdr = xpdr;
  path->user = user;
  printk("XPDR: open");
  ret = path_create(path);
  if (ret) {
    kr_pool_recycle(xpdr->path_pool, path);
    return -4;
  }
  kr_xpdr_event event;
  event.user = path->xpdr->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_CREATE;
  event.info = path->info;
  path->xpdr->event_cb(&event);
  path->user = event.user_path;
  return 0;
  int ret;
  kr_adapter_setup setup;
  ret = path_setup(path);
  if (ret != 0) {
    printke("XPDR: path setup returned %d", ret);
    return ret;
  }
  switch (path->type) {
    case INVALID: return -1;
    case ADAPTER_CTX:
      setup.info = path->info;
      setup.event_cb = adapter_event;
      setup.user = path;
      printk("XPDR: adapter context create");
      path->adapter = adapters[path->adapter_type].open(&setup);
      if (path->adapter == NULL) return -1;
      return 0;
    case ADAPTER_LINK:
      printk("XPDR: adapter path create");
      path_io_create(path);
      path_io_create(path);
      return 0;
  }
  */
  return NULL;
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
  path_setup.adapter_path = path;
  path_setup.user = user;
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

int kr_xpdr_destroy(kr_xpdr *xpdr) {
  int i;
  int ret;
  kr_xpdr_path *path;
  if (xpdr == NULL) return -1;
  printk("XPDR: Destroying");
  kr_adapter_monitor_destroy(xpdr->adapter_mon);
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
  xpdr->adapter_mon = kr_adapter_monitor_create();
  kr_adapter_monitor_wait(xpdr->adapter_mon, 0);
  printk("XPDR: Created");
  return xpdr;
}
