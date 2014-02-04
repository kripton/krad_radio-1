#include "krad_compositor_common_to_ebml.h"

int kr_compositor_path_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_compositor_path_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_compositor_path_type *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}

int kr_compositor_overlay_type_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_compositor_overlay_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_compositor_overlay_type *)st;

  res += kr_ebml_pack_int32(ebml, 0xe1, (int32_t)*actual);

  return res;
}

int kr_rect_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_rect *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_rect *)st;


  return res;
}

int kr_compositor_output_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_output_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_output_info *)st;

  res += kr_ebml_pack_float(ebml, 0xe1, actual->opacity);

  return res;
}

int kr_compositor_bus_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_bus_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_bus_info *)st;

  res += kr_ebml_pack_float(ebml, 0xe1, actual->opacity);

  return res;
}

int kr_compositor_input_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_compositor_input_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_input_info *)st;

  uber.actual = &(actual->crop);
  uber.type = EBML_KR_RECT;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->pos);
  uber.type = EBML_KR_RECT;
  res += info_pack_to_ebml(&ebml[res],&uber);
  uber.actual = &(actual->view);
  uber.type = EBML_KR_PERSPECTIVE_VIEW;
  res += info_pack_to_ebml(&ebml[res],&uber);
  res += kr_ebml_pack_uint32(ebml, 0xe1, actual->z);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->rotation);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->opacity);

  return res;
}

int kr_compositor_source_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_source_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_source_info *)st;


  return res;
}

int kr_compositor_path_type_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_compositor_path_type_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
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
      uber.type = EBML_KR_COMPOSITOR_OUTPUT_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->bus_info);
      uber.type = EBML_KR_COMPOSITOR_BUS_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->input_info);
      uber.type = EBML_KR_COMPOSITOR_INPUT_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->source_info);
      uber.type = EBML_KR_COMPOSITOR_SOURCE_INFO;
      res += info_pack_to_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

int kr_compositor_path_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_compositor_path_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;

  uber.actual = &(actual->type);
  uber.type = EBML_KR_COMPOSITOR_PATH_TYPE;
  res += info_pack_to_ebml(&ebml[res],&uber);
  index = kr_compositor_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = EBML_KR_COMPOSITOR_PATH_TYPE_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}

int kr_sprite_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_sprite_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;

  res += kr_ebml_pack_string(ebml, 0xe1, actual->filename);
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->rate);
  uber.actual = &(actual->input_info);
  uber.type = EBML_KR_COMPOSITOR_INPUT_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}

int kr_text_info_to_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_text_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_text_info *)st;

  res += kr_ebml_pack_string(ebml, 0xe1, actual->string);
  res += kr_ebml_pack_string(ebml, 0xe1, actual->font);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->red);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->green);
  res += kr_ebml_pack_float(ebml, 0xe1, actual->blue);
  uber.actual = &(actual->input_info);
  uber.type = EBML_KR_COMPOSITOR_INPUT_INFO;
  res += info_pack_to_ebml(&ebml[res],&uber);

  return res;
}

