#include "krad_pool.h"
#include "krad_sfx.h"
#include "krad_graph.h"
#include "krad_mixer.h"

typedef enum {
  KR_MXP_NIL,
  KR_MXP_READY,
  KR_MXP_ACTIVE,
  KR_MXP_TERM
} mixer_path_state;

struct kr_mixer {
  kr_graph *graph;
  kr_pool *path_pool;
  kr_mixer_event_cb *event_cb;
  void *user;
};

typedef struct {
  kr_mixer_path_info *info;
  void *control_user;
  void *audio_user;
  kr_audio_cb *audio_cb;
  kr_mixer_path *from;
  kr_mixer_path *to;
} mixer_path_setup;

struct kr_mixer_path {
  kr_mixer_path_type type;
  kr_mixer_channels channels;
  float *samples[KR_MXR_MAX_CHANNELS];
  int nframes;
  int sample_rate;
  mixer_path_state state;
  kr_audio_cb *audio_cb;
  void *audio_user;
  void *control_user;
  kr_mixer *mixer;
  kr_sfx *sfx;
  union {
    kr_graph_elem elem;
    void *ptr;
  } g;
  kr_mixer_path_info info;
};

static kr_vertex_type path_vertex_type(kr_mixer_path_type type);
static void limit_samples(float **samples, int nc, int ns);
static void clear_samples(float **dst, int nc, int ns);
static void copy_samples(float **dst, float **src, int nc, int ns);
static void sum_samples(float **dst, float **src, int nc, int ns);
static void transport(kr_mixer_path *path);
static void update_state(kr_mixer *mixer);
static void path_release(kr_mixer_path *path);
static int path_info_check(kr_mixer_path_info *info);
static int path_setup_check(mixer_path_setup *setup);
static void path_sfx_create(kr_mixer_path *path);
static kr_vertex_type path_vertex_type(kr_mixer_path_type type);
static int kr_mixer_process_path(kr_mixer_path *path);
static void path_setup(kr_mixer_path *path, mixer_path_setup *setup);
static kr_mixer_path *path_create(kr_mixer *mixer, mixer_path_setup *setup);

static kr_vertex_type path_vertex_type(kr_mixer_path_type type) {
  switch (type) {
    case KR_MXR_SOURCE:
      return KR_VERTEX_SOURCE;
    case KR_MXR_BUS:
      return KR_VERTEX_BUS;
    case KR_MXR_OUTPUT:
      return KR_VERTEX_OUTPUT;
    default:
      break;
  }
  return -1;
}

static void limit_samples(float **samples, int nc, int ns) {
  int c;
  for (c = 0; c < nc; c++) {
    krad_hardlimit(samples[c], ns);
  }
}

static void clear_samples(float **dst, int nc, int ns) {
  int s;
  int c;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] = 0.0f;
    }
  }
}

static void copy_samples(float **dst, float **src, int nc, int ns) {
  int s;
  int c;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] = src[c][s];
    }
  }
}

static void sum_samples(float **dst, float **src, int nc, int ns) {
  int c;
  int s;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] += src[c][s];
    }
  }
}

static void transport(kr_mixer_path *path) {
  kr_audio_event event;
  event.audio.channels = path->channels;
  if (path->type == KR_MXR_OUTPUT) {
    event.audio.count = path->nframes;
  }
  //FIXME
  event.audio.rate = 48000;
  event.user = path->audio_user;
  path->audio_cb(&event);
  if (path->type == KR_MXR_SOURCE) {
    path->nframes = event.audio.count;
    copy_samples(path->samples, event.audio.samples, path->channels, path->nframes);
  } else {
    copy_samples(event.audio.samples, path->samples, path->channels, path->nframes);
  }
}

static void update_state(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    switch (path->state) {
      case KR_MXP_READY:
        path->state = KR_MXP_ACTIVE;
        break;
      case KR_MXP_TERM:
        path_release(path);
        break;
      default:
        break;
    }
  }
}

