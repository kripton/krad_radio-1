#include "krad_vector_common_helpers.h"

kr_vector_info_member kr_vector_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_VECTOR_INFO_TYPE;
  }
  if (!strncmp(string,"red",len)) {
    return KR_VECTOR_INFO_RED;
  }
  if (!strncmp(string,"green",len)) {
    return KR_VECTOR_INFO_GREEN;
  }
  if (!strncmp(string,"blue",len)) {
    return KR_VECTOR_INFO_BLUE;
  }
  if (!strncmp(string,"input_info",len)) {
    return KR_VECTOR_INFO_INPUT_INFO;
  }
  return -1;
}

int kr_vector_type_to_index(int val) {
  switch (val) {
    case NOTHING:
      return 0;
    case HEX:
      return 1;
    case CIRCLE:
      return 2;
    case RECT:
      return 3;
    case TRIANGLE:
      return 4;
    case VIPER:
      return 5;
    case METER:
      return 6;
    case GRID:
      return 7;
    case CURVE:
      return 8;
    case ARROW:
      return 9;
    case CLOCK:
      return 10;
    case SHADOW:
      return 11;
  }
  return -1;
}

char *kr_strfr_kr_vector_type(int val) {
  switch (val) {
    case NOTHING:
      return "nothing";
    case HEX:
      return "hex";
    case CIRCLE:
      return "circle";
    case RECT:
      return "rect";
    case TRIANGLE:
      return "triangle";
    case VIPER:
      return "viper";
    case METER:
      return "meter";
    case GRID:
      return "grid";
    case CURVE:
      return "curve";
    case ARROW:
      return "arrow";
    case CLOCK:
      return "clock";
    case SHADOW:
      return "shadow";
  }
  return NULL;
}

int kr_strto_kr_vector_type(char *string) {
  if (!strcmp(string,"nothing")) {
    return NOTHING;
  }
  if (!strcmp(string,"hex")) {
    return HEX;
  }
  if (!strcmp(string,"circle")) {
    return CIRCLE;
  }
  if (!strcmp(string,"rect")) {
    return RECT;
  }
  if (!strcmp(string,"triangle")) {
    return TRIANGLE;
  }
  if (!strcmp(string,"viper")) {
    return VIPER;
  }
  if (!strcmp(string,"meter")) {
    return METER;
  }
  if (!strcmp(string,"grid")) {
    return GRID;
  }
  if (!strcmp(string,"curve")) {
    return CURVE;
  }
  if (!strcmp(string,"arrow")) {
    return ARROW;
  }
  if (!strcmp(string,"clock")) {
    return CLOCK;
  }
  if (!strcmp(string,"shadow")) {
    return SHADOW;
  }
  return -1;
}

int kr_vector_info_patch_apply(struct kr_vector_info *info, kr_vector_info_patch *patch) {
  const ptrdiff_t off[5] = { offsetof(struct kr_vector_info, type), 
    offsetof(struct kr_vector_info, red), offsetof(struct kr_vector_info, green), 
    offsetof(struct kr_vector_info, blue), offsetof(struct kr_vector_info, input_info)
  };
  const size_t sz[5] = { sizeof(info->type), 
    sizeof(info->red), sizeof(info->green), 
    sizeof(info->blue), sizeof(info->input_info)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_vector_info_patch_path(kr_vector_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_vector_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    case KR_VECTOR_INFO_INPUT_INFO:
       if (kr_path_step(path) != 0) return NULL;
       return kr_compositor_input_info_patch_path(&patch->value.input_info_patch, path);
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_vector_info_init(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_vector_info));
  kr_compositor_input_info_init(&st->input_info);

  return 0;
}

int kr_vector_info_valid(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_compositor_input_info_valid(&st->input_info);

  return 0;
}

int kr_vector_info_random(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_vector_info));
  kr_compositor_input_info_random(&st->input_info);

  return 0;
}

