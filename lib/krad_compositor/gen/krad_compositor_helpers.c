#include "krad_compositor_helpers.h"

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

int kr_compositor_controls_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_controls_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_compositor_controls_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_sprite_info_init(void *st) {
  struct kr_sprite_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  memset(actual, 0, sizeof(struct kr_sprite_info));
  kr_compositor_controls_init(&actual->controls);

  return 0;
}

int kr_sprite_info_valid(void *st) {
  struct kr_sprite_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  for (i = 0; i < 256; i++) {
    if (!actual->filename[i]) {
      break;
    }
    if (i == 255 && actual->filename[i]) {
      return -2;
    }
  }
  kr_compositor_controls_valid(&actual->controls);

  return 0;
}

int kr_sprite_info_random(void *st) {
  struct kr_sprite_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_sprite_info *)st;
  memset(actual, 0, sizeof(struct kr_sprite_info));
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    actual->filename[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      actual->filename[255] = '\0';
    }
  }
  kr_compositor_controls_random(&actual->controls);

  return 0;
}

int kr_text_info_init(void *st) {
  struct kr_text_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_text_info *)st;
  memset(actual, 0, sizeof(struct kr_text_info));
  kr_compositor_controls_init(&actual->controls);

  return 0;
}

int kr_text_info_valid(void *st) {
  struct kr_text_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_text_info *)st;
  for (i = 0; i < 512; i++) {
    if (!actual->string[i]) {
      break;
    }
    if (i == 511 && actual->string[i]) {
      return -2;
    }
  }
  for (i = 0; i < 256; i++) {
    if (!actual->font[i]) {
      break;
    }
    if (i == 255 && actual->font[i]) {
      return -3;
    }
  }
  kr_compositor_controls_valid(&actual->controls);

  return 0;
}

int kr_text_info_random(void *st) {
  struct kr_text_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_text_info *)st;
  memset(actual, 0, sizeof(struct kr_text_info));
  for (i = 0; i < 512; i++) {
    scale = (double)25 / RAND_MAX;
    actual->string[i] = 97 + floor(rand() * scale);
    if (i == 511) {
      actual->string[511] = '\0';
    }
  }
  for (i = 0; i < 256; i++) {
    scale = (double)25 / RAND_MAX;
    actual->font[i] = 97 + floor(rand() * scale);
    if (i == 255) {
      actual->font[255] = '\0';
    }
  }
  kr_compositor_controls_random(&actual->controls);

  return 0;
}

int kr_vector_info_init(void *st) {
  struct kr_vector_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  memset(actual, 0, sizeof(struct kr_vector_info));
  kr_compositor_controls_init(&actual->controls);

  return 0;
}

int kr_vector_info_valid(void *st) {
  struct kr_vector_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  kr_compositor_controls_valid(&actual->controls);

  return 0;
}

int kr_vector_info_random(void *st) {
  struct kr_vector_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_vector_info *)st;
  memset(actual, 0, sizeof(struct kr_vector_info));
  kr_compositor_controls_random(&actual->controls);

  return 0;
}

int kr_compositor_path_info_init(void *st) {
  struct kr_compositor_path_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;
  memset(actual, 0, sizeof(struct kr_compositor_path_info));
  kr_perspective_view_init(&actual->view);
  kr_compositor_controls_init(&actual->controls);

  return 0;
}

int kr_compositor_path_info_valid(void *st) {
  struct kr_compositor_path_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;
  for (i = 0; i < 128; i++) {
    if (!actual->name[i]) {
      break;
    }
    if (i == 127 && actual->name[i]) {
      return -2;
    }
  }
  kr_perspective_view_valid(&actual->view);
  kr_compositor_controls_valid(&actual->controls);

  return 0;
}

int kr_compositor_path_info_random(void *st) {
  struct kr_compositor_path_info *actual;

  int i;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_path_info *)st;
  memset(actual, 0, sizeof(struct kr_compositor_path_info));
  for (i = 0; i < 128; i++) {
    scale = (double)25 / RAND_MAX;
    actual->name[i] = 97 + floor(rand() * scale);
    if (i == 127) {
      actual->name[127] = '\0';
    }
  }
  kr_perspective_view_random(&actual->view);
  kr_compositor_controls_random(&actual->controls);

  return 0;
}

int kr_compositor_info_init(void *st) {
  struct kr_compositor_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_info *)st;
  memset(actual, 0, sizeof(struct kr_compositor_info));
  actual->width = 640;
  actual->height = 360;
  actual->fps_numerator = 30;
  actual->fps_denominator = 1;
  actual->sprites = 0;
  actual->vectors = 0;
  actual->texts = 0;
  actual->inputs = 0;
  actual->outputs = 0;

  return 0;
}

int kr_compositor_info_valid(void *st) {
  struct kr_compositor_info *actual;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_info *)st;
  if ( (actual->width < 320) || (actual->width > 1920) ) {
    return -2;
  }

  if ( (actual->height < 240) || (actual->height > 1080) ) {
    return -3;
  }

  if ( (actual->fps_numerator < 20) || (actual->fps_numerator > 120) ) {
    return -4;
  }

  if ( (actual->fps_denominator < 1) || (actual->fps_denominator > 1) ) {
    return -5;
  }

  if ( (actual->sprites < 0) || (actual->sprites > 128) ) {
    return -6;
  }

  if ( (actual->vectors < 0) || (actual->vectors > 128) ) {
    return -7;
  }

  if ( (actual->texts < 0) || (actual->texts > 128) ) {
    return -8;
  }

  if ( (actual->inputs < 0) || (actual->inputs > 32) ) {
    return -9;
  }

  if ( (actual->outputs < 0) || (actual->outputs > 32) ) {
    return -10;
  }


  return 0;
}

int kr_compositor_info_random(void *st) {
  struct kr_compositor_info *actual;

  struct timeval tv;
  double scale;

  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_compositor_info *)st;
  memset(actual, 0, sizeof(struct kr_compositor_info));
  scale = (double)1600 / RAND_MAX;
  actual->width = 320 + floor(rand() * scale);
  scale = (double)840 / RAND_MAX;
  actual->height = 240 + floor(rand() * scale);
  scale = (double)100 / RAND_MAX;
  actual->fps_numerator = 20 + floor(rand() * scale);
  scale = (double)0 / RAND_MAX;
  actual->fps_denominator = 1 + floor(rand() * scale);
  scale = (double)128 / RAND_MAX;
  actual->sprites = 0 + floor(rand() * scale);
  scale = (double)128 / RAND_MAX;
  actual->vectors = 0 + floor(rand() * scale);
  scale = (double)128 / RAND_MAX;
  actual->texts = 0 + floor(rand() * scale);
  scale = (double)32 / RAND_MAX;
  actual->inputs = 0 + floor(rand() * scale);
  scale = (double)32 / RAND_MAX;
  actual->outputs = 0 + floor(rand() * scale);

  return 0;
}

