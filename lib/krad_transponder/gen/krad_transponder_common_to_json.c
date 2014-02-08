#include "krad_transponder_common_to_json.h"

int kr_transponder_path_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_transponder_path_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_transponder_path_type *)st;
  type = kr_strfr_kr_transponder_path_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_transponder_path_type_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_transponder_path_type_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_transponder_path_type_info *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = JSON_KR_JACK_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->jack_input);
      uber.type = JSON_KR_JACK_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->jack_output);
      uber.type = JSON_KR_JACK_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->wayland);
      uber.type = JSON_KR_WAYLAND_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->wayland_output);
      uber.type = JSON_KR_WAYLAND_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 5: {
      uber.actual = &(actual->v4l2);
      uber.type = JSON_KR_V4L2_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 6: {
      uber.actual = &(actual->v4l2_input);
      uber.type = JSON_KR_V4L2_OPEN_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 7: {
      uber.actual = &(actual->decklink);
      uber.type = JSON_KR_DECKLINK_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 8: {
      uber.actual = &(actual->decklink_input);
      uber.type = JSON_KR_DECKLINK_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 9: {
      uber.actual = &(actual->x11);
      uber.type = JSON_KR_X11_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 10: {
      uber.actual = &(actual->x11_input);
      uber.type = JSON_KR_X11_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 11: {
      uber.actual = &(actual->alsa);
      uber.type = JSON_KR_ALSA_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 12: {
      uber.actual = &(actual->alsa_input);
      uber.type = JSON_KR_ALSA_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 13: {
      uber.actual = &(actual->alsa_output);
      uber.type = JSON_KR_ALSA_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_transponder_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_transponder_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_TRANSPONDER_PATH_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_transponder_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->aio);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_TRANSPONDER_PATH_TYPE_INFO;
  res += snprintf(&json[res],max-res,"\"aio\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
