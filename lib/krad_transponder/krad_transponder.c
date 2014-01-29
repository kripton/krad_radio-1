#include "krad_transponder.h"
#include "krad_pool.h"
#include "krad_av.h"

#define ADP_TEMP_MAX 128

typedef union {
  void *exists;
  kr_mixer_path *mixer_path;
  kr_compositor_path *compositor_path;
  kr_adapter_path *adapter_path;
} kr_xpdr_path_io;

struct kr_transponder_path {
  kr_transponder_path_info info;
  kr_xpdr_path_io input;
  kr_xpdr_path_io output;
  kr_audio audio;
  kr_image image;
  void *user;
  kr_transponder *xpdr;
};

struct kr_transponder {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder_info info;
  kr_adapter *adapter[ADP_TEMP_MAX]; /*FIXME size*/
  kr_pool *path_pool;
  kr_adapter_monitor *adapter_mon;
  void *user;
  kr_transponder_event_cb *event_cb;
};

static kr_adapter *adapter_find(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static kr_adapter *adapter_create(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static kr_adapter *adapter_get(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static int path_io_info_check(kr_xpdr_path_io_info *info);
static int path_info_check(kr_xpdr_path_info *info);
static void path_io_create(kr_xpdr_path *path, kr_xpdr_path_io_info *info);
static int path_create(kr_xpdr_path *path);
static void path_io_destroy(kr_xpdr_path_io *io, kr_xpdr_path_io_type type);
static void path_destroy(kr_xpdr_path *path);

void xpdr_compositor_path_frame_cb(kr_compositor_path_frame_cb_arg *arg) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)arg->user;
  arg->image = path->image;
}

void xpdr_mixer_path_audio_cb(kr_mixer_path_audio_cb_arg *arg) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)arg->user;
  arg->audio = path->audio;
}

void xpdr_adapter_path_av_cb(kr_adapter_path_av_cb_arg *arg) {
  kr_xpdr_path *path;
  path = (kr_xpdr_path *)arg->user;
  path->audio = arg->audio;
  path->image = arg->image;
}

static void xpdr_adapter_path_event_cb(kr_adapter_path_event_cb_arg *arg) {
  printk("yay adapter path event!");
}

static void xpdr_adapter_event_cb(kr_adapter_event_cb_arg *arg) {
  kr_xpdr *xpdr;
  kr_transponder_path *path;
  kr_adapter_info info;
  int i;
  uint32_t ret;
  xpdr = (kr_xpdr *)arg->user;
  /* Created / destroyed? or is that on transponder ? */
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
  /* I think we should trigger all of the mixer ports that are connected
   * to this adapter path */
  int test = 0;
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
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
  if (info.api == KR_ADP_JACK) {
    printk("it was a jack proccess callback.. with %d outs", test);
  }
}

static kr_adapter *adapter_find(kr_xpdr *xpdr, kr_adapter_path_setup *ps) {
  int i;
  kr_adapter *adapter;
  kr_adapter_info info;
  adapter = NULL;
  /* V4L2 is single adapter path per adapter */
  if (ps->info.api == KR_ADP_V4L2) return NULL;
  //find adapter instance
  for (i = 0; i < ADP_TEMP_MAX; i++) {
    if (xpdr->adapter[i] != NULL) {
      adapter = xpdr->adapter[i];
      if (kr_adapter_get_info(adapter, &info)) {
        printke("We failed to get adapter info..");
        return NULL;
      }
      if (info.api == ps->info.api) {
        //compare instance string
        //&& (strncmp(xpdr->adapter))) {
        //if we find it return
        printk("Found adapter");
        return adapter;
      } else {
        printk("we skipped an adapter with the wrong api..");
      }
    }
  }
  return NULL;
}

static kr_adapter *adapter_create(kr_xpdr *xpdr, kr_adapter_path_setup *ps) {
  int i;
  kr_adapter_setup adapter_setup;
  memset(&adapter_setup, 0, sizeof(kr_adapter_setup));
  adapter_setup.info.api = ps->info.api;
  /* Needed: adapter info from adapter path info */
  if (ps->info.api == KR_ADP_V4L2) {
    adapter_setup.info.api_info.v4l2.dev = ps->info.info.v4l2.dev;
  }
  for (i = 0; i < ADP_TEMP_MAX; i++) {
    if (xpdr->adapter[i] == NULL) {
      adapter_setup.user = xpdr;
      adapter_setup.ev_cb = xpdr_adapter_event_cb;
      xpdr->adapter[i] = kr_adapter_create(&adapter_setup);
      printk("Created adapter");
      return xpdr->adapter[i];
    }
  }
  printke("cant create adapter, out of mem");
  return NULL;
}

static kr_adapter *adapter_get(kr_xpdr *xpdr, kr_adapter_path_setup *ps) {
  kr_adapter *adapter;
  adapter = adapter_find(xpdr, ps);
  if (!adapter) {
    adapter = adapter_create(xpdr, ps);
  }
  return adapter;
}

static int path_io_info_check(kr_transponder_path_io_info *info) {
  if ((info->type != KR_XPDR_ADAPTER)
   && (info->type != KR_XPDR_MIXER)
   && (info->type != KR_XPDR_COMPOSITOR)) {
    return -1;
  }
  /* FIXME check stuff */
  switch (info->type) {
    case KR_XPDR_ADAPTER:
      break;
    case KR_XPDR_MIXER:
      break;
    case KR_XPDR_COMPOSITOR:
      break;
  }
  return 0;
}

