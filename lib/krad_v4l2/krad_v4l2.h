#include "kr_adapter.h"
#include "krad_v4l2_common.h"

#ifndef KRAD_V4L2_H
#define KRAD_V4L2_H

int kr_v4l2_lctl(kr_adapter_path *, kr_patchset *);
int kr_v4l2_unlink(kr_adapter_path *);
int kr_v4l2_link(kr_adapter *, kr_adapter_path *);
int kr_v4l2_ctl(kr_adapter *, kr_patchset *);
int kr_v4l2_close(kr_adapter *);
int kr_v4l2_open(kr_adapter *);

#endif
