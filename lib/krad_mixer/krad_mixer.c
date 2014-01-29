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
  kr_mixer_path_audio_cb *audio_cb;
  kr_mixer_path *from;
  kr_mixer_path *to;
} kr_mixer_path_setup;

struct kr_mixer_path {
  kr_mixer_path_type type;
  kr_mixer_channels channels;
  kr_mixer_path *from;
  kr_mixer_path *to;
  float *samples[KR_MXR_MAX_CHANNELS];
  mixer_path_state state;
  kr_mixer_path_audio_cb *audio_cb;
  void *audio_user;
  void *control_user;
  kr_mixer *mixer;
  kr_sfx *sfx;
  kr_vertex *vertex;
};

static void apply_effects(kr_mixer_path *path, int nframes);
static void clear_frames(kr_mixer_path *path, uint32_t nframes);
static void import_frames(kr_mixer_path *dest, kr_audio *src);
static void export_frames(kr_audio *dest, kr_mixer_path *src);
static void transport(kr_mixer_path *path, uint32_t nframes);
static void limit(kr_mixer_path *path, uint32_t nframes);
static void copy_samples(float **dst, float **src, int nc, int ns);
static void copy_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n);
static void mix_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n);
static void update_state(kr_mixer *mixer);
static void path_release(kr_mixer_path *path);
static int path_info_check(kr_mixer_path_info *info);
static int path_setup_check(kr_mixer_path_setup *setup);
static void path_sfx_create(kr_mixer_path *path);
static void path_setup(kr_mixer_path *path, kr_mixer_path_setup *setup);
static kr_mixer_path *path_create(kr_mixer *mixer, kr_mixer_path_setup *setup);

static void apply_effects(kr_mixer_path *port, int nframes) {
  // FIXME hrm we count on thems being the same btw in them effects lookout
  kr_sfx_process(port->sfx, port->samples, port->samples, nframes);
}

static void clear_frames(kr_mixer_path *path, uint32_t nframes) {
  int c;
  int s;
  for (c = 0; c < path->channels; c++) {
    for (s = 0; s < nframes; s++) {
      path->samples[c][s] = 0.0f;
    }
  }
}

static void import_frames(kr_mixer_path *dest, kr_audio *src) {
  int s;
  int c;
  int frames;
  int channels;
  //frames = MIN(dest->mixer->period_size, src->count);
  /* hrm FIXME */
  frames = src->count;
  channels = MIN(dest->channels, src->channels);
  for (c = 0; c < channels; c++) {
    for (s = 0; s < frames; s++) {
      dest->samples[c][s] = src->samples[c][s];
    }
  }
}

static void export_frames(kr_audio *dest, kr_mixer_path *src) {
  int s;
  int c;
  int frames;
  int channels;
  frames = dest->count;
  channels = dest->channels;
  for (c = 0; c < channels; c++) {
    for (s = 0; s < frames; s++) {
      dest->samples[c][s] = src->samples[c][s];
    }
  }
}

static void transport(kr_mixer_path *path, uint32_t nframes) {
  kr_mixer_path_audio_cb_arg cb_arg;
  cb_arg.audio.channels = path->channels;
  cb_arg.audio.count = nframes;
  //FIXME
  cb_arg.audio.rate = 48000;
  cb_arg.user = path->audio_user;
  path->audio_cb(&cb_arg);
  if (path->type == KR_MXR_INPUT) {
    import_frames(path, &cb_arg.audio);
  } else {
    export_frames(&cb_arg.audio, path);
  }
}

static void limit(kr_mixer_path *path, uint32_t nframes) {
  int c;
  for (c = 0; c < path->channels; c++) {
    krad_hardlimit(path->samples[c], nframes);
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

static void copy_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n) {
  int s;
  int c;
  for (c = 0; c < dest->channels; c++) {
    for (s = 0; s < n; s++) {
      dest->samples[c][s] = src->samples[c][s];
    }
  }
}

static void mix_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n) {
  int c;
  int s;
  if (dest->channels == src->channels) {
    for (c = 0; c < dest->channels; c++) {
      for (s = 0; s < n; s++) {
        dest->samples[c][s] += src->samples[c][s];
      }
    }
  }
}

