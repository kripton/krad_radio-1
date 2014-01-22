#include "krad_mixer.h"

typedef struct kr_mixer_io_path_setup kr_mixer_path_setup;

typedef enum {
  KR_MXP_NIL,
  KR_MXP_READY,
  KR_MXP_ACTIVE,
  KR_MXP_TERM
} mixer_path_state;

struct kr_mixer {
  uint32_t period_size;
  uint32_t new_period_size;
  uint32_t sample_rate;
  uint32_t new_sample_rate;
  int avg_window_size;
  uint32_t path_count;
  kr_pool *path_pool;
  kr_mixer_crossfader *crossfader;
  int frames_since_peak_read;
  int frames_per_peak_broadcast;
  kr_mixer_event_cb *event_cb;
  void *user;
  void *clock;
  kr_mixer_bus *master;
  kr_graph *mixer_graph;
};

struct kr_mixer_crossfader {
  kr_mixer_path *path[2];
  float fade;
  kr_easer easer;
};

struct kr_mixer_path {
  kr_mixer_path_type type;
  kr_mixer_bus *bus;
  kr_mixer_channels channels;
  kr_mixer_crossfader *crossfader;
  kr_easer volume_easer;
  int map[KR_MXR_MAX_CHANNELS];
  int mixmap[KR_MXR_MAX_CHANNELS];
  float volume[KR_MXR_MAX_CHANNELS];
  float volume_actual[KR_MXR_MAX_CHANNELS];
  float new_volume_actual[KR_MXR_MAX_CHANNELS];
  int last_sign[KR_MXR_MAX_CHANNELS];
  int win;
  int winss[KR_MXR_MAX_MINIWINS];
  float wins[KR_MXR_MAX_CHANNELS][KR_MXR_MAX_MINIWINS];
  float avg[KR_MXR_MAX_CHANNELS];
  float peak[KR_MXR_MAX_CHANNELS];
  float peak_last[KR_MXR_MAX_CHANNELS];
  float *samples[KR_MXR_MAX_CHANNELS];
  float **mapped_samples[KR_MXR_MAX_CHANNELS];
  int delay;
  int delay_actual;
  int state;
  kr_mixer_path_audio_cb *audio_cb;
  void *audio_user;
  void *control_user;
  kr_mixer *mixer;
  kr_sfx *sfx;
  kr_vertex *graph_vrt;
};

/*
static float get_fade_in(float crossfade_value);
static float get_fade_out(float crossfade_value);
static float get_crossfade(kr_mixer_path *path);
*/
static void apply_effects(kr_mixer_path *path, int nframes);
static void apply_volume(kr_mixer_path *path, int nframes);
static void clear_frames(kr_mixer_path *path, uint32_t nframes);
static int handle_delay(kr_mixer_path *path, uint32_t nframes);
static void import_frames(kr_mixer_path *dest, kr_audio *src);
static void export_frames(kr_audio *dest, kr_mixer_path *src);
static void audio_cb(kr_mixer_path *path, uint32_t nframes);
static void pull_frames(kr_mixer_path *path, uint32_t nframes);
static void limit(kr_mixer_path *path, uint32_t nframes);
static void copy_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n);
static void mix_frames(kr_mixer_path *dest, kr_mixer_path *src, uint32_t n);
static void update_controls(kr_mixer *mixer);
static void update_state(kr_mixer *mixer);
static void update_volume(kr_mixer_path *path);
/* static void set_crossfade(kr_mixer_path *path, float value); */
static void set_volume(kr_mixer_path *path, float value);
static void set_channel_volume(kr_mixer_path *path, int channel, float value);
static void mixer_process(kr_mixer *mixer, uint32_t frames);
static uint32_t ms_to_cycles(kr_mixer *mixer, int ms);
//static void kr_mixer_xf_decouple(kr_mixer *mixer, kr_mixer_crossfader *crossfader);
//static void kr_mixer_xf_couple(kr_mixer *mixer, kr_mixer_path *l, kr_mixer_path *r);
static void path_release(kr_mixer_path *path);
static int path_setup_info_check(kr_mixer_path_info *info);
static int path_setup_check(kr_mixer_io_path_setup *setup);
static void path_sfx_create(kr_mixer_path *path);
static void path_create(kr_mixer_path *path, kr_mixer_io_path_setup *setup);
static kr_mixer_path *make_path(kr_mixer *mixer, kr_mixer_path_setup *setup);
//static void kr_mixer_channel_copy(kr_mixer_path *path, int in_chan, int out_chan);
//static void kr_mixer_channel_move(kr_mixer_path *path, int in_chan, int out_chan);

