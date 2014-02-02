#include "krad_compositor_common_helpers.h"

kr_sprite_info_member kr_sprite_info_strto_member(char *string, int len) {
  if (!strncmp(string,"filename",len)) {
    return KR_SPRITE_INFO_FILENAME;
  }
  if (!strncmp(string,"rate",len)) {
    return KR_SPRITE_INFO_RATE;
  }
  if (!strncmp(string,"controls",len)) {
    return KR_SPRITE_INFO_CONTROLS;
  }
  return -1;
}

kr_text_info_member kr_text_info_strto_member(char *string, int len) {
  if (!strncmp(string,"string",len)) {
    return KR_TEXT_INFO_STRING;
  }
  if (!strncmp(string,"font",len)) {
    return KR_TEXT_INFO_FONT;
  }
  if (!strncmp(string,"red",len)) {
    return KR_TEXT_INFO_RED;
  }
  if (!strncmp(string,"green",len)) {
    return KR_TEXT_INFO_GREEN;
  }
  if (!strncmp(string,"blue",len)) {
    return KR_TEXT_INFO_BLUE;
  }
  if (!strncmp(string,"controls",len)) {
    return KR_TEXT_INFO_CONTROLS;
  }
  return -1;
}

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
  if (!strncmp(string,"controls",len)) {
    return KR_VECTOR_INFO_CONTROLS;
  }
  return -1;
}

kr_compositor_path_info_member kr_compositor_path_info_strto_member(char *string, int len) {
  if (!strncmp(string,"type",len)) {
    return KR_COMPOSITOR_PATH_INFO_TYPE;
  }
  if (!strncmp(string,"width",len)) {
    return KR_COMPOSITOR_PATH_INFO_WIDTH;
  }
  if (!strncmp(string,"height",len)) {
    return KR_COMPOSITOR_PATH_INFO_HEIGHT;
  }
  if (!strncmp(string,"crop_x",len)) {
    return KR_COMPOSITOR_PATH_INFO_CROP_X;
  }
  if (!strncmp(string,"crop_y",len)) {
    return KR_COMPOSITOR_PATH_INFO_CROP_Y;
  }
  if (!strncmp(string,"crop_width",len)) {
    return KR_COMPOSITOR_PATH_INFO_CROP_WIDTH;
  }
  if (!strncmp(string,"crop_height",len)) {
    return KR_COMPOSITOR_PATH_INFO_CROP_HEIGHT;
  }
  if (!strncmp(string,"view",len)) {
    return KR_COMPOSITOR_PATH_INFO_VIEW;
  }
  if (!strncmp(string,"controls",len)) {
    return KR_COMPOSITOR_PATH_INFO_CONTROLS;
  }
  return -1;
}

int kr_compositor_path_type_to_index(int val) {
  switch (val) {
    case KR_CMP_OUTPUT:
      return 0;
    case KR_CMP_INPUT:
      return 1;
    case KR_CMP_BUS:
      return 2;
  }
  return -1;
}

char *kr_strfr_kr_compositor_path_type(int val) {
  switch (val) {
    case KR_CMP_OUTPUT:
      return "kr_cmp_output";
    case KR_CMP_INPUT:
      return "kr_cmp_input";
    case KR_CMP_BUS:
      return "kr_cmp_bus";
  }
  return NULL;
}

int kr_strto_kr_compositor_path_type(char *string) {
  if (!strcmp(string,"kr_cmp_output")) {
    return KR_CMP_OUTPUT;
  }
  if (!strcmp(string,"kr_cmp_input")) {
    return KR_CMP_INPUT;
  }
  if (!strcmp(string,"kr_cmp_bus")) {
    return KR_CMP_BUS;
  }
  return -1;
}

int kr_compositor_subunit_type_to_index(int val) {
  switch (val) {
    case KR_VIDEOPORT:
      return 0;
    case KR_SPRITE:
      return 1;
    case KR_TEXT:
      return 2;
    case KR_VECTOR:
      return 3;
  }
  return -1;
}

