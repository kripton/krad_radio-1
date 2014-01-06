#include "krad_app_to_ebml.h"

int kr_radio_payload_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_radio_payload_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_radio_payload_type *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, *actual);

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
      uber.actual = &(actual->transponder_path_info);
      uber.type = EBML_KR_TRANSPONDER_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = EBML_KR_MIXER_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

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
  uber_St uber_sub;
  int index;
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
  uber.actual = &(actual->payload_type);
  uber.type = EBML_KR_RADIO_PAYLOAD_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  index = kr_radio_payload_type_to_index(actual->payload_type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->payload);
  uber.actual = &(uber_sub);
  uber.type = EBML_KR_RADIO_PAYLOAD;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}

