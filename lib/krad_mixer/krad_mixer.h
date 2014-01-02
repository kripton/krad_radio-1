#ifndef KRAD_MIXER_H
#define KRAD_MIXER_H

typedef struct kr_mixer kr_mixer;
typedef struct kr_mixer_path kr_mixer_path;
typedef struct kr_mixer_path kr_mixer_input;
typedef struct kr_mixer_path kr_mixer_bus;
typedef struct kr_mixer_path kr_mixer_output;
typedef struct kr_mixer_crossfader kr_mixer_crossfader;
typedef struct kr_mixer_path_setup kr_mixer_path_setup;
typedef struct kr_mixer_path_setup kr_mixer_input_setup;
typedef struct kr_mixer_path_setup kr_mixer_output_setup;
typedef struct kr_mixer_path_audio_cb_arg kr_mixer_path_audio_cb_arg;
typedef struct kr_mixer_path_info_cb_arg kr_mixer_path_info_cb_arg;
typedef struct kr_mixer_info_cb_arg kr_mixer_info_cb_arg;
typedef struct kr_mixer_setup kr_mixer_setup;

#define KR_MXR_MAX_MINIWINS 192

#include "krad_mixer_common.h"
#include "krad_pool.h"
#include "krad_av.h"
#include "krad_sfx.h"

typedef void (kr_mixer_info_cb)(kr_mixer_info_cb_arg *);
typedef void (kr_mixer_path_info_cb)(kr_mixer_path_info_cb_arg *);
typedef void (kr_mixer_path_audio_cb)(kr_mixer_path_audio_cb_arg *);

struct kr_mixer_info_cb_arg {
  /* EVENT INFO */
  void *user;
};

struct kr_mixer_path_info_cb_arg {
  /* PATH EVENT INFO */
  void *user;
  kr_mixer_path *path;
};

struct kr_mixer_path_audio_cb_arg {
  kr_audio audio;
  void *user;
};

struct kr_mixer_path_setup {
  kr_mixer_path_info info;
  void *user;
  kr_mixer_path_info_cb *info_cb;
  kr_mixer_path_audio_cb *audio_cb;
};

struct kr_mixer_setup {
  uint32_t period_size;
  uint32_t sample_rate;
  uint32_t path_count;
  void *user;
  kr_mixer_info_cb *cb;
  void *mem;
};

kr_mixer_path *kr_mixer_mkpath(kr_mixer *mixer, kr_mixer_path_setup *setup);
int kr_mixer_unlink(kr_mixer_path *path);
kr_mixer_path *kr_mixer_find(kr_mixer *mixer, char *name);
kr_mixer_path *kr_mixer_path_iter(kr_mixer *mixer, int *count);
int kr_mixer_path_ctl(kr_mixer_path *mp, char *ctl, float val, int dr, void *p);

size_t kr_mixer_size(void);
void kr_mixer_setup_init(kr_mixer_setup *setup);
kr_mixer *kr_mixer_create(kr_mixer_setup *setup);
int kr_mixer_destroy(kr_mixer *mixer);
int kr_mixer_process(kr_mixer *mixer);

int kr_mixer_get_info(kr_mixer *mixer, kr_mixer_info *info);
int kr_mixer_get_path_info(kr_mixer_path *unit, kr_mixer_path_info *info);

kr_sfx *kr_mixer_path_sfx_kludge(kr_mixer_path *path);

#endif
