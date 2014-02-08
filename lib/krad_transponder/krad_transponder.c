#include "krad_adapter_monitor.h"
#include "krad_transponder.h"
#include "krad_pool.h"
#include "krad_av.h"
#include "krad_compositor_so_path.h"

#include "krad_jack.h"
#include "krad_wayland.h"
#include "krad_v4l2.h"
#include "krad_decklink.h"
#include "krad_x11.h"
#include "krad_alsa.h"

typedef enum {
  INVALID = -1,
  ADAPTER,
  ADAPTER_PATH
} path_type;

typedef enum {
  KR_XPDR_MIXER,
  KR_XPDR_COMPOSITOR,
  KR_XPDR_ADAPTER
} path_io_type;

typedef struct {
  int coconut;
} kr_adapter;

typedef struct {
  int coconut;
} kr_adapter_path;

typedef union {
  void *exists;
  kr_mixer_path *mixer_path;
  kr_compositor_path *compositor_path;
  kr_adapter_path *adapter_path;
} path_io;

struct kr_transponder_path {
  kr_transponder_path_info info;
  path_type type;
  kr_adapter *adapter;
  path_io_type input_type;
  path_io input;
  path_io_type output_type;
  path_io output;
  kr_audio audio;
  kr_image image;
  void *user;
  kr_transponder *transponder;
};

struct kr_transponder {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_pool *path_pool;
  kr_adapter_monitor *adapter_mon;
  void *user;
  kr_transponder_event_cb *event_cb;
};

static void path_io_destroy(path_io *io, path_io_type type);
static void path_io_create(kr_transponder_path *path);
static int path_setup(kr_transponder_path *info);
static void path_destroy(kr_transponder_path *path);
static int path_create(kr_transponder_path *path);

void compositor_frame_cb(kr_compositor_path_frame_cb_arg *arg) {
  kr_transponder_path *path;
  path = (kr_transponder_path *)arg->user;
  arg->image = path->image;
}

void mixer_audio_cb(kr_mixer_path_audio_cb_arg *arg) {
  kr_transponder_path *path;
  path = (kr_transponder_path *)arg->user;
  arg->audio = path->audio;
}

/*
void transponder_adapter_path_av_cb(kr_adapter_path_av_cb_arg *arg) {
  kr_transponder_path *path;
  path = (kr_transponder_path *)arg->user;
  path->audio = arg->audio;
  path->image = arg->image;
}

static void transponder_adapter_path_event_cb(kr_adapter_path_event_cb_arg *arg) {
  printk("yay adapter path event!");
}

static void transponder_adapter_event_cb(kr_adapter_event_cb_arg *arg) {
  kr_transponder *transponder;
  kr_transponder_path *path;
  kr_adapter_info info;
  int i;
  transponder = (kr_transponder *)arg->user;
  *//* Created / destroyed? or is that on transponder ? *//*
  if (arg->type == KR_ADAPTER_DISCONNECTED) {
    printk("Adapter Disconnected");
    return;
  }
  if (arg->type == KR_ADAPTER_RECONNECTED) {
    printk("Adapter Reconnected");
    return;
  }
  if (arg->type != KR_ADAPTER_PROCESS) {
    printk("Wasn't a process event");
    return;
  }
  kr_adapter_get_info(arg->adapter, &info);
  *//* I think we should trigger all of the mixer ports that are connected
   * to this adapter path *//*
  int test = 0;
  i = 0;
  while ((path = kr_pool_iterate_active(transponder->path_pool, &i))) {
    if (path->info.output.type == KR_XPDR_ADAPTER) {
      if (path->info.output.type == KR_XPDR_COMPOSITOR) {
        kr_compositor_process(path->input.compositor_path);
      }
      if (path->info.input.type == KR_XPDR_MIXER) {
        kr_mixer_process(path->input.mixer_path);
      }
      test++;
    }
  }
}
*/

