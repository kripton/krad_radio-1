#include "krad_compositor_common_from_ebml.h"

int kr_compositor_path_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_compositor_path_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_compositor_path_type *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_compositor_overlay_type_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  kr_compositor_overlay_type *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (kr_compositor_overlay_type *)st;

  res += kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)actual);

  return res;
}

int kr_rect_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_rect *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_rect *)st;


  return res;
}

int kr_compositor_output_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_output_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_output_info *)st;

  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->opacity);

  return res;
}

int kr_compositor_bus_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_bus_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_bus_info *)st;

  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->opacity);

  return res;
}

int kr_compositor_input_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  struct kr_compositor_input_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_input_info *)st;

  uber.actual = &(actual->crop);
  uber.type = DEBML_KR_RECT;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->pos);
  uber.type = DEBML_KR_RECT;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  uber.actual = &(actual->view);
  uber.type = DEBML_KR_PERSPECTIVE_VIEW;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  res += kr_ebml2_unpack_element_uint32(ebml, NULL, &actual->z);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->rotation);
  res += kr_ebml2_unpack_element_float(ebml, NULL, &actual->opacity);

  return res;
}

int kr_compositor_source_info_fr_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_compositor_source_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_compositor_source_info *)st;


  return res;
}

int kr_overlay_type_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  int res;
  uber_St *uber_actual;

  kr_overlay_type_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
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
      uber.type = DEBML_KR_TEXT_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->vector);
      uber.type = DEBML_KR_VECTOR_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->sprite);
      uber.type = DEBML_KR_SPRITE_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

int kr_overlay_info_fr_ebml(kr_ebml *ebml, void *st) {
  uber_St uber;
  uber_St uber_sub;
  int index;
  int res;
  struct kr_overlay_info *actual;

  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_overlay_info *)st;

  uber.actual = &(actual->type);
  uber.type = DEBML_KR_COMPOSITOR_OVERLAY_TYPE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  index = kr_compositor_overlay_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = DEBML_KR_OVERLAY_TYPE_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

int kr_compositor_path_type_info_fr_ebml(kr_ebml *ebml, void *st) {
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
      uber.type = DEBML_KR_COMPOSITOR_OUTPUT_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 1: {
      uber.actual = &(actual->bus_info);
      uber.type = DEBML_KR_COMPOSITOR_BUS_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 2: {
      uber.actual = &(actual->input_info);
      uber.type = DEBML_KR_COMPOSITOR_INPUT_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
    case 3: {
      uber.actual = &(actual->source_info);
      uber.type = DEBML_KR_COMPOSITOR_SOURCE_INFO;
      res += info_unpack_fr_ebml(&ebml[res],&uber);
      break;
    }
  }


  return res;
}

int kr_compositor_path_info_fr_ebml(kr_ebml *ebml, void *st) {
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
  uber.type = DEBML_KR_COMPOSITOR_PATH_TYPE;
  res += info_unpack_fr_ebml(&ebml[res],&uber);
  index = kr_compositor_path_type_to_index(actual->type);
  uber_sub.type = index;
  uber_sub.actual = &(actual->info);
  uber.actual = &(uber_sub);
  uber.type = DEBML_KR_COMPOSITOR_PATH_TYPE_INFO;
  res += info_unpack_fr_ebml(&ebml[res],&uber);

  return res;
}

