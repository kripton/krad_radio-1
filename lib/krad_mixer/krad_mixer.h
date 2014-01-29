#ifndef KRAD_MIXER_H
#define KRAD_MIXER_H

typedef struct kr_mixer kr_mixer;
typedef struct kr_mixer_path kr_mixer_path;
typedef struct kr_mixer_path kr_mixer_source;
typedef struct kr_mixer_path kr_mixer_input;
typedef struct kr_mixer_path kr_mixer_bus;
typedef struct kr_mixer_path kr_mixer_output;
typedef struct kr_mixer_io_path_setup kr_mixer_io_path_setup;
typedef struct kr_mixer_path_audio_cb_arg kr_mixer_path_audio_cb_arg;
typedef struct kr_mixer_event kr_mixer_event;
typedef struct kr_mixer_setup kr_mixer_setup;

#define KR_MXR_MAX_MINIWINS 192

#include "krad_mixer_common.h"
#include "krad_av.h"

typedef enum {
  KR_MIXER_CREATE = 1,
  KR_MIXER_PATCH,
  KR_MIXER_DESTROY
} kr_mixer_event_type;

typedef void (kr_mixer_event_cb)(kr_mixer_event *);
typedef void (kr_mixer_path_audio_cb)(kr_mixer_path_audio_cb_arg *);

struct kr_mixer_event {
  kr_mixer_path *path;
  void *user;
  void *user_path;
  kr_mixer_event_type type;
  kr_mixer_path_patch patch;
  kr_mixer_path_info info;
};

struct kr_mixer_path_audio_cb_arg {
  kr_audio audio;
  void *user;
};

struct kr_mixer_io_path_setup {
  kr_mixer_path_info info;
  void *control_user;
  void *audio_user;
  kr_mixer_path_audio_cb *audio_cb;
};

struct kr_mixer_setup {
  uint32_t path_count;
  void *user;
  kr_mixer_event_cb *event_cb;
};

int kr_mixer_process(kr_mixer_path *path);
int kr_mixer_path_ctl(kr_mixer_path *path, kr_mixer_path_patch *patch);
int kr_mixer_path_info_get(kr_mixer_path *path, kr_mixer_path_info *info);
int kr_mixer_unlink(kr_mixer_path *path);
kr_mixer_path *kr_mixer_mkso(kr_mixer *mixer, kr_mixer_io_path_setup *setup);
int kr_mixer_mkbus(kr_mixer *mixer, kr_mixer_path_info *info, void *user);
int kr_mixer_splice(kr_mixer *mixer, kr_mixer_path_info *info, kr_mixer_path *from, kr_mixer_path *to, void *user);
int kr_mixer_destroy(kr_mixer *mixer);
kr_mixer *kr_mixer_create(kr_mixer_setup *setup);

#endif
