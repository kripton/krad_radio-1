#include "krad_radio_payload_to_ebml.h"

int kr_nil_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_nil *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_nil *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, actual->n);

  return res;
}

int kr_radio_payload_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_radio_payload_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_radio_payload_type *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}

int kr_radio_payload_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_radio_payload *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_radio_payload *)uber_actual->actual;

  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->nil);
      uber.type = EBML_KR_NIL;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->transponder_path_info);
      uber.type = EBML_KR_TRANSPONDER_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = EBML_KR_MIXER_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = EBML_KR_COMPOSITOR_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

