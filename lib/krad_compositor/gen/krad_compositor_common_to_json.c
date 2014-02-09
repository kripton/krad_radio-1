#include "krad_compositor_common_to_json.h"

int kr_compositor_path_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_compositor_path_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_path_type *)st;
  type = kr_strfr_kr_compositor_path_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_compositor_overlay_type_to_json(char *json, void *st, int32_t max) {
  char *type;
  int res;
  kr_compositor_overlay_type *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_overlay_type *)st;
  type = kr_strfr_kr_compositor_overlay_type(*actual);
  res += snprintf(&json[res],max-res,"\"%s\"",type);

  return res;
}
int kr_rect_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_rect *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_rect *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"x\" : %d,",actual->x);
  res += snprintf(&json[res],max-res,"\"y\" : %d,",actual->y);
  res += snprintf(&json[res],max-res,"\"w\" : %d,",actual->w);
  res += snprintf(&json[res],max-res,"\"h\" : %d",actual->h);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_compositor_output_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_compositor_output_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_output_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"w\" : %d,",actual->w);
  res += snprintf(&json[res],max-res,"\"h\" : %d,",actual->h);
  res += snprintf(&json[res],max-res,"\"opacity\" : %0.2f",actual->opacity);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_compositor_bus_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_compositor_bus_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_bus_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"opacity\" : %0.2f",actual->opacity);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_compositor_input_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_compositor_input_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_input_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"crop\": ");
  uber.actual = &(actual->crop);
  uber.type = JSON_KR_RECT;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"pos\": ");
  uber.actual = &(actual->pos);
  uber.type = JSON_KR_RECT;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"view\": ");
  uber.actual = &(actual->view);
  uber.type = JSON_KR_PERSPECTIVE_VIEW;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  res += snprintf(&json[res],max-res,"\"z\" : %u,",actual->z);
  res += snprintf(&json[res],max-res,"\"rotation\" : %0.2f,",actual->rotation);
  res += snprintf(&json[res],max-res,"\"opacity\" : %0.2f",actual->opacity);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_compositor_source_info_to_json(char *json, void *st, int32_t max) {
  int res;
  struct kr_compositor_source_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_source_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"w\" : %d,",actual->w);
  res += snprintf(&json[res],max-res,"\"h\" : %d",actual->h);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_overlay_type_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_overlay_type_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_overlay_type_info *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->text);
      uber.type = JSON_KR_TEXT_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->vector);
      uber.type = JSON_KR_VECTOR_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->sprite);
      uber.type = JSON_KR_SPRITE_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_overlay_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_overlay_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_overlay_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_COMPOSITOR_OVERLAY_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_compositor_overlay_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_OVERLAY_TYPE_INFO;
  res += snprintf(&json[res],max-res,"\"info\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
int kr_compositor_path_type_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_compositor_path_type_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_compositor_path_type_info *)uber_actual->actual;
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->output_info);
      uber.type = JSON_KR_COMPOSITOR_OUTPUT_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->bus_info);
      uber.type = JSON_KR_COMPOSITOR_BUS_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->input_info);
      uber.type = JSON_KR_COMPOSITOR_INPUT_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->source_info);
      uber.type = JSON_KR_COMPOSITOR_SOURCE_INFO;
      res += info_pack_to_json(&json[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_compositor_path_info_to_json(char *json, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_compositor_path_info *actual;
  res = 0;

  if ((json == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;
  res += snprintf(&json[res],max-res,"{");
  res += snprintf(&json[res],max-res,"\"type\": ");
  uber.actual = &(actual->type);
  uber.type = JSON_KR_COMPOSITOR_PATH_TYPE;
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,",");
  index = kr_compositor_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = JSON_KR_COMPOSITOR_PATH_TYPE_INFO;
  res += snprintf(&json[res],max-res,"\"info\": ");
  res += info_pack_to_json(&json[res],&uber,max-res);
  res += snprintf(&json[res],max-res,"}");

  return res;
}
