#ifndef KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#define KRAD_WAYLAND_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_wayland_common.h"
typedef enum {
  KR_WAYLAND_INFO_STATE,
  KR_WAYLAND_INFO_DISPLAY_NAME
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
  KR_WAYLAND_PATH_INFO_DISPLAY_NAME,
  KR_WAYLAND_PATH_INFO_STATE,
  KR_WAYLAND_PATH_INFO_WIDTH,
  KR_WAYLAND_PATH_INFO_HEIGHT,
  KR_WAYLAND_PATH_INFO_FULLSCREEN
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
int kr_wayland_path_info_patch_apply(kr_wayland_path_info *info, kr_wayland_path_info_patch *patch);
int kr_wayland_info_init(void *st);
int kr_wayland_info_valid(void *st);
int kr_wayland_info_random(void *st);
int kr_wayland_path_info_init(void *st);
int kr_wayland_path_info_valid(void *st);
int kr_wayland_path_info_random(void *st);
#endif
