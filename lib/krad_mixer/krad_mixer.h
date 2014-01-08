#ifndef KRAD_MIXER_H
#define KRAD_MIXER_H

typedef struct kr_mixer kr_mixer;
typedef struct kr_mixer_path kr_mixer_path;
typedef struct kr_mixer_path kr_mixer_input;
typedef struct kr_mixer_path kr_mixer_bus;
typedef struct kr_mixer_path kr_mixer_output;
typedef struct kr_mixer_crossfader kr_mixer_crossfader;
typedef struct kr_mixer_io_path_setup kr_mixer_io_path_setup;
typedef struct kr_mixer_path_audio_cb_arg kr_mixer_path_audio_cb_arg;
typedef struct kr_mixer_event kr_mixer_event;
typedef struct kr_mixer_setup kr_mixer_setup;

#define KR_MXR_MAX_MINIWINS 192

#include "krad_mixer_common.h"
#include "krad_pool.h"
#include "krad_av.h"
#include "krad_sfx.h"

typedef void (kr_mixer_event_cb)(kr_mixer_event *);
typedef void (kr_mixer_path_audio_cb)(kr_mixer_path_audio_cb_arg *);

struct kr_mixer_event {
  /* EVENT INFO */
  void *user;
};

struct kr_mixer_path_audio_cb_arg {
  kr_audio audio;
  void *user;
};

struct kr_mixer_io_path_setup {
  kr_mixer_path_info info;
  kr_mixer_bus *bus;
  void *user;
  kr_mixer_path_audio_cb *audio_cb;
};

struct kr_mixer_setup {
  uint32_t period_size;
  uint32_t sample_rate;
  uint32_t path_count;
  void *user;
  kr_mixer_event_cb *event_cb;
};

size_t kr_mixer_size(void);
void kr_mixer_setup_init(kr_mixer_setup *setup);
int kr_mixer_destroy(kr_mixer *mixer);
kr_mixer *kr_mixer_create(kr_mixer_setup *setup);

int kr_mixer_unlink(kr_mixer_path *path);
kr_mixer_path *kr_mixer_mkio(kr_mixer *mixer, kr_mixer_io_path_setup *setup);
kr_mixer_path *kr_mixer_mkbus(kr_mixer *mixer, kr_mixer_path_info *info, void *p);
int kr_mixer_path_ctl(kr_mixer_path *path, kr_mixer_path_patch *patch);

int kr_mixer_path_info_get(kr_mixer_path *unit, kr_mixer_path_info *info);

/* go away */
int kr_mixer_process(kr_mixer *mixer);
kr_sfx *kr_mixer_path_sfx_kludge(kr_mixer_path *path);
/* end go away */

#endif
