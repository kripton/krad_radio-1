#ifndef KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#define KRAD_COMPOSITOR_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_compositor_common.h"
typedef enum {
  KR_COMPOSITOR_OUTPUT_INFO_W = 1,
  KR_COMPOSITOR_OUTPUT_INFO_H = 2,
  KR_COMPOSITOR_OUTPUT_INFO_OPACITY = 3
} kr_compositor_output_info_member;

typedef struct {
  kr_var var;
} kr_compositor_output_info_patch_value;

typedef struct {
  kr_compositor_output_info_member member;
  kr_compositor_output_info_patch_value value;
} kr_compositor_output_info_patch;

typedef enum {
  KR_COMPOSITOR_BUS_INFO_OPACITY = 1
} kr_compositor_bus_info_member;

typedef struct {
  kr_var var;
} kr_compositor_bus_info_patch_value;

typedef struct {
  kr_compositor_bus_info_member member;
  kr_compositor_bus_info_patch_value value;
} kr_compositor_bus_info_patch;

typedef enum {
  KR_COMPOSITOR_INPUT_INFO_CROP = 1,
  KR_COMPOSITOR_INPUT_INFO_POS = 2,
  KR_COMPOSITOR_INPUT_INFO_VIEW = 3,
  KR_COMPOSITOR_INPUT_INFO_Z = 4,
  KR_COMPOSITOR_INPUT_INFO_ROTATION = 5,
  KR_COMPOSITOR_INPUT_INFO_OPACITY = 6
} kr_compositor_input_info_member;

typedef struct {
  kr_var var;
} kr_compositor_input_info_patch_value;

typedef struct {
  kr_compositor_input_info_member member;
  kr_compositor_input_info_patch_value value;
} kr_compositor_input_info_patch;

typedef enum {
  KR_COMPOSITOR_SOURCE_INFO_W = 1,
  KR_COMPOSITOR_SOURCE_INFO_H = 2
} kr_compositor_source_info_member;

typedef struct {
  kr_var var;
} kr_compositor_source_info_patch_value;

typedef struct {
  kr_compositor_source_info_member member;
  kr_compositor_source_info_patch_value value;
} kr_compositor_source_info_patch;

typedef enum {
  KR_COMPOSITOR_PATH_INFO_TYPE = 1,
  KR_COMPOSITOR_PATH_INFO_INFO = 2
} kr_compositor_path_info_member;

typedef struct {
  kr_var var;
} kr_compositor_path_info_patch_value;

typedef struct {
  kr_compositor_path_info_member member;
  kr_compositor_path_info_patch_value value;
} kr_compositor_path_info_patch;

typedef enum {
  KR_SPRITE_INFO_FILENAME = 1,
  KR_SPRITE_INFO_RATE = 2,
  KR_SPRITE_INFO_INPUT_INFO = 3
} kr_sprite_info_member;

typedef struct {
  kr_var var;
  kr_compositor_input_info_patch input_info_patch;
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
  KR_TEXT_INFO_INPUT_INFO = 6
} kr_text_info_member;

typedef struct {
  kr_var var;
  kr_compositor_input_info_patch input_info_patch;
} kr_text_info_patch_value;

typedef struct {
  kr_text_info_member member;
  kr_text_info_patch_value value;
} kr_text_info_patch;

int kr_compositor_output_info_patch_apply(struct kr_compositor_output_info *info, kr_compositor_output_info_patch *patch);
kr_var *kr_compositor_output_info_patch_path(kr_compositor_output_info_patch *patch, kr_path *path);
int kr_compositor_bus_info_patch_apply(struct kr_compositor_bus_info *info, kr_compositor_bus_info_patch *patch);
kr_var *kr_compositor_bus_info_patch_path(kr_compositor_bus_info_patch *patch, kr_path *path);
int kr_compositor_input_info_patch_apply(struct kr_compositor_input_info *info, kr_compositor_input_info_patch *patch);
kr_var *kr_compositor_input_info_patch_path(kr_compositor_input_info_patch *patch, kr_path *path);
int kr_compositor_source_info_patch_apply(struct kr_compositor_source_info *info, kr_compositor_source_info_patch *patch);
kr_var *kr_compositor_source_info_patch_path(kr_compositor_source_info_patch *patch, kr_path *path);
int kr_compositor_path_info_patch_apply(struct kr_compositor_path_info *info, kr_compositor_path_info_patch *patch);
kr_var *kr_compositor_path_info_patch_path(kr_compositor_path_info_patch *patch, kr_path *path);
int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch);
kr_var *kr_sprite_info_patch_path(kr_sprite_info_patch *patch, kr_path *path);
int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch);
kr_var *kr_text_info_patch_path(kr_text_info_patch *patch, kr_path *path);
int kr_rect_init(struct kr_rect *st);
int kr_rect_valid(struct kr_rect *st);
int kr_rect_random(struct kr_rect *st);
int kr_compositor_output_info_init(struct kr_compositor_output_info *st);
int kr_compositor_output_info_valid(struct kr_compositor_output_info *st);
int kr_compositor_output_info_random(struct kr_compositor_output_info *st);
int kr_compositor_bus_info_init(struct kr_compositor_bus_info *st);
int kr_compositor_bus_info_valid(struct kr_compositor_bus_info *st);
int kr_compositor_bus_info_random(struct kr_compositor_bus_info *st);
int kr_compositor_input_info_init(struct kr_compositor_input_info *st);
int kr_compositor_input_info_valid(struct kr_compositor_input_info *st);
int kr_compositor_input_info_random(struct kr_compositor_input_info *st);
int kr_compositor_source_info_init(struct kr_compositor_source_info *st);
int kr_compositor_source_info_valid(struct kr_compositor_source_info *st);
int kr_compositor_source_info_random(struct kr_compositor_source_info *st);
int kr_compositor_path_type_info_init(kr_compositor_path_type_info *st, int idx);
int kr_compositor_path_type_info_valid(kr_compositor_path_type_info *st, int idx);
int kr_compositor_path_type_info_random(kr_compositor_path_type_info *st, int idx);
int kr_compositor_path_info_init(struct kr_compositor_path_info *st);
int kr_compositor_path_info_valid(struct kr_compositor_path_info *st);
int kr_compositor_path_info_random(struct kr_compositor_path_info *st);
int kr_sprite_info_init(struct kr_sprite_info *st);
int kr_sprite_info_valid(struct kr_sprite_info *st);
int kr_sprite_info_random(struct kr_sprite_info *st);
int kr_text_info_init(struct kr_text_info *st);
int kr_text_info_valid(struct kr_text_info *st);
int kr_text_info_random(struct kr_text_info *st);
kr_compositor_output_info_member kr_compositor_output_info_strto_member(char *string, int len);
kr_compositor_bus_info_member kr_compositor_bus_info_strto_member(char *string, int len);
kr_compositor_input_info_member kr_compositor_input_info_strto_member(char *string, int len);
kr_compositor_source_info_member kr_compositor_source_info_strto_member(char *string, int len);
kr_compositor_path_info_member kr_compositor_path_info_strto_member(char *string, int len);
kr_sprite_info_member kr_sprite_info_strto_member(char *string, int len);
kr_text_info_member kr_text_info_strto_member(char *string, int len);
int kr_compositor_path_type_to_index(int val);
int kr_strto_kr_compositor_path_type(char *string);
char *kr_strfr_kr_compositor_path_type(int val);
int kr_compositor_overlay_type_to_index(int val);
int kr_strto_kr_compositor_overlay_type(char *string);
char *kr_strfr_kr_compositor_overlay_type(int val);
#endif