#include "metering.c"
/*
static float get_fade_in(float crossfade_value) {
  return 1.0f - get_fade_out(crossfade_value);
}

static float get_fade_out(float crossfade_value) {
  float fade_out;
  fade_out = cos (3.14159f*0.5f*((crossfade_value + 100.0f) + 0.5f)/200.0f);
  return fade_out * fade_out;
}

static float get_crossfade(kr_mixer_path *path) {
  if (path->crossfader->path[0] == path) {
    return get_fade_out(path->crossfader->fade);
  } else if (path->crossfader->path[1] == path) {
    return get_fade_in(path->crossfader->fade);
  }

  failfast("failed to get path for crossfade!");
  return 0.0f;
}
*/

static void apply_effects(kr_mixer_path *port, int nframes) {
  // FIXME hrm we count on thems being the same btw in them effects lookout
  kr_sfx_process(port->sfx, port->samples, port->samples, nframes);
}

static void apply_volume(kr_mixer_path *path, int nframes) {
  int c;
  int s;
  int sign;
  sign = 0;
  for (c = 0; c < path->channels; c++) {
    if (path->new_volume_actual[c] == path->volume_actual[c]) {
      for (s = 0; s < nframes; s++) {
        path->samples[c][s] = path->samples[c][s] * path->volume_actual[c];
      }
    } else {
      /* The way the volume change is set up here, the volume can only
       * change once per callback, but thats allways plenty
       * last_sign: 0 = unset, -1 neg, +1 pos */
      if (path->last_sign[c] == 0) {
        if (path->samples[c][0] > 0.0f) {
          path->last_sign[c] = 1;
        } else {
          /* Zero counts as negative here, but its moot */
          path->last_sign[c] = -1;
        }
      }
      for (s = 0; s < nframes; s++) {
        if (path->last_sign[c] != 0) {
          if (path->samples[c][s] > 0.0f) {
            sign = 1;
          } else {
            sign = -1;
          }
          if ((sign != path->last_sign[c]) || (path->samples[c][s] == 0.0f)) {
            path->volume_actual[c] = path->new_volume_actual[c];
            path->last_sign[c] = 0;
          }
        }
        path->samples[c][s] = (path->samples[c][s] * path->volume_actual[c]);
      }
      if (path->last_sign[c] != 0) {
        path->last_sign[c] = sign;
      }
    }
  }
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

static int handle_delay(kr_mixer_path *path, uint32_t nframes) {
  int delay_frames;
  if (path->delay > path->delay_actual) {
    delay_frames = path->delay - path->delay_actual;
    if (delay_frames > nframes) {
      delay_frames = nframes;
    }
    clear_frames(path, delay_frames);
    path->delay += delay_frames;
    return nframes - delay_frames;
  }
  return nframes;
}

static void import_frames(kr_mixer_path *dest, kr_audio *src) {
  int s;
  int c;
  int frames;
  int channels;
  frames = MIN(dest->mixer->period_size, src->count);
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

static void audio_cb(kr_mixer_path *path, uint32_t nframes) {
  kr_mixer_path_audio_cb_arg cb_arg;
  cb_arg.audio.channels = path->channels;
  cb_arg.audio.count = nframes;
  cb_arg.audio.rate = path->mixer->sample_rate;
  cb_arg.user = path->audio_user;
  path->audio_cb(&cb_arg);
  if (path->type == KR_MXR_INPUT) {
    import_frames(path, &cb_arg.audio);
  } else {
    export_frames(&cb_arg.audio, path);
  }
}

static void pull_frames(kr_mixer_path *path, uint32_t nframes) {
  if (path->delay != path->delay_actual) {
    nframes = handle_delay(path, nframes);
  }
  audio_cb(path, nframes);
}

static void limit(kr_mixer_path *path, uint32_t nframes) {
  int c;
  for (c = 0; c < path->channels; c++) {
    krad_hardlimit(path->samples[c], nframes);
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
  } else {
    //up mix
    for (c = 0; c < dest->channels; c++) {
      if (src->mixmap[c] != -1) {
        for (s = 0; s < n; s++) {
          dest->samples[c][s] += src->samples[src->mixmap[c]][s];
        }
      }
    }
  }
}

static void update_controls(kr_mixer *mixer) {
  //int p;
  int i;
  void *client;
  kr_mixer_path *path;
  //kr_mixer_crossfader *crossfader;
  i = 0;
  client = NULL;
  path = NULL;
  //crossfader = NULL;
  /*
  for (p = 0; p < mixer->path_count / 2; p++) {
    crossfader = &mixer->crossfader[p];
    if ((crossfader != NULL) && (crossfader->path[0] != NULL)
     && (crossfader->path[1] != NULL)) {
      if (kr_easer_active(&crossfader->easer)) {
        set_crossfade(crossfader->path[0], kr_easer_process(&crossfader->easer,
         crossfader->fade, &client));
       // krad_radio_broadcast_subpath_control(mixer->broadcaster,
       // &crossfader->path[0]->address, KR_CROSSFADE,
       // crossfader->fade, client);
      }
    }
  }
  */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE)
     && (kr_easer_active(&path->volume_easer))) {
      set_volume(path, kr_easer_process(&path->volume_easer, path->volume[0],
       &client));
     /* krad_radio_broadcast_subpath_control(mixer->broadcaster,
      * &path->address, KR_VOLUME, path->volume[0], client);
      */
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
  if (mixer->new_sample_rate != mixer->sample_rate) {
    mixer->sample_rate = mixer->new_sample_rate;
    while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
      cmd.control = KR_SFX_SET_SAMPLERATE;
      cmd.sample_rate= mixer->sample_rate;
      kr_sfx_ctl(path->sfx, &cmd);
    }
  }
  update_controls(mixer);
}

