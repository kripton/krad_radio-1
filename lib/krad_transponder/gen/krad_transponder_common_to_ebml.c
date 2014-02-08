#include "krad_transponder_common_to_ebml.h"

int kr_transponder_path_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_transponder_path_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_transponder_path_type *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}
int kr_transponder_path_type_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_transponder_path_type_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
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
      uber.type = EBML_KR_JACK_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->jack_input);
      uber.type = EBML_KR_JACK_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->jack_output);
      uber.type = EBML_KR_JACK_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->wayland);
      uber.type = EBML_KR_WAYLAND_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 4: {
      uber.actual = &(actual->wayland_output);
      uber.type = EBML_KR_WAYLAND_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 5: {
      uber.actual = &(actual->v4l2);
      uber.type = EBML_KR_V4L2_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 6: {
      uber.actual = &(actual->v4l2_input);
      uber.type = EBML_KR_V4L2_OPEN_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 7: {
      uber.actual = &(actual->decklink);
      uber.type = EBML_KR_DECKLINK_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 8: {
      uber.actual = &(actual->decklink_input);
      uber.type = EBML_KR_DECKLINK_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 9: {
      uber.actual = &(actual->x11);
      uber.type = EBML_KR_X11_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 10: {
      uber.actual = &(actual->x11_input);
      uber.type = EBML_KR_X11_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 11: {
      uber.actual = &(actual->alsa);
      uber.type = EBML_KR_ALSA_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 12: {
      uber.actual = &(actual->alsa_input);
      uber.type = EBML_KR_ALSA_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 13: {
      uber.actual = &(actual->alsa_output);
      uber.type = EBML_KR_ALSA_PATH_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}
int kr_transponder_path_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_transponder_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;
  uber.actual = &(actual->type);
  uber.type = EBML_KR_TRANSPONDER_PATH_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  index = kr_transponder_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->aio);
  uber.actual = &(uber_sub);
  uber.type = EBML_KR_TRANSPONDER_PATH_TYPE_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}
