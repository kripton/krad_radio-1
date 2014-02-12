#ifndef KR_XPDR_H
#define KR_XPDR_H

typedef struct kr_xpdr kr_xpdr;
typedef struct kr_xpdr_setup kr_xpdr_setup;
typedef struct kr_xpdr_path kr_xpdr_path;
typedef struct kr_xpdr_event kr_xpdr_event;
typedef void (kr_xpdr_event_cb)(kr_xpdr_event *);

#include "kr_xpdr_common.h"
#include "krad_av.h"
#include "krad_mixer.h"
#include "krad_compositor.h"

typedef enum {
  KR_XPDR_CREATE = 1,
  KR_XPDR_PATCH,
  KR_XPDR_DESTROY
} kr_xpdr_event_type;

struct kr_xpdr_event {
  kr_xpdr_path *path;
  void *user;
  void *user_path;
  kr_xpdr_event_type type;
  kr_xpdr_path_info_patch patch;
  kr_xpdr_path_info info;
};

struct kr_xpdr_setup {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_xpdr_event_cb *event_cb;
  void *user;
  int path_count;
};

int kr_xpdr_ctl(kr_xpdr_path *path, kr_xpdr_path_info_patch *patch);
int kr_xpdr_remove(kr_xpdr_path *path);
int kr_xpdr_link(kr_xpdr_path *path, kr_xpdr_path_info *info, void *user);
int kr_xpdr_open(kr_xpdr *xpdr, kr_xpdr_path_info *info, void *user);
int kr_xpdr_monitor(kr_xpdr *xpdr, int on);
int kr_xpdr_destroy(kr_xpdr *xpdr);
kr_xpdr *kr_xpdr_create(kr_xpdr_setup *setup);

#endif