static void update_volume(kr_mixer_path *path) {
  int c;
  for (c = 0; c < path->channels; c++) {
    set_channel_volume(path, c, path->volume[c]);
  }
}

/*
static void set_crossfade(kr_mixer_path *path, float value) {
  kr_mixer_crossfader *crossfader;
  if (path->crossfader != NULL) {
    crossfader = path->crossfader;
    if ((crossfader->path[0] != NULL) && (crossfader->path[1] != NULL)) {
      crossfader->fade = value;
      update_volume(crossfader->path[0]);
      update_volume(crossfader->path[1]);
    }
  }
}
*/

static void set_volume(kr_mixer_path *path, float value) {
  int c;
  int i;
  float volume_temp;
  kr_mixer_path *sub;
  i = 0;
  volume_temp = (value/100.0f);
  volume_temp *= volume_temp;
  /* FIXME input gets bus volume, bus gets higher level bus vol */
  if ((path->type == KR_MXR_INPUT) || (1 == 2)) {
    volume_temp = volume_temp * path->bus->volume_actual[0];
    /* FIXME
    if (path->crossfader != NULL) {
      volume_temp = volume_temp * get_crossfade(path);
    }
    */
  }
  for (c = 0; c < path->channels; c++) {
    path->volume[c] = value;
    if (path->type == KR_MXR_BUS) {
      path->volume_actual[c] = volume_temp;
    } else {
      path->new_volume_actual[c] = volume_temp;
    }
  }
  if (path->type == KR_MXR_BUS) {
    while ((sub = kr_pool_iterate_active(path->mixer->path_pool, &i))) {
      if ((sub->state == KR_MXP_ACTIVE) && (sub->type == KR_MXR_INPUT)
          && (sub->bus == path)) {
        update_volume(sub);
      }
    }
  }
}

static void set_channel_volume(kr_mixer_path *path, int channel, float value) {
  float volume_temp;
  if (path->type == KR_MXR_INPUT) {
    path->volume[channel] = value;
    volume_temp = (path->volume[channel]/100.0f);
    volume_temp *= volume_temp;
    volume_temp = volume_temp * path->bus->volume_actual[0];
    /*
    if (path->crossfader != NULL) {
      volume_temp = volume_temp * get_crossfade(path);
    }
    */
    path->new_volume_actual[channel] = volume_temp;
  }
}

