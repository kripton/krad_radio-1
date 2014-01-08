#ifndef KRAD_TRANSPONDER_H
#define KRAD_TRANSPONDER_H

#define KR_XPDR_PATHS_MAX 32

typedef struct kr_transponder kr_transponder;
typedef struct kr_transponder_setup kr_transponder_setup;
typedef struct kr_transponder_path kr_transponder_path;
typedef struct kr_transponder_event_info kr_transponder_event_info;
typedef void (kr_transponder_event_cb)(kr_transponder_event_info *);

typedef struct kr_transponder kr_xpdr;
typedef struct kr_transponder_setup kr_xpdr_setup;
typedef struct kr_transponder_path kr_xpdr_path;
typedef struct kr_transponder_event_info kr_xpdr_event_info;
typedef void (kr_xpdr_event_cb)(kr_xpdr_event_info *);

#include "krad_transponder_common.h"
#include "krad_adapter.h"
#include "krad_mixer.h"
#include "krad_compositor.h"

struct kr_transponder_event_info {
  void *user;
  void *user_path;
};

struct kr_transponder_setup {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder_event_cb *event_cb;
  void *user;
};

kr_xpdr_path *kr_transponder_mkpath(kr_xpdr *x, kr_xpdr_path_info *i, void *p);
int kr_transponder_unlink(kr_xpdr_path *path);
kr_transponder *kr_transponder_create(kr_transponder_setup *setup);
int kr_transponder_destroy(kr_transponder *transponder);

#endif
