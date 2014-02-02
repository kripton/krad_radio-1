#ifndef KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#define KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_compositor_common.h"
typedef enum {
  KR_SPRITE_INFO_FILENAME = 1,
  KR_SPRITE_INFO_RATE = 2,
  KR_SPRITE_INFO_CONTROLS = 3
} kr_sprite_info_member;

typedef struct {
  kr_var var;
} kr_sprite_info_patch_value;

typedef struct {
  kr_sprite_info_member member;
  kr_sprite_info_patch_value value;
} kr_sprite_info_patch;

typedef enum {
  KR_TEXT_INFO_STRING = 1,
  KR_TEXT_INFO_FONT = 2,
  KR_TEXT_INFO_RED = 3,
  KR_TEXT_INFO_GREEN = 4,
  KR_TEXT_INFO_BLUE = 5,
  KR_TEXT_INFO_CONTROLS = 6
} kr_text_info_member;

typedef struct {
  kr_var var;
} kr_text_info_patch_value;

typedef struct {
  kr_text_info_member member;
  kr_text_info_patch_value value;
} kr_text_info_patch;

typedef enum {
  KR_VECTOR_INFO_TYPE = 1,
  KR_VECTOR_INFO_RED = 2,
  KR_VECTOR_INFO_GREEN = 3,
  KR_VECTOR_INFO_BLUE = 4,
  KR_VECTOR_INFO_CONTROLS = 5
} kr_vector_info_member;

typedef struct {
  kr_var var;
} kr_vector_info_patch_value;

typedef struct {
  kr_vector_info_member member;
  kr_vector_info_patch_value value;
} kr_vector_info_patch;

typedef enum {
  KR_COMPOSITOR_PATH_INFO_TYPE = 1,
  KR_COMPOSITOR_PATH_INFO_WIDTH = 2,
  KR_COMPOSITOR_PATH_INFO_HEIGHT = 3,
  KR_COMPOSITOR_PATH_INFO_CROP_X = 4,
  KR_COMPOSITOR_PATH_INFO_CROP_Y = 5,
  KR_COMPOSITOR_PATH_INFO_CROP_WIDTH = 6,
  KR_COMPOSITOR_PATH_INFO_CROP_HEIGHT = 7,
  KR_COMPOSITOR_PATH_INFO_VIEW = 8,
  KR_COMPOSITOR_PATH_INFO_CONTROLS = 9
} kr_compositor_path_info_member;

typedef struct {
  kr_var var;
} kr_compositor_path_info_patch_value;

typedef struct {
  kr_compositor_path_info_member member;
  kr_compositor_path_info_patch_value value;
} kr_compositor_path_info_patch;

int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch);
kr_var *kr_sprite_info_patch_path(kr_sprite_info_patch *patch, kr_path *path);
int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch);
kr_var *kr_text_info_patch_path(kr_text_info_patch *patch, kr_path *path);
int kr_vector_info_patch_apply(struct kr_vector_info *info, kr_vector_info_patch *patch);
kr_var *kr_vector_info_patch_path(kr_vector_info_patch *patch, kr_path *path);
int kr_compositor_path_info_patch_apply(struct kr_compositor_path_info *info, kr_compositor_path_info_patch *patch);
kr_var *kr_compositor_path_info_patch_path(kr_compositor_path_info_patch *patch, kr_path *path);
int kr_compositor_controls_init(struct kr_compositor_controls *st);
int kr_compositor_controls_valid(struct kr_compositor_controls *st);
int kr_compositor_controls_random(struct kr_compositor_controls *st);
int kr_sprite_info_init(struct kr_sprite_info *st);
int kr_sprite_info_valid(struct kr_sprite_info *st);
int kr_sprite_info_random(struct kr_sprite_info *st);
int kr_text_info_init(struct kr_text_info *st);
int kr_text_info_valid(struct kr_text_info *st);
int kr_text_info_random(struct kr_text_info *st);
int kr_vector_info_init(struct kr_vector_info *st);
int kr_vector_info_valid(struct kr_vector_info *st);
int kr_vector_info_random(struct kr_vector_info *st);
int kr_compositor_path_info_init(struct kr_compositor_path_info *st);
int kr_compositor_path_info_valid(struct kr_compositor_path_info *st);
int kr_compositor_path_info_random(struct kr_compositor_path_info *st);
kr_sprite_info_member kr_sprite_info_strto_member(char *string, int len);
kr_text_info_member kr_text_info_strto_member(char *string, int len);
kr_vector_info_member kr_vector_info_strto_member(char *string, int len);
kr_compositor_path_info_member kr_compositor_path_info_strto_member(char *string, int len);
int kr_compositor_path_type_to_index(int val);
int kr_strto_kr_compositor_path_type(char *string);
char *kr_strfr_kr_compositor_path_type(int val);
int kr_compositor_subunit_type_to_index(int val);
int kr_strto_kr_compositor_subunit_type(char *string);
char *kr_strfr_kr_compositor_subunit_type(int val);
int kr_vector_type_to_index(int val);
int kr_strto_kr_vector_type(char *string);
char *kr_strfr_kr_vector_type(int val);
int kr_compositor_control_to_index(int val);
int kr_strto_kr_compositor_control(char *string);
char *kr_strfr_kr_compositor_control(int val);
#endif
