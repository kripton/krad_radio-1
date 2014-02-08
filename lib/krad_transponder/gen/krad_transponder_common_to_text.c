#include "krad_transponder_common_to_text.h"

int kr_transponder_path_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_transponder_path_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_transponder_path_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_transponder_path_type(*actual));

  return res;
}
int kr_transponder_path_type_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_transponder_path_type_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_transponder_path_type_info *)uber_actual->actual;
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
      uber.actual = &(actual->jack_input);
      uber.type = TEXT_KR_JACK_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->jack_output);
      uber.type = TEXT_KR_JACK_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->wayland);
      uber.type = TEXT_KR_WAYLAND_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 4: {
      uber.actual = &(actual->wayland_output);
      uber.type = TEXT_KR_WAYLAND_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 5: {
      uber.actual = &(actual->v4l2);
      uber.type = TEXT_KR_V4L2_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 6: {
      uber.actual = &(actual->v4l2_input);
      uber.type = TEXT_KR_V4L2_OPEN_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 7: {
      uber.actual = &(actual->decklink);
      uber.type = TEXT_KR_DECKLINK_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 8: {
      uber.actual = &(actual->decklink_input);
      uber.type = TEXT_KR_DECKLINK_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 9: {
      uber.actual = &(actual->x11);
      uber.type = TEXT_KR_X11_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 10: {
      uber.actual = &(actual->x11_input);
      uber.type = TEXT_KR_X11_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 11: {
      uber.actual = &(actual->alsa);
      uber.type = TEXT_KR_ALSA_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 12: {
      uber.actual = &(actual->alsa_input);
      uber.type = TEXT_KR_ALSA_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 13: {
      uber.actual = &(actual->alsa_output);
      uber.type = TEXT_KR_ALSA_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_transponder_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_transponder_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_TRANSPONDER_PATH_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_transponder_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->aio);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_TRANSPONDER_PATH_TYPE_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