static int path_info_check(kr_xpdr_path_info *info) {
  if (((info->input.type == KR_XPDR_MIXER)
      && (info->output.type == KR_XPDR_COMPOSITOR))
      || ((info->input.type == KR_XPDR_COMPOSITOR)
      && (info->output.type == KR_XPDR_MIXER))
      || ((info->input.type == KR_XPDR_ADAPTER)
      && (info->output.type == KR_XPDR_ADAPTER))) {
    return -4;
  }
  if (path_io_info_check(&info->input)) return -5;
  if (path_io_info_check(&info->output)) return -6;
  return 0;
}

static void path_io_destroy(kr_xpdr_path_io *io, kr_xpdr_path_io_type type) {
  switch (type) {
    case KR_XPDR_ADAPTER:
      kr_adapter_unlink(io->adapter_path);
      break;
    case KR_XPDR_MIXER:
      kr_mixer_unlink(io->mixer_path);
      break;
    case KR_XPDR_COMPOSITOR:
      kr_compositor_unlink(io->compositor_path);
      break;
  }
}

static void path_io_create(kr_xpdr_path *path, kr_xpdr_path_io_info *info) {
  kr_xpdr_path_io *io;
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_adapter *adapter;
  kr_mixer_io_path_setup mp_setup;
  kr_compositor_io_path_setup cp_setup;
  kr_adapter_path_setup ap_setup;
  mixer = path->xpdr->mixer;
  compositor = path->xpdr->compositor;
  if (path->output.exists) {
    io = &path->input;
  } else {
    io = &path->output;
  }
  switch (info->type) {
    case KR_XPDR_ADAPTER:
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
      adapter = adapter_get(path->xpdr, &ap_setup);
      if (adapter == NULL) {
        printke("get_adapter returned NULL");
      } else {
        io->adapter_path = kr_adapter_mkpath(adapter, &ap_setup);
        if (io->adapter_path == NULL) {
          printke("adapter mkpath returned NULL");
        }
      }
      break;
    case KR_XPDR_MIXER:
      memcpy(&mp_setup.info, &info->info.mixer_path_info,
       sizeof(kr_mixer_path_info));
      mp_setup.audio_cb = xpdr_mixer_path_audio_cb;
      mp_setup.audio_user = path;
      mp_setup.control_user = path->user;
      io->mixer_path = kr_mixer_mkso(mixer, &mp_setup);
      if (io->mixer_path == NULL) {
        printke("mixer mkpath returned NULL");
      }
      break;
    case KR_XPDR_COMPOSITOR:
      memcpy(&cp_setup.info, &info->info.compositor_path_info,
       sizeof(kr_compositor_path_info));
      cp_setup.frame_cb = xpdr_compositor_path_frame_cb;
      cp_setup.frame_user = path;
      cp_setup.control_user = path->user;
      io->compositor_path = kr_compositor_mkio(compositor, &cp_setup);
      if (io->compositor_path == NULL) {
        printke("compositor mkpath returned NULL");
      }
      break;
  }
}

static void path_destroy(kr_xpdr_path *path) {
  path_io_destroy(&path->input, path->info.input.type);
  path_io_destroy(&path->output, path->info.output.type);
}

static int path_create(kr_xpdr_path *path) {
  path_io_create(path, &path->info.output);
  path_io_create(path, &path->info.input);
  return 0;
}

int kr_transponder_path_ctl(kr_xpdr_path *path, kr_xpdr_path_patch *patch) {
  return -1;
}

int kr_transponder_unlink(kr_xpdr_path *path) {
  kr_transponder *xpdr;
  if (path == NULL) return -1;
  xpdr = path->xpdr;
  path_destroy(path);
  xpdr->info.active_paths--;
  kr_pool_recycle(xpdr->path_pool, path);
  /* do event callback */
  return -1;
}

int kr_transponder_mkpath(kr_xpdr *x, kr_xpdr_path_info *i, void *p) {
  int ret;
  kr_transponder_path *path;
  if ((x == NULL) || (i == NULL)) return -1;
  if (path_info_check(i)) return -2;
  path = kr_pool_slice(x->path_pool);
  if (path == NULL) return -3;
  memcpy(&path->info, i, sizeof(kr_transponder_path_info));
  path->xpdr = x;
  path->user = p;
  ret = path_create(path);
  if (ret) return -4;
  x->info.active_paths++;
  /* do event callback */
  return 0;
}

int kr_transponder_destroy(kr_transponder *xpdr) {
  int i;
  int ret;
  kr_transponder_path *path;
  if (xpdr == NULL) return -1;
  printk("Transponder: Destroying");
  kr_adapter_monitor_destroy(xpdr->adapter_mon);
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    ret = kr_transponder_unlink(path);
    if (ret) printke("trouble unlinking an path..");
  }
  for (i = 0; i < ADP_TEMP_MAX; i++) {
    if (xpdr->adapter[i] != NULL) {
      ret = kr_adapter_destroy(xpdr->adapter[i]);
      if (ret) printke("trouble unlinking an adapter..");
      xpdr->adapter[i] = NULL;
    }
  }
  kr_pool_destroy(xpdr->path_pool);
  printk("Transponder: Destroyed");
  return 0;
}

kr_transponder *kr_transponder_create(kr_transponder_setup *setup) {
  kr_transponder *xpdr;
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
  xpdr = kr_pool_overlay_get(pool);
  memset(xpdr, 0, sizeof(kr_transponder));
  xpdr->path_pool = pool;
  xpdr->mixer = setup->mixer;
  xpdr->compositor = setup->compositor;
  xpdr->adapter_mon = kr_adapter_monitor_create();
  kr_adapter_monitor_wait(xpdr->adapter_mon, 0);
  printk("Transponder: Created");
  return xpdr;
}
