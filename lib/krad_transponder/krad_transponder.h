#ifndef KRAD_TRANSPONDER_H
#define KRAD_TRANSPONDER_H

typedef struct kr_transponder kr_transponder;
typedef struct kr_transponder_setup kr_transponder_setup;
typedef struct kr_transponder_path kr_transponder_path;
typedef struct kr_transponder_event kr_transponder_event;
typedef void (kr_transponder_event_cb)(kr_transponder_event *);

#include "krad_transponder_common.h"
#include "krad_av.h"
#include "krad_mixer.h"
#include "krad_compositor.h"

typedef enum {
  KR_XPDR_CREATE = 1,
  KR_XPDR_PATCH,
  KR_XPDR_DESTROY
} kr_transponder_event_type;

struct kr_transponder_event {
  kr_transponder_path *path;
  void *user;
  void *user_path;
  kr_transponder_event_type type;
  kr_transponder_path_info_patch patch;
  kr_transponder_path_info info;
};

struct kr_transponder_setup {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder_event_cb *event_cb;
  void *user;
  int path_count;
};

int kr_transponder_path_ctl(kr_transponder_path *path, kr_transponder_path_info_patch *patch);
int kr_transponder_unlink(kr_transponder_path *path);
int kr_transponder_mkpath(kr_transponder *transponder, kr_transponder_path_info *i, void *user);
int kr_transponder_destroy(kr_transponder *transponder);
kr_transponder *kr_transponder_create(kr_transponder_setup *setup);

#endif