static int kr_mixer_process_path(kr_mixer_path *path) {
  int i;
  int e;
  kr_mixer_path *streamer45;
  kr_mixer *mixer;
  i = 0;
  void *user[16];
  if (path == NULL) return -1;
  mixer = path->mixer;
  update_state(mixer);
  if (path->type == KR_MXR_SOURCE) {
    transport(path);
    kr_sfx_process(path->sfx, path->samples, path->samples, path->nframes);
    e = kr_graph_in_out_links(mixer->graph, &path->g.elem, -45, user, 16);
    for (i = 0; i < e; i++) {
      streamer45 = (kr_mixer_path *)user[i];
      copy_samples(streamer45->samples, path->samples, path->channels, path->nframes);
      streamer45->nframes = path->nframes;
      kr_sfx_process(streamer45->sfx, streamer45->samples, streamer45->samples, streamer45->nframes);
    }
    //  printk("source had %d edges", e);
    return 0;
  }
  /*
  if (path->type == KR_MXR_INPUT) {
    //copy_samples(path->to->samples, path->from->samples, path->channels, path->nframes);
    kr_sfx_process(path->sfx, path->samples, path->samples, path->nframes);
    return 0;
  }
  */
  if (path->type == KR_MXR_BUS) {
    //clear_samples(path->samples, path->channels, path->nframes);
    e = kr_graph_in_out_links(mixer->graph, &path->g.elem, -45, user, 16);
    for (i = 0; i < e; i++) {
      streamer45 = (kr_mixer_path *)user[i];
      path->nframes = streamer45->nframes;
      if (i == 0) {
        clear_samples(path->samples, path->channels, path->nframes);
      }
      sum_samples(path->samples, streamer45->samples, path->channels, path->nframes);
    }
    //  printk("bus had %d edges", e);
    kr_sfx_process(path->sfx, path->samples, path->samples, path->nframes);
    return 0;
  }
  if (path->type == KR_MXR_OUTPUT) {
    //clear_samples(path->samples, path->channels, path->nframes);
    e = kr_graph_in_out_links(mixer->graph, &path->g.elem, -45, user, 16);
    for (i = 0; i < e; i++) {
      streamer45 = (kr_mixer_path *)user[i];
      path->nframes = streamer45->nframes;
      if (i == 0) {
        clear_samples(path->samples, path->channels, path->nframes);
      }
      sum_samples(path->samples, streamer45->samples, path->channels, path->nframes);
    }
    //  printk("output had %d edges", e);
    kr_sfx_process(path->sfx, path->samples, path->samples, path->nframes);
    limit_samples(path->samples, path->channels, path->nframes);
    transport(path);
    return 0;
  }
  return -1;
}

static int kr_mixer_process_stupid(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->type == KR_MXR_SOURCE) {
      kr_mixer_process_path(path);
    }
  }
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->type == KR_MXR_INPUT) {
      kr_mixer_process_path(path);
    }
  }
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->type == KR_MXR_BUS) {
      kr_mixer_process_path(path);
    }
  }
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->type == KR_MXR_OUTPUT) {
      kr_mixer_process_path(path);
    }
  }
  return 0;
}

int kr_mixer_process(kr_mixer_path *path) {
  int ret;
  if (path == NULL) return -1;
  ret = kr_mixer_process_stupid(path->mixer);
  return ret;
}

static void path_release(kr_mixer_path *path) {
  int c;
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    free(path->samples[c]);
  }
  if (path->sfx != NULL) {
    kr_sfx_destroy(path->sfx);
    path->sfx = NULL;
  }
  path->state = KR_MXP_NIL;
  kr_graph_remove(path->mixer->graph, &path->g.elem);
  kr_pool_recycle(path->mixer->path_pool, path);
}

