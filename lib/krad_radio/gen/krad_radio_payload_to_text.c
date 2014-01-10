#include "krad_radio_payload_to_text.h"

int kr_radio_payload_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_radio_payload_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_radio_payload_type *)st;

  res += snprintf(&text[res],max-res,"kr_radio_payload_type : %u \n",*actual);

  return res;
}

int kr_radio_payload_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_radio_payload *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
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
      uber.type = TEXT_KR_TRANSPONDER_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = TEXT_KR_MIXER_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = TEXT_KR_COMPOSITOR_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}

