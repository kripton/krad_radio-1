#ifndef KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#define KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_wayland_common.h"
typedef enum {
  KR_WAYLAND_INFO_DISPLAY_NAME = 1,
  KR_WAYLAND_INFO_STATE = 2
} kr_wayland_info_member;

typedef struct {
  kr_var var;
} kr_wayland_info_patch_value;

typedef struct {
  kr_wayland_info_member member;
  kr_wayland_info_patch_value value;
} kr_wayland_info_patch;

typedef enum {
  KR_WAYLAND_PATH_INFO_WIDTH = 1,
  KR_WAYLAND_PATH_INFO_HEIGHT = 2,
  KR_WAYLAND_PATH_INFO_FULLSCREEN = 3
} kr_wayland_path_info_member;

typedef struct {
  kr_var var;
} kr_wayland_path_info_patch_value;

typedef struct {
  kr_wayland_path_info_member member;
  kr_wayland_path_info_patch_value value;
} kr_wayland_path_info_patch;

int kr_wayland_info_patch_apply(kr_wayland_info *info, kr_wayland_info_patch *patch);
kr_var *kr_wayland_info_patch_path(kr_wayland_info_patch *patch, kr_path *path);
int kr_wayland_path_info_patch_apply(kr_wayland_path_info *info, kr_wayland_path_info_patch *patch);
kr_var *kr_wayland_path_info_patch_path(kr_wayland_path_info_patch *patch, kr_path *path);
int kr_wayland_info_init(kr_wayland_info *st);
int kr_wayland_info_valid(kr_wayland_info *st);
int kr_wayland_info_random(kr_wayland_info *st);
int kr_wayland_path_info_init(kr_wayland_path_info *st);
int kr_wayland_path_info_valid(kr_wayland_path_info *st);
int kr_wayland_path_info_random(kr_wayland_path_info *st);
kr_wayland_info_member kr_wayland_info_strto_member(char *string, int len);
kr_wayland_path_info_member kr_wayland_path_info_strto_member(char *string, int len);
int kr_wayland_state_to_index(int val);
int kr_strto_kr_wayland_state(char *string);
char *kr_strfr_kr_wayland_state(int val);
#endif
