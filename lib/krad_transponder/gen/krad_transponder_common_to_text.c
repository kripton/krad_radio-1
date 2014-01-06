#include "krad_transponder_common_to_text.h"

int krad_link_av_mode_t_to_text(char *text, void *st, int32_t max) {
  int res;
  krad_link_av_mode_t *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_link_av_mode_t *)st;

  res += snprintf(&text[res],max-res,"krad_link_av_mode_t : %u \n",*actual);

  return res;
}

int kr_txpdr_su_type_t_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_txpdr_su_type_t *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_txpdr_su_type_t *)st;

  res += snprintf(&text[res],max-res,"kr_txpdr_su_type_t : %u \n",*actual);

  return res;
}

int krad_link_transport_mode_t_to_text(char *text, void *st, int32_t max) {
  int res;
  krad_link_transport_mode_t *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (krad_link_transport_mode_t *)st;

  res += snprintf(&text[res],max-res,"krad_link_transport_mode_t : %u \n",*actual);

  return res;
}

int kr_transponder_path_io_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_transponder_path_io_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_transponder_path_io_type *)st;

  res += snprintf(&text[res],max-res,"kr_transponder_path_io_type : %u \n",*actual);

  return res;
}

int kr_transponder_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_transponder_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_info *)st;

  res += snprintf(&text[res],max-res,"active_paths : %u \n",actual->active_paths);

  return res;
}

int kr_transponder_path_io_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_transponder_path_io_path_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;

  if (uber_actual->actual == NULL) {
    return -1;
  }

  actual = (kr_transponder_path_io_path_info *)uber_actual->actual;

  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->mixer_path_info);
      uber.type = TEXT_KR_MIXER_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = TEXT_KR_COMPOSITOR_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->adapter_path_info);
      uber.type = TEXT_KR_ADAPTER_PATH_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}

int kr_transponder_path_io_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_transponder_path_io_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_io_info *)st;

  uber.actual = &(actual->type);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_TYPE;
  res += info_pack_to_text(&text[res],&uber,max-res);
  index = kr_transponder_path_io_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_PATH_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

int kr_transponder_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_transponder_path_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;

  res += snprintf(&text[res],max-res,"name : %s \n",actual->name);
  uber.actual = &(actual->input);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->output);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

