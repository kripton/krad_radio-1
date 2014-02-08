#include "kr_aux.h"

int kr_aux_pctl(kr_adapter_path *path, kr_patchset *patchset) {
 if (path == NULL) return -1;
 if (patchset == NULL) return -2;
 return 0;
}

int kr_aux_rmpath(kr_adapter_path *path) {
 if (path == NULL) return -1;
 return 0;
}

kr_adapter_path *kr_aux_mkpath(kr_adapter *adp, kr_adapter_path_setup *setup) {
 if (adp == NULL) return NULL;
 if (setup == NULL) return NULL;
 return NULL;
}

int kr_aux_ctl(kr_adapter *adp, kr_patchset *patchset) {
 if (adp == NULL) return -1;
 if (patchset == NULL) return -2;
 return 0;
}

int kr_aux_destroy(kr_adapter *adp) {
 if (adp == NULL) return -1;
 return 0;
}

kr_adapter *kr_aux_create(kr_adapter_setup *setup) {
 if (setup == NULL) return NULL;
 return NULL;
}
