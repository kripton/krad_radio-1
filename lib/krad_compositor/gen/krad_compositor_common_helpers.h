#ifndef KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#define KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_compositor_common.h"
typedef enum {
  KR_SPRITE_INFO_FILENAME,
  KR_SPRITE_INFO_RATE,
  KR_SPRITE_INFO_CONTROLS
} kr_sprite_info_member;

typedef struct {
  int integer;
  float real;
} kr_sprite_info_patch_value;

typedef struct {
  kr_sprite_info_member member;
  kr_sprite_info_patch_value value;
} kr_sprite_info_patch;

typedef enum {
  KR_TEXT_INFO_STRING,
  KR_TEXT_INFO_FONT,
  KR_TEXT_INFO_RED,
  KR_TEXT_INFO_GREEN,
  KR_TEXT_INFO_BLUE,
  KR_TEXT_INFO_CONTROLS
} kr_text_info_member;

typedef struct {
  int integer;
  float real;
} kr_text_info_patch_value;

typedef struct {
  kr_text_info_member member;
  kr_text_info_patch_value value;
} kr_text_info_patch;

typedef enum {
  KR_VECTOR_INFO_TYPE,
  KR_VECTOR_INFO_RED,
  KR_VECTOR_INFO_GREEN,
  KR_VECTOR_INFO_BLUE,
  KR_VECTOR_INFO_CONTROLS
} kr_vector_info_member;

typedef struct {
  int integer;
  float real;
} kr_vector_info_patch_value;

typedef struct {
  kr_vector_info_member member;
  kr_vector_info_patch_value value;
} kr_vector_info_patch;

typedef enum {
  KR_COMPOSITOR_PATH_INFO_TYPE,
  KR_COMPOSITOR_PATH_INFO_WIDTH,
  KR_COMPOSITOR_PATH_INFO_HEIGHT,
  KR_COMPOSITOR_PATH_INFO_CROP_X,
  KR_COMPOSITOR_PATH_INFO_CROP_Y,
  KR_COMPOSITOR_PATH_INFO_CROP_WIDTH,
  KR_COMPOSITOR_PATH_INFO_CROP_HEIGHT,
  KR_COMPOSITOR_PATH_INFO_VIEW,
  KR_COMPOSITOR_PATH_INFO_CONTROLS
} kr_compositor_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_compositor_path_info_patch_value;

typedef struct {
  kr_compositor_path_info_member member;
  kr_compositor_path_info_patch_value value;
} kr_compositor_path_info_patch;

int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch);
int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch);
int kr_vector_info_patch_apply(struct kr_vector_info *info, kr_vector_info_patch *patch);
int kr_compositor_path_info_patch_apply(struct kr_compositor_path_info *info, kr_compositor_path_info_patch *patch);
int kr_compositor_controls_init(void *st);
int kr_compositor_controls_valid(void *st);
int kr_compositor_controls_random(void *st);
int kr_sprite_info_init(void *st);
int kr_sprite_info_valid(void *st);
int kr_sprite_info_random(void *st);
int kr_text_info_init(void *st);
int kr_text_info_valid(void *st);
int kr_text_info_random(void *st);
int kr_vector_info_init(void *st);
int kr_vector_info_valid(void *st);
int kr_vector_info_random(void *st);
int kr_compositor_path_info_init(void *st);
int kr_compositor_path_info_valid(void *st);
int kr_compositor_path_info_random(void *st);
int kr_compositor_path_patch_init(void *st);
int kr_compositor_path_patch_valid(void *st);
int kr_compositor_path_patch_random(void *st);
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
