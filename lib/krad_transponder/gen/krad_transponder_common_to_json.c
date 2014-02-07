#include "krad_transponder_common_to_json.h"

int kr_transponder_path_io_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_transponder_path_io_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_transponder_path_io_type *)st;
  type = kr_strfr_kr_transponder_path_io_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_transponder_path_io_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_transponder_path_io_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
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
      uber.type = JSON_KR_MIXER_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->compositor_path_info);
      uber.type = JSON_KR_COMPOSITOR_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->adapter_path_info);
      uber.type = JSON_KR_ADAPTER_PATH_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_transponder_path_io_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_transponder_path_io_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_io_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_TRANSPONDER_PATH_IO_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_transponder_path_io_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_TRANSPONDER_PATH_IO_PATH_INFO;
  res += snprintf(&json[res],max-res,"\"info\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_transponder_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_transponder_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_transponder_path_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"input\": ");
  uber.actual = &(actual->input);
  uber.type = JSON_KR_TRANSPONDER_PATH_IO_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"output\": ");
  uber.actual = &(actual->output);
  uber.type = JSON_KR_TRANSPONDER_PATH_IO_INFO;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
