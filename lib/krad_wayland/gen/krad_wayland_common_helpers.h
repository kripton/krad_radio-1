#ifndef KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#define KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_wayland_common.h"
typedef enum {
  KR_WAYLAND_INFO_STATE = 1,
  KR_WAYLAND_INFO_DISPLAY_NAME = 2
} kr_wayland_info_member;

typedef struct {
  int integer;
  float real;
} kr_wayland_info_patch_value;

typedef struct {
  kr_wayland_info_member member;
  kr_wayland_info_patch_value value;
} kr_wayland_info_patch;

typedef enum {
  KR_WAYLAND_PATH_INFO_DISPLAY_NAME = 1,
  KR_WAYLAND_PATH_INFO_STATE = 2,
  KR_WAYLAND_PATH_INFO_WIDTH = 3,
  KR_WAYLAND_PATH_INFO_HEIGHT = 4,
  KR_WAYLAND_PATH_INFO_FULLSCREEN = 5
} kr_wayland_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_wayland_path_info_patch_value;

typedef struct {
  kr_wayland_path_info_member member;
  kr_wayland_path_info_patch_value value;
} kr_wayland_path_info_patch;

int kr_wayland_info_patch_apply(kr_wayland_info *info, kr_wayland_info_patch *patch);
kr_value *kr_wayland_info_address_to_patch(kr_wayland_info_patch *patch, kr_address2 *addr);
int kr_wayland_path_info_patch_apply(kr_wayland_path_info *info, kr_wayland_path_info_patch *patch);
kr_value *kr_wayland_path_info_address_to_patch(kr_wayland_path_info_patch *patch, kr_address2 *addr);
int kr_wayland_info_init(kr_wayland_info *st);
int kr_wayland_info_valid(kr_wayland_info *st);
int kr_wayland_info_random(kr_wayland_info *st);
int kr_wayland_path_info_init(kr_wayland_path_info *st);
int kr_wayland_path_info_valid(kr_wayland_path_info *st);
int kr_wayland_path_info_random(kr_wayland_path_info *st);
kr_wayland_info_member kr_wayland_info_strto_member(char *string, int len);
kr_wayland_path_info_member kr_wayland_path_info_strto_member(char *string, int len);
#endif
