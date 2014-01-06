#include "krad_app_common_to_ebml.h"

int kr_app_method_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_app_method *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_app_method *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, *actual);

  return res;
}

int kr_crate2_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_crate2 *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_crate2 *)st;

  res += kr_ebml_pack_string(ebml, 0xe1, actual->address);
  uber.actual = &(actual->method);
  uber.type = EBML_KR_APP_METHOD;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}

