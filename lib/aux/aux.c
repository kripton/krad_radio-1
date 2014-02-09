#include "kr_aux.h"

int kr_aux_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Aux adapter path controlled");
  return 0;
}

int kr_aux_unlink(kr_adapter_path *path) {
  if (path == NULL) return -1;
  printk("Aux adapter path removed");
  return 0;
}

kr_adapter_path *kr_aux_link(kr_adapter *adp, kr_adapter_path_setup *setup) {
  if (adp == NULL) return NULL;
  if (setup == NULL) return NULL;
  printk("Aux adapter path created");
  return NULL;
}

int kr_aux_ctl(kr_adapter *adp, kr_patchset *patchset) {
  if (adp == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Aux adapter controlled");
  return 0;
}

int kr_aux_close(kr_adapter *adp) {
  if (adp == NULL) return -1;
  printk("Aux adapter destroyed");
  return 0;
}

kr_adapter *kr_aux_open(kr_adapter_setup *setup) {
  if (setup == NULL) return NULL;
  printk("Aux adapter created");
  return NULL;
}