char *kr_strfr_kr_compositor_subunit_type(int val) {
  switch (val) {
    case KR_VIDEOPORT:
      return "kr_videoport";
    case KR_SPRITE:
      return "kr_sprite";
    case KR_TEXT:
      return "kr_text";
    case KR_VECTOR:
      return "kr_vector";
  }
  return NULL;
}

int kr_strto_kr_compositor_subunit_type(char *string) {
  if (!strcmp(string,"kr_videoport")) {
    return KR_VIDEOPORT;
  }
  if (!strcmp(string,"kr_sprite")) {
    return KR_SPRITE;
  }
  if (!strcmp(string,"kr_text")) {
    return KR_TEXT;
  }
  if (!strcmp(string,"kr_vector")) {
    return KR_VECTOR;
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

int kr_compositor_control_to_index(int val) {
  switch (val) {
    case KR_NO:
      return 0;
    case KR_X:
      return 1;
    case KR_Y:
      return 2;
    case KR_Z:
      return 3;
    case KR_WIDTH:
      return 4;
    case KR_HEIGHT:
      return 5;
    case KR_ROTATION:
      return 6;
    case KR_OPACITY:
      return 7;
    case KR_RED:
      return 8;
    case KR_GREEN:
      return 9;
    case KR_BLUE:
      return 10;
    case KR_ALPHA:
      return 11;
    case KR_TICKRATE:
      return 12;
    case KR_CROP_X:
      return 13;
    case KR_CROP_Y:
      return 14;
    case KR_CROP_W:
      return 15;
    case KR_CROP_H:
      return 16;
    case KR_VIEW_TL_X:
      return 17;
    case KR_VIEW_TL_Y:
      return 18;
    case KR_VIEW_TR_X:
      return 19;
    case KR_VIEW_TR_Y:
      return 20;
    case KR_VIEW_BL_X:
      return 21;
    case KR_VIEW_BL_Y:
      return 22;
    case KR_VIEW_BR_X:
      return 23;
    case KR_VIEW_BR_Y:
      return 24;
  }
  return -1;
}

char *kr_strfr_kr_compositor_control(int val) {
  switch (val) {
    case KR_NO:
      return "kr_no";
    case KR_X:
      return "kr_x";
    case KR_Y:
      return "kr_y";
    case KR_Z:
      return "kr_z";
    case KR_WIDTH:
      return "kr_width";
    case KR_HEIGHT:
      return "kr_height";
    case KR_ROTATION:
      return "kr_rotation";
    case KR_OPACITY:
      return "kr_opacity";
    case KR_RED:
      return "kr_red";
    case KR_GREEN:
      return "kr_green";
    case KR_BLUE:
      return "kr_blue";
    case KR_ALPHA:
      return "kr_alpha";
    case KR_TICKRATE:
      return "kr_tickrate";
    case KR_CROP_X:
      return "kr_crop_x";
    case KR_CROP_Y:
      return "kr_crop_y";
    case KR_CROP_W:
      return "kr_crop_w";
    case KR_CROP_H:
      return "kr_crop_h";
    case KR_VIEW_TL_X:
      return "kr_view_tl_x";
    case KR_VIEW_TL_Y:
      return "kr_view_tl_y";
    case KR_VIEW_TR_X:
      return "kr_view_tr_x";
    case KR_VIEW_TR_Y:
      return "kr_view_tr_y";
    case KR_VIEW_BL_X:
      return "kr_view_bl_x";
    case KR_VIEW_BL_Y:
      return "kr_view_bl_y";
    case KR_VIEW_BR_X:
      return "kr_view_br_x";
    case KR_VIEW_BR_Y:
      return "kr_view_br_y";
  }
  return NULL;
}

int kr_strto_kr_compositor_control(char *string) {
  if (!strcmp(string,"kr_no")) {
    return KR_NO;
  }
  if (!strcmp(string,"kr_x")) {
    return KR_X;
  }
  if (!strcmp(string,"kr_y")) {
    return KR_Y;
  }
  if (!strcmp(string,"kr_z")) {
    return KR_Z;
  }
  if (!strcmp(string,"kr_width")) {
    return KR_WIDTH;
  }
  if (!strcmp(string,"kr_height")) {
    return KR_HEIGHT;
  }
  if (!strcmp(string,"kr_rotation")) {
    return KR_ROTATION;
  }
  if (!strcmp(string,"kr_opacity")) {
    return KR_OPACITY;
  }
  if (!strcmp(string,"kr_red")) {
    return KR_RED;
  }
  if (!strcmp(string,"kr_green")) {
    return KR_GREEN;
  }
  if (!strcmp(string,"kr_blue")) {
    return KR_BLUE;
  }
  if (!strcmp(string,"kr_alpha")) {
    return KR_ALPHA;
  }
  if (!strcmp(string,"kr_tickrate")) {
    return KR_TICKRATE;
  }
  if (!strcmp(string,"kr_crop_x")) {
    return KR_CROP_X;
  }
  if (!strcmp(string,"kr_crop_y")) {
    return KR_CROP_Y;
  }
  if (!strcmp(string,"kr_crop_w")) {
    return KR_CROP_W;
  }
  if (!strcmp(string,"kr_crop_h")) {
    return KR_CROP_H;
  }
  if (!strcmp(string,"kr_view_tl_x")) {
    return KR_VIEW_TL_X;
  }
  if (!strcmp(string,"kr_view_tl_y")) {
    return KR_VIEW_TL_Y;
  }
  if (!strcmp(string,"kr_view_tr_x")) {
    return KR_VIEW_TR_X;
  }
  if (!strcmp(string,"kr_view_tr_y")) {
    return KR_VIEW_TR_Y;
  }
  if (!strcmp(string,"kr_view_bl_x")) {
    return KR_VIEW_BL_X;
  }
  if (!strcmp(string,"kr_view_bl_y")) {
    return KR_VIEW_BL_Y;
  }
  if (!strcmp(string,"kr_view_br_x")) {
    return KR_VIEW_BR_X;
  }
  if (!strcmp(string,"kr_view_br_y")) {
    return KR_VIEW_BR_Y;
  }
  return -1;
}

int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch) {
  const ptrdiff_t off[3] = { offsetof(struct kr_sprite_info, filename), 
    offsetof(struct kr_sprite_info, rate), offsetof(struct kr_sprite_info, controls)
  };
  const size_t sz[3] = { sizeof(info->filename), 
    sizeof(info->rate), sizeof(info->controls)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_sprite_info_patch_path(kr_sprite_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_sprite_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch) {
  const ptrdiff_t off[6] = { offsetof(struct kr_text_info, string), 
    offsetof(struct kr_text_info, font), offsetof(struct kr_text_info, red), 
    offsetof(struct kr_text_info, green), offsetof(struct kr_text_info, blue), 
    offsetof(struct kr_text_info, controls)
  };
  const size_t sz[6] = { sizeof(info->string), 
    sizeof(info->font), sizeof(info->red), 
    sizeof(info->green), sizeof(info->blue), 
    sizeof(info->controls)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_text_info_patch_path(kr_text_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_text_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_vector_info_patch_apply(struct kr_vector_info *info, kr_vector_info_patch *patch) {
  const ptrdiff_t off[5] = { offsetof(struct kr_vector_info, type), 
    offsetof(struct kr_vector_info, red), offsetof(struct kr_vector_info, green), 
    offsetof(struct kr_vector_info, blue), offsetof(struct kr_vector_info, controls)
  };
  const size_t sz[5] = { sizeof(info->type), 
    sizeof(info->red), sizeof(info->green), 
    sizeof(info->blue), sizeof(info->controls)  };

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
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_path_info_patch_apply(struct kr_compositor_path_info *info, kr_compositor_path_info_patch *patch) {
  const ptrdiff_t off[9] = { offsetof(struct kr_compositor_path_info, type), 
    offsetof(struct kr_compositor_path_info, width), offsetof(struct kr_compositor_path_info, height), 
    offsetof(struct kr_compositor_path_info, crop_x), offsetof(struct kr_compositor_path_info, crop_y), 
    offsetof(struct kr_compositor_path_info, crop_width), offsetof(struct kr_compositor_path_info, crop_height), 
    offsetof(struct kr_compositor_path_info, view), offsetof(struct kr_compositor_path_info, controls)
  };
  const size_t sz[9] = { sizeof(info->type), 
    sizeof(info->width), sizeof(info->height), 
    sizeof(info->crop_x), sizeof(info->crop_y), 
    sizeof(info->crop_width), sizeof(info->crop_height), 
    sizeof(info->view), sizeof(info->controls)  };

  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);
  return 0;
}

kr_var *kr_compositor_path_info_patch_path(kr_compositor_path_info_patch *patch, kr_path *path) {
  char *name;
  int len;
  if (patch == NULL) return NULL;
  if (path == NULL) return NULL;
  len = kr_path_cur_name(path, &name);
  patch->member = kr_compositor_path_info_strto_member(name, len);
  if (patch->member < 1) return NULL;
  switch(patch->member) {
    default:
      if (kr_path_steps_ahead(path) != 0) return NULL;
      break;
  }
  /*patch->value.var.type = NN; not sure about this uhm*/
  return &patch->value.var;
}

int kr_compositor_controls_init(struct kr_compositor_controls *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_controls_valid(struct kr_compositor_controls *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_controls_random(struct kr_compositor_controls *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_sprite_info_init(struct kr_sprite_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_sprite_info));
  kr_compositor_controls_init(&st->controls);

  return 0;
}

int kr_sprite_info_valid(struct kr_sprite_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 256; i++) {
    if (!st->filename[i]) {
      break;
    }
    if (i == 255 && st->filename[i]) {
      return -2;
    }
  }
  kr_compositor_controls_valid(&st->controls);

  return 0;
}

int kr_sprite_info_random(struct kr_sprite_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_sprite_info));
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    st->filename[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      st->filename[255] = '\0';
    }
  }
  kr_compositor_controls_random(&st->controls);

  return 0;
}

int kr_text_info_init(struct kr_text_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_text_info));
  kr_compositor_controls_init(&st->controls);

  return 0;
}

int kr_text_info_valid(struct kr_text_info *st) {
  int i;

  if (st == NULL) {
    return -1;
  }

  for (i = 0; i < 512; i++) {
    if (!st->string[i]) {
      break;
    }
    if (i == 511 && st->string[i]) {
      return -2;
    }
  }
  for (i = 0; i < 256; i++) {
    if (!st->font[i]) {
      break;
    }
    if (i == 255 && st->font[i]) {
      return -3;
    }
  }
  kr_compositor_controls_valid(&st->controls);

  return 0;
}

int kr_text_info_random(struct kr_text_info *st) {
  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_text_info));
  for (i = 0; i < 512; i++) {
    scale = (double)25 / RAND_MAX;
    st->string[i] = 97 + floor(rand() * scale);
    if (i == 511) {
      st->string[511] = '\0';
    }
  }
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    st->font[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      st->font[255] = '\0';
    }
  }
  kr_compositor_controls_random(&st->controls);

  return 0;
}

int kr_vector_info_init(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_vector_info));
  kr_compositor_controls_init(&st->controls);

  return 0;
}

int kr_vector_info_valid(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_compositor_controls_valid(&st->controls);

  return 0;
}

int kr_vector_info_random(struct kr_vector_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_vector_info));
  kr_compositor_controls_random(&st->controls);

  return 0;
}

int kr_compositor_path_info_init(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_path_info));
  kr_perspective_view_init(&st->view);
  kr_compositor_controls_init(&st->controls);

  return 0;
}

int kr_compositor_path_info_valid(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  kr_perspective_view_valid(&st->view);
  kr_compositor_controls_valid(&st->controls);

  return 0;
}

int kr_compositor_path_info_random(struct kr_compositor_path_info *st) {
  if (st == NULL) {
    return -1;
  }

  memset(st, 0, sizeof(struct kr_compositor_path_info));
  kr_perspective_view_random(&st->view);
  kr_compositor_controls_random(&st->controls);

  return 0;
}