static void mixer_process(kr_mixer *mixer, uint32_t nframes) {
  int i;
  int bi;
  kr_mixer_path *path;
  kr_mixer_path *bus;
  i = 0;
  bi = 0;
  update_state(mixer);
  /* Pull input audio */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_INPUT)) {
      pull_frames(path, nframes);
    }
  }
  /* Process input and compute metrics */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_INPUT)) {
      apply_volume(path, nframes);
      /* experiment */
      if (path->volume_actual[0] != 0.0f) {
        apply_effects(path, nframes);
      }
      compute_meters(path, nframes);
    }
  }
  /* Mix, process and compute metrics */
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
  /* Copy output and process */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE)
     && (path->type == KR_MXR_OUTPUT) && (path->bus != NULL)) {
      copy_frames(path, path->bus, nframes);
      apply_volume(path, nframes);
      limit(path, nframes);
    }
  }
  /* Push output */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_OUTPUT)) {
      audio_cb(path, nframes);
    }
  }
  /* Compute metrics */
  /*
  compute_meters(mixer->..., nframes);
  */
  mixer->frames_since_peak_read += nframes;
  if (mixer->frames_since_peak_read >= mixer->frames_per_peak_broadcast) {
    mixer->frames_since_peak_read = 0;
    while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
      if (path->state == KR_MXP_ACTIVE) {
        update_meter_readings(path);
      }
    }
  }
  /* Clear mixes for next cycle */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if ((path->state == KR_MXP_ACTIVE) && (path->type == KR_MXR_BUS)) {
      clear_frames(path, nframes);
    }
  }
}

static uint32_t ms_to_cycles(kr_mixer *mixer, int ms) {
  uint32_t cycles;
  float samples_ms;
  float cycle_ms;
  if ((ms < 1) || (ms > (10 * 60 * 1000))) {
    return 0;
  }
  samples_ms = mixer->sample_rate / 1000.0f;
  cycle_ms = mixer->period_size / samples_ms;
  cycles = (ms / cycle_ms) + 1;
  return cycles;
}

/*
void kr_mixer_xf_decouple(kr_mixer *mixer, kr_mixer_crossfader *crossfader) {
  kr_mixer_path *path[2];
  kr_mixer_path_patch patch;
  path[0] = NULL;
  path[1] = NULL;
  if ((crossfader != NULL) && (crossfader->path[0] != NULL) &&
      (crossfader->path[1] != NULL)) {
    path[0] = crossfader->path[0];
    path[1] = crossfader->path[1];
    crossfader->path[0]->crossfader = NULL;
    crossfader->path[1]->crossfader = NULL;
    crossfader->path[0] = NULL;
    crossfader->path[1] = NULL;
    crossfader->fade = -100.0f;
    // Hrm
    strncpy(patch.ctl, "volume", sizeof(patch.ctl));
    patch.val = 0;
    patch.ms = 0;
    kr_mixer_path_ctl(path[0], &patch);
    kr_mixer_path_ctl(path[1], &patch);
    //
    // krad_radio_broadcast_subpath_update(mixer->as->app_broadcaster,
    // &path[0]->address, KR_CROSSFADE_GROUP, KR_STRING, "", NULL);
    //
  }
}

void kr_mixer_xf_couple(kr_mixer *mixer, kr_mixer_path *path1,
 kr_mixer_path *path2) {
  int i;
  kr_mixer_crossfader *crossfader;
  i = 0;
  crossfader = NULL;
  if ((path1 == NULL) || ((path1->state != 1) && (path1->state != 2)) ||
      (path2 == NULL) || ((path2->state != 1) && (path2->state != 2))) {
    printke("Invalid path for crossfade!");
    return;
  }
  if (!(((path1->type == KR_MXR_INPUT) || (path1->type == KR_MXR_BUS)) &&
      ((path1->type == KR_MXR_INPUT) || (path2->type == KR_MXR_BUS)) &&
      (((path1->type == KR_MXR_BUS) && (path2->type == KR_MXR_BUS)) ||
       ((path1->type != KR_MXR_BUS) && (path2->type != KR_MXR_BUS))))) {
    printke("Invalid crossfade group!");
    return;
  }
  if (path1->crossfader != NULL) {
    kr_mixer_xf_decouple(mixer, path1->crossfader);
  }
  if (path2->crossfader != NULL) {
    kr_mixer_xf_decouple(mixer, path2->crossfader);
  }
  for (i = 0; i < mixer->path_count / 2; i++) {
    crossfader = &mixer->crossfader[i];
    if ((crossfader != NULL) && ((crossfader->path[0] == NULL) &&
        (crossfader->path[1] == NULL))) {
      break;
    }
  }
  if (crossfader == NULL) return;
  crossfader->path[0] = path1;
  crossfader->path[1] = path2;
  path1->crossfader = crossfader;
  path2->crossfader = crossfader;

  //krad_radio_broadcast_subpath_update(mixer->as->app_broadcaster,
  // &path1->address, KR_CROSSFADE_GROUP, KR_STRING, path2->ame, NULL);

  kr_mixer_path_ctl(path1, "crossfade", -100.0f, 0, NULL);
}
*/

