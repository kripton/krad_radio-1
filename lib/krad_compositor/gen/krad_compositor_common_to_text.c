#include "krad_compositor_common_to_text.h"

int kr_compositor_path_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_compositor_path_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_path_type *)st;

  res += snprintf(&text[res],max-res,"kr_compositor_path_type : %u \n",*actual);

  return res;
}

int kr_compositor_overlay_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_compositor_overlay_type *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_overlay_type *)st;

  res += snprintf(&text[res],max-res,"kr_compositor_overlay_type : %u \n",*actual);

  return res;
}

int kr_rect_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_rect *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_rect *)st;


  return res;
}

int kr_compositor_output_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_output_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_output_info *)st;

  res += snprintf(&text[res],max-res,"opacity : %0.2f \n",actual->opacity);

  return res;
}

int kr_compositor_bus_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_bus_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_bus_info *)st;

  res += snprintf(&text[res],max-res,"opacity : %0.2f \n",actual->opacity);

  return res;
}

int kr_compositor_input_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_compositor_input_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_input_info *)st;

  uber.actual = &(actual->crop);
  uber.type = TEXT_KR_RECT;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->pos);
  uber.type = TEXT_KR_RECT;
  res += info_pack_to_text(&text[res],&uber,max-res);
  uber.actual = &(actual->view);
  uber.type = TEXT_KR_PERSPECTIVE_VIEW;
  res += info_pack_to_text(&text[res],&uber,max-res);
  res += snprintf(&text[res],max-res,"z : %u \n",actual->z);
  res += snprintf(&text[res],max-res,"rotation : %0.2f \n",actual->rotation);
  res += snprintf(&text[res],max-res,"opacity : %0.2f \n",actual->opacity);

  return res;
}

int kr_compositor_source_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_source_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_source_info *)st;


  return res;
}

int kr_compositor_path_type_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_compositor_path_type_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
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
      uber.type = TEXT_KR_COMPOSITOR_OUTPUT_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->bus_info);
      uber.type = TEXT_KR_COMPOSITOR_BUS_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->input_info);
      uber.type = TEXT_KR_COMPOSITOR_INPUT_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 3: {
      uber.actual = &(actual->source_info);
      uber.type = TEXT_KR_COMPOSITOR_SOURCE_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}

int kr_compositor_path_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_compositor_path_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;

  uber.actual = &(actual->type);
  uber.type = TEXT_KR_COMPOSITOR_PATH_TYPE;
  res += info_pack_to_text(&text[res],&uber,max-res);
  index = kr_compositor_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_COMPOSITOR_PATH_TYPE_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

int kr_sprite_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_sprite_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;

  res += snprintf(&text[res],max-res,"filename : %s \n",actual->filename);
  res += snprintf(&text[res],max-res,"rate : %d \n",actual->rate);
  uber.actual = &(actual->input_info);
  uber.type = TEXT_KR_COMPOSITOR_INPUT_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

int kr_text_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += snprintf(&text[res],max-res,"string : %s \n",actual->string);
  res += snprintf(&text[res],max-res,"font : %s \n",actual->font);
  res += snprintf(&text[res],max-res,"red : %0.2f \n",actual->red);
  res += snprintf(&text[res],max-res,"green : %0.2f \n",actual->green);
  res += snprintf(&text[res],max-res,"blue : %0.2f \n",actual->blue);
  uber.actual = &(actual->input_info);
  uber.type = TEXT_KR_COMPOSITOR_INPUT_INFO;
  res += info_pack_to_text(&text[res],&uber,max-res);

  return res;
}

