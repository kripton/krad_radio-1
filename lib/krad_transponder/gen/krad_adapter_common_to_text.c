#include "krad_adapter_common_to_text.h"

int kr_adapter_path_direction_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_adapter_path_direction *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_adapter_path_direction *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_adapter_path_direction(*actual));

  return res;
}
int kr_adapter_api_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_adapter_api *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_adapter_api *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_adapter_api(*actual));

  return res;
}
int kr_adapter_api_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_adapter_api_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_adapter_api_info *)uber_actual->actual;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = TEXT_KR_JACK_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = TEXT_KR_WAYLAND_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = TEXT_KR_V4L2_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = TEXT_KR_DECKLINK_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = TEXT_KR_X11_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = TEXT_KR_ALSA_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_adapter_api_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_adapter_api_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_adapter_api_path_info *)uber_actual->actual;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = TEXT_KR_JACK_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = TEXT_KR_WAYLAND_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = TEXT_KR_V4L2_OPEN_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = TEXT_KR_DECKLINK_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = TEXT_KR_X11_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = TEXT_KR_ALSA_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_adapter_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_adapter_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sapi:",indent);
  uber.actual = &(actual->api);
  uber.type = TEXT_KR_ADAPTER_API;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->api_info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_ADAPTER_API_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
int kr_adapter_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_adapter_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sname: %s \n",indent,actual->name);
  res += snprintf(&text[res],max-res,"%sdir:",indent);
  uber.actual = &(actual->dir);
  uber.type = TEXT_KR_ADAPTER_PATH_DIRECTION;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sapi:",indent);
  uber.actual = &(actual->api);
  uber.type = TEXT_KR_ADAPTER_API;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_ADAPTER_API_PATH_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
