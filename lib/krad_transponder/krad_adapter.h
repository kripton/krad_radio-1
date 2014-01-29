#ifndef KRAD_ADAPTER_H
#define KRAD_ADAPTER_H

#define KR_ADAPTER_PATHS_MAX 32

typedef struct kr_adapter kr_adapter;
typedef struct kr_adapter_setup kr_adapter_setup;
typedef struct kr_adapter_path kr_adapter_path;
typedef struct kr_adapter_path_setup kr_adapter_path_setup;

#include "krad_adapter_common.h"
#include "krad_adapter_monitor.h"
#include "krad_av.h"

#include "krad_jack.h"
#include "krad_wayland.h"
#include "krad_v4l2.h"
#include "krad_decklink.h"
#include "krad_x11.h"
#include "krad_alsa.h"

typedef enum {
  KR_ADAPTER_PROCESS, /* Process all paths: jack,alsa,decklink */
  KR_ADAPTER_DISCONNECTED,
  KR_ADAPTER_RECONNECTED
} kr_adapter_event_type;

typedef enum {
  KR_ADAPTER_PATH_PROCESS, /* Process path: wayland,x11,krad - v4l2*(could work either way) */
} kr_adapter_path_event_type;

typedef struct {
  kr_adapter_path *path;
  void *user;
  kr_adapter_path_event_type type;
} kr_adapter_path_event_cb_arg;

typedef struct {
  kr_adapter_path *path;
  kr_image image;
  kr_image *image_in;
  kr_audio audio;
  void *user;
} kr_adapter_path_av_cb_arg;

typedef struct {
  kr_adapter *adapter;
  void *user;
  kr_adapter_event_type type;
} kr_adapter_event_cb_arg;

typedef int (kr_adapter_process_function)(kr_adapter *);
typedef int (kr_adapter_path_process_function)(kr_adapter_path *);
typedef void (kr_adapter_event_cb)(kr_adapter_event_cb_arg *);
typedef void (kr_adapter_path_event_cb)(kr_adapter_path_event_cb_arg *);
typedef void (kr_adapter_path_av_cb)(kr_adapter_path_av_cb_arg *);

struct kr_adapter_path_setup {
  kr_adapter_path_info info;
  void *user;
  kr_adapter_path_event_cb *ev_cb;
  kr_adapter_path_av_cb *av_cb;
};

struct kr_adapter_setup {
  kr_adapter_info info;
  void *user;
  kr_adapter_event_cb *ev_cb;
};

int kr_adapter_unlink(kr_adapter_path *path);
kr_adapter_path *kr_adapter_mkpath(kr_adapter *a, kr_adapter_path_setup *s);
int kr_adapter_get_info(kr_adapter *adapter, kr_adapter_info *info);
int kr_adapter_destroy(kr_adapter *adapter);
kr_adapter *kr_adapter_create(kr_adapter_setup *setup);

#endif
