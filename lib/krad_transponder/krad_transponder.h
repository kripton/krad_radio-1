#ifndef KRAD_TRANSPONDER_H
#define KRAD_TRANSPONDER_H

#define KR_XPDR_PATHS_MAX 32

typedef struct kr_transponder kr_transponder;
typedef struct kr_transponder_setup kr_transponder_setup;
typedef struct kr_transponder_path kr_transponder_path;
typedef struct kr_transponder_event kr_transponder_event;
typedef void (kr_transponder_event_cb)(kr_transponder_event *);

typedef struct kr_transponder kr_xpdr;
typedef struct kr_transponder_setup kr_xpdr_setup;
typedef struct kr_transponder_path kr_xpdr_path;
typedef struct kr_transponder_event kr_xpdr_event;
typedef void (kr_xpdr_event_cb)(kr_xpdr_event *);

#include "krad_transponder_common.h"
#include "krad_adapter.h"
#include "krad_mixer.h"
#include "krad_compositor.h"

struct kr_transponder_event {
  void *user;
  void *user_path;
  /* method
   * info struct */
};

struct kr_transponder_setup {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder_event_cb *event_cb;
  void *user;
};

int kr_transponder_destroy(kr_transponder *transponder);
kr_transponder *kr_transponder_create(kr_transponder_setup *setup);

int kr_transponder_unlink(kr_xpdr_path *path);
int kr_transponder_mkpath(kr_xpdr *xpdr, kr_xpdr_path_info *i, void *user);
int kr_transponder_path_ctl(kr_xpdr_path *path, kr_xpdr_path_patch *patch);

#endif
