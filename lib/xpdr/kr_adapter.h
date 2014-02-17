#ifndef KRAD_ADAPTER_H
#define KRAD_ADAPTER_H

typedef struct kr_adapter kr_adapter;
typedef struct kr_adapter_spec kr_adapter_spec;
typedef struct kr_adapter_path kr_adapter_path;

#include "kr_xpdr_common.h"
#include "krad_av.h"

typedef enum {
  KR_ADAPTER_PROCESS, /* Process all paths: jack,alsa,decklink */
  KR_ADAPTER_DISCONNECTED,
  KR_ADAPTER_RECONNECTED
} kr_adapter_event_type;

typedef struct {
  kr_adapter *adapter;
  void *user;
  kr_adapter_event_type type;
} kr_adapter_event;

typedef void (kr_adapter_event_cb)(kr_adapter_event *);

struct kr_adapter {
  void *handle;
  int fd;
  void *user;
  kr_adapter_event_cb *event_cb;
  kr_xpdr_path_info *info;
};

struct kr_adapter_path {
  void *handle;
  void *user;
  kr_av_handler *av_handler;
  kr_xpdr_path_info *info;
};

typedef struct {
  int coconut;
} kr_patchset;

typedef int (kr_adapter_lctl)(kr_adapter_path *, kr_patchset *);
typedef int (kr_adapter_unlink)(kr_adapter_path *);
typedef int (kr_adapter_link)(kr_adapter *, kr_adapter_path *);
typedef int (kr_adapter_ctl)(kr_adapter *, kr_patchset *);
typedef int (kr_adapter_close)(kr_adapter *);
typedef int (kr_adapter_open)(kr_adapter *);

struct kr_adapter_spec {
  kr_adapter_lctl *lctl;
  kr_adapter_unlink *unlink;
  kr_adapter_link *link;
  kr_adapter_ctl *ctl;
  kr_adapter_close *close;
  kr_adapter_open *open;
};

#endif
