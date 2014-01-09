#include "krad_transponder.h"
#include "krad_av.h"

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
  kr_adapter *adapter[KR_XPDR_PATHS_MAX * 2];
  kr_transponder_path *path[KR_XPDR_PATHS_MAX];
  kr_adapter_monitor *adapter_mon;
  void *user;
  kr_transponder_event_cb *event_cb;
};

#include "krad_transponder_processor.c"

static kr_adapter *adapter_find(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static kr_adapter *adapter_create(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static kr_adapter *adapter_get(kr_xpdr *xpdr, kr_adapter_path_setup *ps);
static int path_io_info_check(kr_xpdr_path_io_info *info);
static int path_info_check(kr_xpdr_path_info *info);
static void path_io_create(kr_xpdr_path *path, kr_xpdr_path_io_info *info);
static int path_create(kr_xpdr_path *path, kr_xpdr_path_info *info, void *p);
static void path_io_destroy(kr_xpdr_path_io *io, kr_xpdr_path_io_type type);
static void path_destroy(kr_xpdr_path *path);

static void xpdr_adapter_path_event_cb(kr_adapter_path_event_cb_arg *arg) {
  printk("yay adapter path event!");
}

static void xpdr_adapter_event_cb(kr_adapter_event_cb_arg *arg) {
  kr_xpdr *xpdr;
  kr_adapter_info info;
  uint32_t ret;
  xpdr = (kr_xpdr *)arg->user;
  /* if we are the audio or video clock do a process .. */
  if (kr_adapter_prepare(arg->adapter)) {
    /* FIXME FIXME FIXME need to reconcile period size */
    if (kr_adapter_get_info(arg->adapter, &info)) {
      printke("We failed to get adapter info..");
      return;
    }
    if (info.api == KR_ADP_JACK) {
      ret = kr_mixer_process(xpdr->mixer);
      if (ret > 0) {
        /* printk("mixed %u frames", ret); */
        /* number of frames mixed, so time can be dealt with */
      }
    }
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
  for (i = 0; i < KR_XPDR_PATHS_MAX * 2; i++) {
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
  for (i = 0; i < KR_XPDR_PATHS_MAX; i++) {
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
      mp_setup.user = path;
      io->mixer_path = kr_mixer_mkio(mixer, &mp_setup);
      if (io->mixer_path == NULL) {
        printke("mixer mkpath returned NULL");
      }
      break;
    case KR_XPDR_COMPOSITOR:
      memcpy(&cp_setup.info, &info->info.compositor_path_info,
       sizeof(kr_compositor_path_info));
      cp_setup.frame_cb = xpdr_compositor_path_frame_cb;
      cp_setup.user = path;
      io->compositor_path = kr_compositor_mkio(compositor, &cp_setup);
      if (io->compositor_path == NULL) {
        printke("compositor mkpath returned NULL");
      }
      break;
  }
}

static int path_create(kr_xpdr_path *path, kr_xpdr_path_info *info, void *p) {
  memcpy(&path->info, info, sizeof(kr_transponder_path_info));
  path->user = p;
  path_io_create(path, &path->info.output);
  /* FIXME hrmmm i think we need to create both and if in failed state owell*/
  if (path->output.exists != NULL) {
    path_io_create(path, &path->info.input);
  }
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

static void path_destroy(kr_xpdr_path *path) {
  if (path->input.exists != NULL) {
    path_io_destroy(&path->input, path->info.input.type);
  }
  if (path->output.exists != NULL) {
    path_io_destroy(&path->output, path->info.output.type);
  }
}

static kr_xpdr_path *path_alloc(kr_transponder *xpdr) {
  int i;
  for (i = 0; i < KR_XPDR_PATHS_MAX; i++) {
    if (xpdr->path[i] == NULL) {
      xpdr->path[i] = calloc(1, sizeof(kr_transponder_path));
      xpdr->path[i]->xpdr = xpdr;
      return xpdr->path[i];
    }
  }
  return NULL;
}

int kr_transponder_path_ctl(kr_xpdr_path *path, kr_xpdr_path_patch *patch) {
  return -1;
}

kr_xpdr_path *kr_transponder_mkpath(kr_xpdr *x, kr_xpdr_path_info *i, void *p) {
  int ret;
  kr_transponder_path *path;
  if ((x == NULL) || (i == NULL)) return NULL;
  if (path_info_check(i)) return NULL;
  path = path_alloc(x);
  if (path == NULL) return NULL;
  ret = path_create(path, i, p);
  if (ret) return NULL;
  x->info.active_paths++;
  return path;
}

int kr_transponder_unlink(kr_xpdr_path *path) {
  int i;
  kr_transponder *xpdr;
  if (path == NULL) return -1;
  xpdr = path->xpdr;
  for (i = 0; i < KR_XPDR_PATHS_MAX; i++) {
    if (xpdr->path[i] == path) {
      path_destroy(path);
      free(path);
      xpdr->path[i] = NULL;
      xpdr->info.active_paths--;
      return 0;
    }
  }
  return -1;
}

int kr_transponder_destroy(kr_transponder *xpdr) {
  int i;
  int ret;
  if (xpdr == NULL) return -1;
  printk("Krad Transponder: Destroy Started");
  kr_adapter_monitor_destroy(xpdr->adapter_mon);
  for (i = 0; i < KR_XPDR_PATHS_MAX; i++) {
    if (xpdr->path[i] != NULL) {
      ret = kr_transponder_unlink(xpdr->path[i]);
      if (ret) printke("trouble unlinking an path..");
    }
  }
  for (i = 0; i < KR_XPDR_PATHS_MAX * 2; i++) {
    if (xpdr->adapter[i] != NULL) {
      ret = kr_adapter_destroy(xpdr->adapter[i]);
      if (ret) printke("trouble unlinking an adapter..");
      xpdr->adapter[i] = NULL;
    }
  }
  free(xpdr);
  printk("Krad Transponder: Destroy Completed");
  return 0;
}

kr_transponder *kr_transponder_create(kr_transponder_setup *setup) {
  kr_transponder *xpdr;
  if (setup == NULL) return NULL;
  xpdr = calloc(1, sizeof(kr_transponder));
  xpdr->mixer = setup->mixer;
  xpdr->compositor = setup->compositor;
  xpdr->adapter_mon = kr_adapter_monitor_create();
  kr_adapter_monitor_wait(xpdr->adapter_mon, 0);
  return xpdr;
}