static void path_release(kr_mixer_path *path) {
  int c;
  path->delay = 0;
  path->delay_actual = 0;
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    free(path->samples[c]);
  }
  if (path->sfx != NULL) {
    kr_sfx_destroy(path->sfx);
    path->sfx = NULL;
  }
  path->state = KR_MXP_NIL;
  kr_pool_recycle(path->mixer->path_pool, path);
}

static int path_setup_info_check(kr_mixer_path_info *info) {
  if (info->channels < 1) return -1;
  if (info->channels > KR_MXR_MAX_CHANNELS) return -1;
  if ((info->type != KR_MXR_INPUT)
      && (info->type != KR_MXR_BUS)
      && (info->type != KR_MXR_OUTPUT)) {
    return -4;
  }
  /* FIXME check all the things
   *
   * bus exists
   * sfx params
   * volume
   * crossfade partner
   * matrix
   *
   * */
  return 0;
}

static int path_setup_check(kr_mixer_path_setup *setup) {
  if (setup->info.type == KR_MXR_BUS) {
    if (setup->audio_cb != NULL) return -1;
    if (setup->audio_user != NULL) return -2;
  } else {
    if (setup->audio_cb == NULL) return -3;
    if (setup->audio_user == NULL) return -4;
  }
  if (setup->control_user == NULL) return -5;
  if (path_setup_info_check(&setup->info)) return -6;
  return 0;
}

static void path_sfx_create(kr_mixer_path *path) {
  kr_sfx_setup setup;
  kr_sfx_cmd cmd;
  setup.channels = path->channels;
  setup.sample_rate = path->mixer->sample_rate;
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
}

static void path_create(kr_mixer_path *path, kr_mixer_path_setup *setup) {
  int c;
  kr_mixer_event event;
  path->crossfader = NULL;
  path->channels = setup->info.channels;
  path->type = setup->info.type;
  path->audio_cb = setup->audio_cb;
  path->audio_user = setup->audio_user;
  path->control_user = setup->control_user;
  if (path->type == KR_MXR_BUS) {
    path->bus = NULL;
  } else {
    path->bus = path->mixer->master;
  }
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    if (c < path->channels) {
      /* FIXME take mapping from setup */
      path->mixmap[c] = c;
    } else {
      path->mixmap[c] = -1;
    }
    path->map[c] = c;
    path->mapped_samples[c] = &path->samples[c];
    path->volume[c] = setup->info.volume[c];
    path->volume_actual[c] = (float)(path->volume[c]/100.0f);
    path->volume_actual[c] *= path->volume_actual[c];
    path->new_volume_actual[c] = path->volume_actual[c];
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
}

/*
void kr_mixer_channel_move(kr_mixer_path *path, int in_chan, int out_chan) {
  path->map[in_chan] = out_chan;
  path->mapped_samples[in_chan] = &path->samples[out_chan];
}

void kr_mixer_channel_copy(kr_mixer_path *path, int in_chan, int out_chan) {
  path->mixmap[out_chan] = in_chan;
}
*/

int kr_mixer_process(kr_mixer *mixer) {
  if (mixer == NULL) return -1;
  mixer_process(mixer, mixer->period_size);
  return mixer->period_size;
}

int kr_mixer_path_ctl(kr_mixer_path *path, kr_mixer_path_patch *patch) {
  int duration;
  if (path == NULL) return -1;
  duration = ms_to_cycles(path->mixer, patch->ms);
  if ((strncmp(patch->ctl, "volume", 6) == 0) && (strlen(patch->ctl) == 6)) {
    //path_set_volume (path, value);
    kr_easer_set(&path->volume_easer, patch->val, duration, EASEINOUTSINE, NULL);
   return 0;
  }
  /* FIXME Add bus to bus and output to output crossfading */
  if ((path->type == KR_MXR_INPUT) && (path->type != KR_MXR_BUS)) {
    if ((strncmp(patch->ctl, "crossfade", 9) == 0) && (strlen(patch->ctl) == 9)) {
      if (path->crossfader != NULL) {
        //path_set_crossfade (path, value);
        kr_easer_set(&path->crossfader->easer, patch->val, duration, EASEINOUTSINE, NULL);
      }
      return 0;
    }
    if (strncmp(patch->ctl, "volume_left", 11) == 0) {
      set_channel_volume(path, 0, patch->val);
      return 0;
    }
    if (strncmp(patch->ctl, "volume_right", 12) == 0) {
      set_channel_volume(path, 1, patch->val);
      return 0;
    }
  }
  return -2;
}

