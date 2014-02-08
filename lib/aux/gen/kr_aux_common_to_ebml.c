#include "kr_aux_common_to_ebml.h"

int kr_aux_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_aux_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_info *)st;
  res += kr_ebml_pack_string(ebml, 0xe1, actual->monkeyname);

  return res;
}
int kr_aux_path_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_aux_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_aux_path_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->ncoconuts);

  return res;
}
