#ifndef KRAD_X11_COMMON_HELPERS_GEN_H
#define KRAD_X11_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_x11_common.h"
typedef enum {
  KR_X11_INFO_DISPLAY = 1
} kr_x11_info_member;

typedef struct {
  kr_var var;
} kr_x11_info_patch_value;

typedef struct {
  kr_x11_info_member member;
  kr_x11_info_patch_value value;
} kr_x11_info_patch;

typedef enum {
  KR_X11_PATH_INFO_DISPLAY = 1,
  KR_X11_PATH_INFO_WIDTH = 2,
  KR_X11_PATH_INFO_HEIGHT = 3,
  KR_X11_PATH_INFO_NUM = 4,
  KR_X11_PATH_INFO_DEN = 5,
  KR_X11_PATH_INFO_X = 6,
  KR_X11_PATH_INFO_Y = 7
} kr_x11_path_info_member;

typedef struct {
  kr_var var;
} kr_x11_path_info_patch_value;

typedef struct {
  kr_x11_path_info_member member;
  kr_x11_path_info_patch_value value;
} kr_x11_path_info_patch;

int kr_x11_info_patch_apply(struct kr_x11_info *info, kr_x11_info_patch *patch);
kr_var *kr_x11_info_patch_path(kr_x11_info_patch *patch, kr_path *path);
int kr_x11_path_info_patch_apply(struct kr_x11_path_info *info, kr_x11_path_info_patch *patch);
kr_var *kr_x11_path_info_patch_path(kr_x11_path_info_patch *patch, kr_path *path);
int kr_x11_info_init(struct kr_x11_info *st);
int kr_x11_info_valid(struct kr_x11_info *st);
int kr_x11_info_random(struct kr_x11_info *st);
int kr_x11_path_info_init(struct kr_x11_path_info *st);
int kr_x11_path_info_valid(struct kr_x11_path_info *st);
int kr_x11_path_info_random(struct kr_x11_path_info *st);
kr_x11_info_member kr_x11_info_strto_member(char *string, int len);
kr_x11_path_info_member kr_x11_path_info_strto_member(char *string, int len);
#endif