static int path_info_check(kr_mixer_path_info *info) {
  if (info->channels < 1) return -1;
  if (info->channels > KR_MXR_MAX_CHANNELS) return -1;
  if ((info->type != KR_MXR_SOURCE)
   && (info->type != KR_MXR_INPUT)
   && (info->type != KR_MXR_BUS)
   && (info->type != KR_MXR_OUTPUT)) {
    return -4;
  }
  /* FIXME check all the things
   * sfx params
   * */
  return 0;
}

static int path_setup_check(mixer_path_setup *setup) {
  if ((setup->info->type == KR_MXR_INPUT)
   || (setup->info->type == KR_MXR_BUS)) {
    if (setup->audio_cb != NULL) return -1;
    if (setup->audio_user != NULL) return -2;
  } else {
    if (setup->audio_cb == NULL) return -3;
    if (setup->audio_user == NULL) return -4;
  }
  if (setup->control_user == NULL) return -5;
  if (path_info_check(setup->info)) return -6;
  return 0;
}

static void path_sfx_create(kr_mixer_path *path) {
  kr_sfx_setup setup;
  kr_sfx_cmd cmd;
  setup.channels = path->channels;
  //FIXME
  setup.sample_rate = 48000;
  setup.user = path;
  setup.cb = NULL;
  /* FIXME actual sfx info callback
   * have sfx info callback hit path cb that propagates to path info cb
   * FIXME set sfx params from setup
   * */
  path->sfx = kr_sfx_create(&setup);
  cmd.control = KR_SFX_EFFECT_ADD;
  cmd.effect = KR_SFX_EQ;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_LOWPASS;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_HIGHPASS;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_ANALOG;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_VOLUME;
  kr_sfx_ctl(path->sfx, &cmd);
}

static void path_setup(kr_mixer_path *path, mixer_path_setup *setup) {
  int c;
  kr_mixer_event event;
  path->channels = setup->info->channels;
  path->type = setup->info->type;
  path->audio_cb = setup->audio_cb;
  path->audio_user = setup->audio_user;
  path->control_user = setup->control_user;
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    path->samples[c] = kr_allocz(1, KR_MXR_PERIOD_MAX * sizeof(float));
  }
  path_sfx_create(path);
  path->state = KR_MXP_READY;
  event.user = path->mixer->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_MIXER_CREATE;
  kr_mixer_path_info_get(path, &event.info);
  path->mixer->event_cb(&event);
  path->control_user = event.user_path;
}

static kr_mixer_path *path_create(kr_mixer *mixer, mixer_path_setup *setup) {
  int ret;
  kr_mixer_path *path;
  kr_vertex_type vertex_type;
  if ((mixer == NULL) || (setup == NULL)) return NULL;
  ret = path_setup_check(setup);
  if (ret) {
    printke("mixer mkpath failed setup check: %d", ret);
    return NULL;
  }
  path = kr_pool_slice(mixer->path_pool);
  if (path == NULL) {
    printke("mixer mkpath could not slice new path");
    return NULL;
  }
  path->mixer = mixer;
  path->g.ptr = NULL;
  if (setup->info->type == KR_MXR_INPUT) {
    path->g.elem = kr_graph_link(mixer->graph, &setup->to->g.elem, &setup->from->g.elem, path);
  } else {
    vertex_type = path_vertex_type(setup->info->type);
    path->g.elem = kr_graph_vertex(mixer->graph, vertex_type, path);
  }
  if (path->g.ptr == NULL) {
    kr_pool_recycle(mixer->path_pool, path);
    return NULL;
  }
  path_setup(path, setup);
  return path;
}

int kr_mixer_ctl(kr_mixer_path *path, kr_mixer_path_info_patch *patch) {
  int ret;
  if ((path == NULL) || (patch == NULL)) return -1;
  ret = kr_mixer_path_info_patch_apply(&path->info, patch);
  return ret;
}

