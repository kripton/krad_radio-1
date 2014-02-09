#ifndef KR_AUX_H
#define KR_AUX_H

#include "krad_system.h"
#include "kr_adapter.h"
#include "kr_aux_common.h"

int kr_aux_lctl(kr_adapter_path *, kr_patchset *);
int kr_aux_unlink(kr_adapter_path *);
kr_adapter_path *kr_aux_link(kr_adapter *, kr_adapter_path_setup *);
int kr_aux_ctl(kr_adapter *, kr_patchset *);
int kr_aux_close(kr_adapter *);
kr_adapter *kr_aux_open(kr_adapter_setup *);

#endif
