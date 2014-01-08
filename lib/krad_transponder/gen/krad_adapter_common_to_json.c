#include "krad_adapter_common_to_json.h"

int kr_adapter_path_direction_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_adapter_path_direction *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_adapter_path_direction *)st;

  type = kr_strfr_kr_adapter_path_direction(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}

int kr_adapter_api_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_adapter_api *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_adapter_api *)st;

  type = kr_strfr_kr_adapter_api(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}

int kr_adapter_api_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_adapter_api_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_adapter_api_info *)uber_actual->actual;

  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = JSON_KR_JACK_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = JSON_KR_WAYLAND_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = JSON_KR_V4L2_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = JSON_KR_DECKLINK_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = JSON_KR_X11_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}

int kr_adapter_api_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_adapter_api_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_adapter_api_path_info *)uber_actual->actual;

  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = JSON_KR_JACK_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = JSON_KR_WAYLAND_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = JSON_KR_V4L2_OPEN_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = JSON_KR_DECKLINK_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = JSON_KR_X11_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = JSON_KR_ALSA_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}

int kr_adapter_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_adapter_info *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"api\": ");
  uber.actual = &(actual->api);
  uber.type = JSON_KR_ADAPTER_API;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->api_info);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_ADAPTER_API_INFO;
  res += snprintf(&json[res],max-res,"\"api_info\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

int kr_adapter_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_path_info *actual;

  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_adapter_path_info *)st;

  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"name\" : \"%s\",",actual->name);
  res += snprintf(&json[res],max-res,"\"dir\": ");
  uber.actual = &(actual->dir);
  uber.type = JSON_KR_ADAPTER_PATH_DIRECTION;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"api\": ");
  uber.actual = &(actual->api);
  uber.type = JSON_KR_ADAPTER_API;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_ADAPTER_API_PATH_INFO;
  res += snprintf(&json[res],max-res,"\"info\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}

