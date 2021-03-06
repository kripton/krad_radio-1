#ifndef KRAD_V4L2_COMMON_HELPERS_GEN_H
#define KRAD_V4L2_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_v4l2_common.h"
typedef enum {
  KR_V4L2_PATH_INFO_WIDTH = 1,
  KR_V4L2_PATH_INFO_HEIGHT = 2,
  KR_V4L2_PATH_INFO_NUM = 3,
  KR_V4L2_PATH_INFO_DEN = 4,
  KR_V4L2_PATH_INFO_FORMAT = 5
} kr_v4l2_path_info_member;

typedef struct {
  kr_var var;
} kr_v4l2_path_info_patch_value;

typedef struct {
  kr_v4l2_path_info_member member;
  kr_v4l2_path_info_patch_value value;
} kr_v4l2_path_info_patch;

typedef enum {
  KR_V4L2_INFO_DEV = 1,
  KR_V4L2_INFO_PRIORITY = 2,
  KR_V4L2_INFO_STATE = 3
} kr_v4l2_info_member;

typedef struct {
  kr_var var;
} kr_v4l2_info_patch_value;

typedef struct {
  kr_v4l2_info_member member;
  kr_v4l2_info_patch_value value;
} kr_v4l2_info_patch;

int kr_v4l2_path_info_patch_apply(kr_v4l2_path_info *info, kr_v4l2_path_info_patch *patch);
kr_var *kr_v4l2_path_info_patch_path(kr_v4l2_path_info_patch *patch, kr_path *path);
int kr_v4l2_info_patch_apply(kr_v4l2_info *info, kr_v4l2_info_patch *patch);
kr_var *kr_v4l2_info_patch_path(kr_v4l2_info_patch *patch, kr_path *path);
int kr_v4l2_path_info_init(kr_v4l2_path_info *st);
int kr_v4l2_path_info_valid(kr_v4l2_path_info *st);
int kr_v4l2_path_info_random(kr_v4l2_path_info *st);
int kr_v4l2_info_init(kr_v4l2_info *st);
int kr_v4l2_info_valid(kr_v4l2_info *st);
int kr_v4l2_info_random(kr_v4l2_info *st);
kr_v4l2_path_info_member kr_v4l2_path_info_strto_member(char *string, int len);
kr_v4l2_info_member kr_v4l2_info_strto_member(char *string, int len);
int kr_v4l2_state_to_index(int val);
int kr_strto_kr_v4l2_state(char *string);
char *kr_strfr_kr_v4l2_state(int val);
#endif
