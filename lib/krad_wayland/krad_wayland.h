#include <inttypes.h>
#include "kr_adapter.h"
#include "krad_wayland_common.h"

#ifndef KRAD_WAYLAND_H
#define KRAD_WAYLAND_H

#define KR_WL_MAX_WINDOWS 4
#define KR_WL_BUFFER_COUNT 1

typedef struct kr_wayland kr_wayland;
typedef struct kr_wayland_path kr_wayland_path;

enum kr_wayland_event_type {
  KR_WL_FRAME,
  KR_WL_POINTER,
  KR_WL_KEY
};

typedef struct {
  int x;
  int y;
  int click;
  int pointer_in;
  int pointer_out;
} kr_wayland_pointer_event;

typedef struct {
  int key;
  int down;
} kr_wayland_key_event;

typedef struct {
  uint8_t *buffer;
} kr_wayland_frame_event;

typedef struct {
  int type;
  kr_wayland_pointer_event pointer_event;
  kr_wayland_key_event key_event;
  kr_wayland_frame_event frame_event;
} kr_wayland_event;

typedef struct {
  kr_wayland_path_info info;
  int (*callback)(void *, kr_wayland_event *);
  void *user;
} kr_wayland_path_setup;

typedef struct {
  kr_wayland_info info;
} kr_wayland_setup;

kr_wayland_path *kr_wayland_mkpath(kr_wayland *wayland,
 kr_wayland_path_setup *setup);
int kr_wayland_unlink(kr_wayland_path **path);
int kr_wayland_get_fd(kr_wayland *wayland);
int kr_wayland_process(kr_wayland *wayland);
int kr_wayland_destroy(kr_wayland **wl);
kr_wayland *kr_wayland_create(kr_wayland_setup *setup);

int kr_wl_lctl(kr_adapter_path *, kr_patchset *);
int kr_wl_unlink(kr_adapter_path *);
kr_adapter_path *kr_wl_link(kr_adapter *, kr_adapter_path_setup *);
int kr_wl_ctl(kr_adapter *, kr_patchset *);
int kr_wl_close(kr_adapter *);
kr_adapter *kr_wl_open(kr_adapter_setup *);

#endif
