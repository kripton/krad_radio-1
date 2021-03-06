#ifndef KRAD_JACK_H
#define KRAD_JACK_H

#include <jack/jack.h>

#include "krad_system.h"
#include "krad_jack_common.h"
#include "krad_av.h"

#define KR_JACK_CHANNELS_MAX 8

typedef struct kr_jack kr_jack;
typedef struct kr_jack_setup kr_jack_setup;
typedef struct kr_jack_path_setup kr_jack_path_setup;
typedef struct kr_jack_path kr_jack_path;
typedef struct kr_jack_path kr_jack_input;
typedef struct kr_jack_path kr_jack_output;

typedef enum {
  KR_JACK_SHUTDOWN,
  KR_JACK_PROCESS,
//  KR_JACK_AUDIO_INPUT,
//  KR_JACK_AUDIO_OUTPUT,
  KR_JACK_INFO, /* FIXME what info.. how to get .. */
  KR_JACK_PATH_INFO /* FIXME what info.. how to get .. */
} kr_jack_event_type;

typedef struct {
  kr_jack_event_type event;
  void *user;
  kr_jack_path *path;
} kr_jack_path_event_cb_arg;

typedef struct {
  kr_jack_event_type event;
  void *user;
  kr_jack_path *path;
  kr_audio audio;
} kr_jack_path_audio_cb_arg;

typedef struct {
  kr_jack_event_type type;
  void *user;
} kr_jack_event_cb_arg;

typedef void (kr_jack_event_cb)(kr_jack_event_cb_arg *);
typedef void (kr_jack_path_event_cb)(kr_jack_path_event_cb_arg *);
typedef void (kr_jack_path_audio_cb)(kr_jack_path_audio_cb_arg *);

struct kr_jack_setup {
  char client_name[64];
  char server_name[64];
  kr_jack_info info;
  void *user;
  kr_jack_event_cb *event_cb;
};

struct kr_jack_path_setup {
  kr_jack_path_info info;
  void *user;
  kr_jack_path_event_cb *event_cb;
  kr_jack_path_audio_cb *audio_cb;
};

int kr_jack_unlink(kr_jack_path *path);
kr_jack_path *kr_jack_mkpath(kr_jack *jack, kr_jack_path_setup *setup);
int kr_jack_destroy(kr_jack *jack);
kr_jack *kr_jack_create(kr_jack_setup *setup);
int kr_jack_detect();
int kr_jack_detect_server_name(char *name);

#endif
