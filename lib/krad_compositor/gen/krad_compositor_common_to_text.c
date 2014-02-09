#include "krad_compositor_common_to_text.h"

int kr_compositor_path_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_compositor_path_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_path_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_compositor_path_type(*actual));

  return res;
}
int kr_compositor_overlay_type_to_text(char *text, void *st, int32_t max) {
  int res;
  kr_compositor_overlay_type *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (kr_compositor_overlay_type *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res," %s \n",kr_strfr_kr_compositor_overlay_type(*actual));

  return res;
}
int kr_rect_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_rect *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_rect *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sx: %d \n",indent,actual->x);
  res += snprintf(&text[res],max-res,"%sy: %d \n",indent,actual->y);
  res += snprintf(&text[res],max-res,"%sw: %d \n",indent,actual->w);
  res += snprintf(&text[res],max-res,"%sh: %d \n",indent,actual->h);

  return res;
}
int kr_compositor_output_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_output_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_output_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sw: %d \n",indent,actual->w);
  res += snprintf(&text[res],max-res,"%sh: %d \n",indent,actual->h);
  res += snprintf(&text[res],max-res,"%sopacity: %0.2f \n",indent,actual->opacity);

  return res;
}
int kr_compositor_bus_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_bus_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_bus_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sopacity: %0.2f \n",indent,actual->opacity);

  return res;
}
int kr_compositor_input_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  struct kr_compositor_input_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_input_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%scrop:\n",indent);
  uber.actual = &(actual->crop);
  uber.type = TEXT_KR_RECT;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%spos:\n",indent);
  uber.actual = &(actual->pos);
  uber.type = TEXT_KR_RECT;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sview:\n",indent);
  uber.actual = &(actual->view);
  uber.type = TEXT_KR_PERSPECTIVE_VIEW;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  res += snprintf(&text[res],max-res,"%sz: %u \n",indent,actual->z);
  res += snprintf(&text[res],max-res,"%srotation: %0.2f \n",indent,actual->rotation);
  res += snprintf(&text[res],max-res,"%sopacity: %0.2f \n",indent,actual->opacity);

  return res;
}
int kr_compositor_source_info_to_text(char *text, void *st, int32_t max) {
  int res;
  struct kr_compositor_source_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_source_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%sw: %d \n",indent,actual->w);
  res += snprintf(&text[res],max-res,"%sh: %d \n",indent,actual->h);

  return res;
}
int kr_overlay_type_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_overlay_type_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_overlay_type_info *)uber_actual->actual;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  switch (uber_actual->type) {
    case 0: {
      uber.actual = &(actual->text);
      uber.type = TEXT_KR_TEXT_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 1: {
      uber.actual = &(actual->vector);
      uber.type = TEXT_KR_VECTOR_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
    case 2: {
      uber.actual = &(actual->sprite);
      uber.type = TEXT_KR_SPRITE_INFO;
      res += info_pack_to_text(&text[res],&uber,max-res);
      break;
    }
  }


  return res;
}
int kr_overlay_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_overlay_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_overlay_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_COMPOSITOR_OVERLAY_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_compositor_overlay_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_OVERLAY_TYPE_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
int kr_compositor_path_type_info_to_text(char *text, void *st, int32_t max) {
  uber_St uber;
  int res;
  uber_St *uber_actual;
  kr_compositor_path_type_info *actual;
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  uber_actual = (uber_St *)st;
  if (uber_actual->actual == NULL) {
    return -1;
  }
  actual = (kr_compositor_path_type_info *)uber_actual->actual;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
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
  char indent[(depth_state*2)+1];
  res = 0;

  if ((text == NULL) || (st == NULL) || (max < 1)) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;
  memset(indent,' ',depth_state*2);
  indent[depth_state*2] = '\0';
  res += snprintf(&text[res],max-res,"%stype:",indent);
  uber.actual = &(actual->type);
  uber.type = TEXT_KR_COMPOSITOR_PATH_TYPE;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;
  index = kr_compositor_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = TEXT_KR_COMPOSITOR_PATH_TYPE_INFO;
  depth_state++;
  res += info_pack_to_text(&text[res],&uber,max-res);
  depth_state--;

  return res;
}
