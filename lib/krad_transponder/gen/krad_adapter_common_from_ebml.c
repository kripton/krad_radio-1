#include "krad_adapter_common_from_ebml.h"

int kr_adapter_path_direction_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_adapter_path_direction *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_adapter_path_direction *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_adapter_api_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_adapter_api *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_adapter_api *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_adapter_api_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_adapter_api_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
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
      uber.type = DEBML_KR_JACK_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
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
      uber.actual = &(actual->decklink);
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
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = DEBML_KR_ALSA_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

int kr_adapter_api_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_adapter_api_path_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
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
      uber.type = DEBML_KR_JACK_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->wayland);
      uber.type = DEBML_KR_WAYLAND_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->v4l2);
      uber.type = DEBML_KR_V4L2_OPEN_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->decklink);
      uber.type = DEBML_KR_DECKLINK_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 4: {
      uber.actual = &(actual->x11);
      uber.type = DEBML_KR_X11_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 5: {
      uber.actual = &(actual->alsa);
      uber.type = DEBML_KR_ALSA_PATH_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

int kr_adapter_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_adapter_info *)st;

  uber.actual = &(actual->api);
  uber.type = DEBML_KR_ADAPTER_API;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->api_info);
  uber.actual = &(uber_sub);
  uber.type = DEBML_KR_ADAPTER_API_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

int kr_adapter_path_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_adapter_path_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_adapter_path_info *)st;

  res += kr_ebml2_unpack_element_string(ebml, NULL, actual->name , sizeof(actual->name));
  uber.actual = &(actual->dir);
  uber.type = DEBML_KR_ADAPTER_PATH_DIRECTION;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->api);
  uber.type = DEBML_KR_ADAPTER_API;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  index = kr_adapter_api_to_index(actual->api);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = DEBML_KR_ADAPTER_API_PATH_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

