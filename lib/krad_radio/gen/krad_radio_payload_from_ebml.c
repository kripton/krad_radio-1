#include "krad_radio_payload_from_ebml.h"

int kr_nil_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_nil *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_nil *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, &actual->n);

  return res;
}
int kr_payload_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_payload_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_payload_type *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
int kr_payload_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_payload *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_payload *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->nil);
      uber.type = DEBML_KR_NIL;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->xpdr_path_info);
      uber.type = DEBML_KR_XPDR_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = DEBML_KR_MIXER_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = DEBML_KR_COMPOSITOR_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}
