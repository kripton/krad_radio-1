#include "kr_adapter.h"
#include "krad_wayland_common.h"

#ifndef KRAD_WAYLAND_H
#define KRAD_WAYLAND_H

int kr_wl_lctl(kr_adapter_path *, kr_patchset *);
int kr_wl_unlink(kr_adapter_path *);
kr_adapter_path *kr_wl_link(kr_adapter *, kr_adapter_path_setup *);
int kr_wl_ctl(kr_adapter *, kr_patchset *);
int kr_wl_close(kr_adapter *);
kr_adapter *kr_wl_open(kr_adapter_setup *);

#endif