static void path_io_destroy(path_io *io, path_io_type type) {
  switch (type) {
    case KR_XPDR_ADAPTER:
      //kr_adapter_unlink(io->adapter_path);
      break;
    case KR_XPDR_MIXER:
      kr_mixer_unlink(io->mixer_path);
      break;
    case KR_XPDR_COMPOSITOR:
      kr_compositor_unlink(io->compositor_path);
      break;
  }
}

static void path_io_create(kr_transponder_path *path) {
  path_io *io;
  path_io_type type;
  kr_mixer *mixer;
  kr_compositor *compositor;
  /*kr_adapter *adapter;*/
  kr_mixer_io_path_setup mp_setup;
  kr_compositor_io_path_setup cp_setup;
  mixer = path->transponder->mixer;
  compositor = path->transponder->compositor;
  if (path->output.exists) {
    type = path->input_type;
    io = &path->input;
  } else {
    type = path->output_type;
    io = &path->output;
  }
  switch (type) {
    case KR_XPDR_ADAPTER:
      /*
      memcpy(&ap_setup.info, &info->info.adapter_path_info,
       sizeof(kr_adapter_path_info));
      if (io == &path->input) {
        ap_setup.info.dir = KR_ADP_PATH_INPUT;
      } else {
        ap_setup.info.dir = KR_ADP_PATH_OUTPUT;
      }
      ap_setup.ev_cb = transponder_adapter_path_event_cb;
      ap_setup.av_cb = transponder_adapter_path_av_cb;
      ap_setup.user = path;
      //adapter = adapter_get(path->transponder, &ap_setup);
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
    case KR_XPDR_MIXER:
      /* mp_setup.info setup srate?channels? FIXME */
      mp_setup.audio_cb = mixer_audio_cb;
      mp_setup.audio_user = path;
      mp_setup.control_user = path->user;
      io->mixer_path = kr_mixer_mkso(mixer, &mp_setup);
      if (io->mixer_path == NULL) {
        printke("mixer mkpath returned NULL");
      }
      break;
    case KR_XPDR_COMPOSITOR:
      /* cp_setup.info setup frame size? FIXME */
      cp_setup.frame_cb = compositor_frame_cb;
      cp_setup.frame_user = path;
      cp_setup.control_user = path->user;
      io->compositor_path = kr_compositor_mkso(compositor, &cp_setup);
      if (io->compositor_path == NULL) {
        printke("compositor mkpath returned NULL");
      }
      break;
  }
}

static int path_setup(kr_transponder_path *path) {
  switch (path->info.type) {
    case KR_XR_JACK:
    case KR_XR_WAYLAND:
    case KR_XR_V4L2:
    case KR_XR_X11:
    case KR_XR_ALSA:
    case KR_XR_KRAD:
    case KR_XR_DECKLINK:
      path->type = ADAPTER;
      break;
    case KR_XR_DECKLINK_INPUT: /* FIXME these two */
    case KR_XR_KRAD_INPUT:
    case KR_XR_X11_INPUT:
    case KR_XR_V4L2_INPUT:
      path->input_type = KR_XPDR_ADAPTER;
      path->output_type = KR_XPDR_COMPOSITOR;
      path->type = ADAPTER_PATH;
      return 0;
    case KR_XR_ALSA_INPUT:
    case KR_XR_JACK_INPUT:
      path->input_type = KR_XPDR_ADAPTER;
      path->output_type = KR_XPDR_MIXER;
      path->type = ADAPTER_PATH;
      return 0;
    case KR_XR_JACK_OUTPUT:
    case KR_XR_ALSA_OUTPUT:
    case KR_XR_KRAD_OUTPUT:
      path->input_type = KR_XPDR_MIXER;
      path->output_type = KR_XPDR_ADAPTER;
      path->type = ADAPTER_PATH;
    case KR_XR_WAYLAND_OUTPUT:
      path->input_type = KR_XPDR_COMPOSITOR;
      path->output_type = KR_XPDR_ADAPTER;
      path->type = ADAPTER_PATH;
      return 0;
    default:
      return -1;
  }
  return -1;
}