int kr_mixer_path_info_get(kr_mixer_path *path, kr_mixer_path_info *info) {
  int i;
  kr_sfx_cmd cmd;
  if ((path == NULL) || (info == NULL)) return -1;
  strcpy(info->bus, "Master");
  info->channels = path->channels;
  info->delay = path->delay;
  info->type = path->type;
  for (i = 0; i < KR_MXR_MAX_CHANNELS; i++) {
    info->volume[i] = path->volume[i];
    info->map[i] = path->map[i];
    info->mixmap[i] = path->mixmap[i];
    info->rms[i] = path->avg[i];
    info->peak[i] = path->peak_last[i];
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
  if ((path->crossfader != NULL) && (path->crossfader->path[0] == path)) {
    info->fade = path->crossfader->fade;
  } else {
    info->crossfade_group[0] = '\0';
    info->fade = 0.0f;
  }
  return 0;
}

int kr_mixer_unlink(kr_mixer_path *path) {
  kr_mixer_event event;
  if (path == NULL) {
    printke("mixer path unlink called with null value");
    return -1;
  }
  if (path->crossfader != NULL) {
    /* FIXME kr_mixer_xf_decouple(path->mixer, path->crossfader); */
  }
  event.user = path->mixer->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = KR_MIXER_DESTROY;
  memset(&event.info, 0, sizeof(event.info));
  path->mixer->event_cb(&event);
  path->state = KR_MXP_TERM;
  if (path->mixer->clock == NULL) {
    update_state(path->mixer);
  }
  return 0;
}

static kr_mixer_path *make_path(kr_mixer *mixer, kr_mixer_path_setup *setup) {
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
  path_create(path, setup);
  path->graph_vrt = kr_graph_vertex_create(mixer->mixer_graph,setup->info.type);
  return path;
}

kr_mixer_path *kr_mixer_mkio(kr_mixer *mixer, kr_mixer_io_path_setup *setup) {
  if ((mixer == NULL) || (setup == NULL)) return NULL;
  return make_path(mixer, setup);
}

int kr_mixer_mkbus(kr_mixer *mixer, kr_mixer_path_info *info, void *user) {
  kr_mixer_path *path;
  kr_mixer_path_setup setup;
  if ((mixer == NULL) || (info == NULL) || (user == NULL)) return -1;
  setup.info = *info;
  setup.control_user = user;
  setup.audio_user = NULL;
  setup.audio_cb = NULL;
  path = make_path(mixer, &setup);
  if (path == NULL) return -2;
  if (mixer->master == NULL) {
    /* FIXME hrm?? */
    mixer->master = path;
  }
  return 0;
}

int kr_mixer_destroy(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  printk("Mixer: Destroying");
  i = 0;
  if (mixer->clock != NULL) {
    while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
      if ((path->state != KR_MXP_NIL) && (path->type != KR_MXR_BUS)) {
        kr_mixer_unlink(path);
      }
    }
  }
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->state != KR_MXP_NIL) {
      kr_mixer_unlink(path);
    }
  }
  if (mixer->clock == NULL) {
    update_state(mixer);
  } else {
    /* FIXME maybe we need to wait maybe not well see (dobut it) */
  }
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
  mixer->path_count = setup->path_count;
  mixer->mixer_graph = kr_graph_create(&graph_setup);
  /* FIXME defaults */
  mixer->period_size = KR_MXR_PERIOD_DEF;
  mixer->new_period_size = mixer->period_size;
  mixer->sample_rate = KR_MXR_SRATE_DEF;
  mixer->new_sample_rate = mixer->sample_rate;
  mixer->avg_window_size = (mixer->sample_rate / 1000) * KR_MXR_RMS_WINDOW_MS;
  mixer->frames_per_peak_broadcast = 1536;
  /* end defaults */
  printk("Mixer: Created");
  return mixer;
}
