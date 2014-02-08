#include "kr_xpdr_common_from_ebml.h"

int kr_xpdr_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_xpdr_type *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_xpdr_type *)st;
  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}
int kr_xpdr_type_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_xpdr_type_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_xpdr_type_info *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->jack);
      uber.type = DEBML_KR_JACK_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->wl);
      uber.type = DEBML_KR_WAYLAND_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = DEBML_KR_V4L2_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->dl);
      uber.type = DEBML_KR_DECKLINK_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = DEBML_KR_X11_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 7: {
      uber.actual = &(actual->jack_in);
      uber.type = DEBML_KR_JACK_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 8: {
      uber.actual = &(actual->jack_out);
      uber.type = DEBML_KR_JACK_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 9: {
      uber.actual = &(actual->wl_out);
      uber.type = DEBML_KR_WAYLAND_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 10: {
      uber.actual = &(actual->v4l2_in);
      uber.type = DEBML_KR_V4L2_OPEN_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 11: {
      uber.actual = &(actual->dl_in);
      uber.type = DEBML_KR_DECKLINK_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 12: {
      uber.actual = &(actual->x11_in);
      uber.type = DEBML_KR_X11_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 13: {
      uber.actual = &(actual->alsa_in);
      uber.type = DEBML_KR_ALSA_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 14: {
      uber.actual = &(actual->alsa_out);
      uber.type = DEBML_KR_ALSA_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}
int kr_xpdr_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_xpdr_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_xpdr_path_info *)st;

  return res;
}