static void path_destroy(kr_transponder_path *path) {
  if (path->type == ADAPTER) {
    /* kr_adapter_destroy(path->adapter); */
  } else {
    path_io_destroy(&path->input, path->input_type);
    path_io_destroy(&path->output, path->output_type);
  }
}

static int path_create(kr_transponder_path *path) {
  int ret;
  ret = path_setup(path);
  if (ret != 0) return ret;
  switch (path->type) {
    case INVALID: return -1;
    case ADAPTER:
      //path->adapter = kr_adapter_create();
      if (path->adapter == NULL) return -1;
      return 0;
    case ADAPTER_PATH:
      path_io_create(path);
      path_io_create(path);
      return 0;
  }
  return -1;
}

int kr_transponder_path_ctl(kr_transponder_path *path, kr_transponder_path_info_patch *patch) {
  int ret;
  if ((path == NULL) || (patch == NULL)) return -1;
  ret = kr_transponder_path_info_patch_apply(&path->info, patch);
  return ret;
}

int kr_transponder_unlink(kr_transponder_path *path) {
  kr_transponder *transponder;
  kr_transponder_event event;
  if (path == NULL) return -1;
  transponder = path->transponder;
  path_destroy(path);
  event.user = path->transponder->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_DESTROY;
  event.info = path->info;
  path->transponder->event_cb(&event);
  kr_pool_recycle(transponder->path_pool, path);
  return -1;
}

int kr_transponder_mkpath(kr_transponder *x, kr_transponder_path_info *i, void *p) {
  int ret;
  kr_transponder_path *path;
  kr_transponder_event event;
  if ((x == NULL) || (i == NULL)) return -1;
  /*if (path_info_check(i)) return -2;*/
  path = kr_pool_slice(x->path_pool);
  if (path == NULL) return -3;
  memcpy(&path->info, i, sizeof(kr_transponder_path_info));
  path->transponder = x;
  path->user = p;
  ret = path_create(path);
  if (ret) return -4;
  event.user = path->transponder->user;
  event.user_path = path->user;
  event.path = path;
  event.type = KR_XPDR_CREATE;
  event.info = path->info;
  path->transponder->event_cb(&event);
  path->user = event.user_path;
  return 0;
}

int kr_transponder_destroy(kr_transponder *transponder) {
  int i;
  int ret;
  kr_transponder_path *path;
  if (transponder == NULL) return -1;
  printk("Transponder: Destroying");
  kr_adapter_monitor_destroy(transponder->adapter_mon);
  i = 0;
  while ((path = kr_pool_iterate_active(transponder->path_pool, &i))) {
    ret = kr_transponder_unlink(path);
    if (ret) printke("trouble unlinking an path..");
  }
  kr_pool_destroy(transponder->path_pool);
  printk("Transponder: Destroyed");
  return 0;
}

kr_transponder *kr_transponder_create(kr_transponder_setup *setup) {
  kr_transponder *transponder;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (setup == NULL) return NULL;
  printk("Transponder: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_transponder);
  pool_setup.size = sizeof(kr_transponder_path);
  pool_setup.slices = setup->path_count;
  pool = kr_pool_create(&pool_setup);
  transponder = kr_pool_overlay_get(pool);
  memset(transponder, 0, sizeof(kr_transponder));
  transponder->path_pool = pool;
  transponder->mixer = setup->mixer;
  transponder->compositor = setup->compositor;
  transponder->user = setup->user;
  transponder->event_cb = setup->event_cb;
  transponder->adapter_mon = kr_adapter_monitor_create();
  kr_adapter_monitor_wait(transponder->adapter_mon, 0);
  printk("Transponder: Created");
  return transponder;
}
