#include "krad_transponder_common_to_text.h"

int kr_transponder_path_io_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_transponder_path_io_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_transponder_path_io_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_transponder_path_io_type(*actual));

  return res;
}
int kr_transponder_path_io_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_transponder_path_io_path_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_transponder_path_io_path_info *)uber_actual->actual;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
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
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_io_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_transponder_path_io_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_PATH_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
int kr_transponder_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
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
  res += snprintf(&text[res],max-res,"%sinput:\n",indent);
  uber.actual = &(actual->input);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%soutput:\n",indent);
  uber.actual = &(actual->output);
  uber.type = TEXT_KR_TRANSPONDER_PATH_IO_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