static void update_state(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  kr_sfx_cmd cmd;
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

int kr_mixer_process(kr_mixer_path *path) {
  int i;
  int nframes;
  //int bi;
  kr_mixer *mixer;
  //kr_mixer_path *bus;
  i = 0;
  //bi = 0;
  return 0;
  if (path == NULL) return -1;
  mixer = path->mixer;
  update_state(mixer);
  nframes = 32; /*TEMP */
  /* Pull input audio */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_INPUT)) {
      transport(path, nframes);
    }
  }
  /* Process input and compute metrics */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_INPUT)) {
      apply_effects(path, nframes);
      //compute_meters(path, nframes);
    }
  }
  /* Mix, process and compute metrics */
  /*
  while ((bus = kr_pool_iterate_active(mixer->path_pool, &bi))) {
    if ((bus->state) && (bus->type == KR_MXR_BUS)) {
      while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
        if ((path->state == KR_MXP_ACTIVE) && (path->bus == bus)
         && (path->type == KR_MXR_INPUT)) {
          mix_frames(bus, path, nframes);
        }
      }
    }
  }
  */
  /* Copy output and process */
  /*
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE)
     && (path->type == KR_MXR_OUTPUT) && (path->bus != NULL)) {
      copy_frames(path, path->bus, nframes);
      limit(path, nframes);
    }
  }
  */
  /* Push output */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_OUTPUT)) {
      transport(path, nframes);
    }
  }
  /* Compute metrics */
  /*
  compute_meters(mixer->..., nframes);
  mixer->frames_since_peak_read += nframes;
  if (mixer->frames_since_peak_read >= mixer->frames_per_peak_broadcast) {
    mixer->frames_since_peak_read = 0;
    while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
      if (path->state == KR_MXP_ACTIVE) {
        //update_meter_readings(path);
      }
    }
  }
  */
  /* Clear mixes for next cycle */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_BUS)) {
      clear_frames(path, nframes);
    }
  }
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
  if (path->type == KR_MXR_INPUT) {
    kr_graph_edge_destroy(path->mixer->graph, path->to->vertex, path->from->vertex);
  } else {
    kr_graph_vertex_destroy(path->mixer->graph, path->vertex);
    path->vertex = NULL;
  }
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

static int path_setup_check(kr_mixer_path_setup *setup) {
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

static void path_setup(kr_mixer_path *path, kr_mixer_path_setup *setup) {
  int c;
  kr_mixer_event event;
  path->channels = setup->info->channels;
  path->type = setup->info->type;
  path->audio_cb = setup->audio_cb;
  path->audio_user = setup->audio_user;
  path->control_user = setup->control_user;
  if (path->type == KR_MXR_INPUT) {
    path->from = setup->from;
    path->to = setup->to;
  } else {
    path->from = NULL;
    path->to = NULL;
  }
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
  if (path->type == KR_MXR_INPUT) {
    kr_graph_edge_create(path->mixer->graph, path->to->vertex, path->from->vertex, path);
  } else {
    path->vertex = kr_graph_vertex_create(path->mixer->graph, setup->info->type, path);
  }
  path->mixer->event_cb(&event);
}

static kr_mixer_path *path_create(kr_mixer *mixer, kr_mixer_path_setup *setup) {
  int ret;
  kr_mixer_path *path;
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
  path_setup(path, setup);
  return path;
}

int kr_mixer_path_ctl(kr_mixer_path *path, kr_mixer_path_patch *patch) {
  int duration;
  if (path == NULL) return -1;
  return -2;
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

int kr_mixer_unlink(kr_mixer_path *path) {
  kr_mixer_event event;
  if (path == NULL) {
    printke("mixer path unlink called with null value");
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

kr_mixer_path *kr_mixer_mkso(kr_mixer *mixer, kr_mixer_io_path_setup *setup) {
  kr_mixer_path_setup path_setup;
  if ((mixer == NULL) || (setup == NULL)) return NULL;
  path_setup.info = &setup->info;
  path_setup.control_user = setup->control_user;
  path_setup.audio_user = setup->audio_user;
  path_setup.audio_cb = setup->audio_cb;
  return path_create(mixer, &path_setup);
}

int kr_mixer_mkbus(kr_mixer *mixer, kr_mixer_path_info *info, void *user) {
  kr_mixer_path *path;
  kr_mixer_path_setup setup;
  if ((mixer == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = info;
  setup.control_user = user;
  setup.audio_user = NULL;
  setup.audio_cb = NULL;
  path = path_create(mixer, &setup);
  if (path == NULL) return -2;
  return 0;
}

int kr_mixer_splice(kr_mixer *mixer, kr_mixer_path_info *info, kr_mixer_path *from, kr_mixer_path *to, void *user) {
  kr_mixer_path *path;
  kr_mixer_path_setup setup;
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
      kr_mixer_unlink(path);
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