int kr_mixer_path_info_get(kr_mixer_path *path, kr_mixer_path_info *info) {
  int i;
  kr_sfx_cmd cmd;
  if ((path == NULL) || (info == NULL)) return -1;
  info->channels = path->channels;
  info->type = path->type;
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
  }
  cmd.control = KR_SFX_GET_INFO;
  cmd.effect = KR_SFX_EQ;
  cmd.user = &info->eq;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_LOWPASS;
  cmd.user = &info->lowpass;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_HIGHPASS;
  cmd.user = &info->highpass;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_ANALOG;
  cmd.user = &info->analog;
  kr_sfx_ctl(path->sfx, &cmd);
  cmd.effect = KR_SFX_VOLUME;
  cmd.user = &info->volume;
  kr_sfx_ctl(path->sfx, &cmd);
  return 0;
}

int kr_mixer_remove(kr_mixer_path *path) {
  kr_mixer_event event;
  if (path == NULL) {
    printke("mixer path remove called with null value");
    return -1;
  }
  event.user = path->mixer->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_MIXER_DESTROY;
  memset(&event.info, 0, sizeof(event.info));
  path->mixer->event_cb(&event);
  path->state = KR_MXP_TERM;
  /* FIXME perhaps, if we are bus or bus input that is connected to
   * no inputs or outputs, we go away now, otherwise we wait for
   * the next path process to make us go away */
  update_state(path->mixer);
  return 0;
}

kr_mixer_path *kr_mixer_port(kr_mixer *mixer, kr_mixer_port_setup *setup) {
  mixer_path_setup path_setup;
  if ((mixer == NULL) || (setup == NULL)) return NULL;
  path_setup.info = &setup->info;
  path_setup.control_user = setup->control_user;
  path_setup.audio_user = setup->audio_user;
  path_setup.audio_cb = setup->audio_cb;
  return path_create(mixer, &path_setup);
}

int kr_mixer_bus(kr_mixer *mixer, kr_mixer_path_info *info, void *user) {
  kr_mixer_path *path;
  mixer_path_setup setup;
  if ((mixer == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = info;
  setup.control_user = user;
  setup.audio_user = NULL;
  setup.audio_cb = NULL;
  path = path_create(mixer, &setup);
  if (path == NULL) return -2;
  return 0;
}

int kr_mixer_link(kr_mixer *mixer, kr_mixer_path_info *info, kr_mixer_path *from, kr_mixer_path *to, void *user) {
  kr_mixer_path *path;
  mixer_path_setup setup;
  if ((mixer == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = info;
  setup.control_user = user;
  setup.audio_user = NULL;
  setup.audio_cb = NULL;
  setup.from = from;
  setup.to = to;
  path = path_create(mixer, &setup);
  if (path == NULL) return -2;
  return 0;
}

int kr_mixer_destroy(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  printk("Mixer: Destroying");
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->state != KR_MXP_NIL) {
      kr_mixer_remove(path);
    }
  }
  kr_graph_destroy(mixer->graph);
  kr_pool_destroy(mixer->path_pool);
  printk("Mixer: Destroyed");
  return 0;
}

kr_mixer *kr_mixer_create(kr_mixer_setup *setup) {
  kr_mixer *mixer;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_graph_setup graph_setup;
  if (setup == NULL) return NULL;
  printk("Mixer: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_mixer);
  pool_setup.size = sizeof(kr_mixer_path);
  pool_setup.slices = setup->path_count;
  pool = kr_pool_create(&pool_setup);
  mixer = kr_pool_overlay_get(pool);
  memset(mixer, 0, sizeof(kr_mixer));
  mixer->path_pool = pool;
  mixer->user = setup->user;
  mixer->event_cb = setup->event_cb;
  memset(&graph_setup, 0, sizeof(graph_setup));
  mixer->graph = kr_graph_create(&graph_setup);
  printk("Mixer: Created");
  return mixer;
}
