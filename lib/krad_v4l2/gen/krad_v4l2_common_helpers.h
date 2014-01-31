#ifndef KRAD_V4L2_COMMON_HELPERS_GEN_H
#define KRAD_V4L2_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_v4l2_common.h"
typedef enum {
  KR_V4L2_INFO_DEV,
  KR_V4L2_INFO_PRIORITY,
  KR_V4L2_INFO_STATE,
  KR_V4L2_INFO_MODE
} kr_v4l2_info_member;

typedef struct {
  int integer;
  float real;
} kr_v4l2_info_patch_value;

typedef struct {
  kr_v4l2_info_member member;
  kr_v4l2_info_patch_value value;
} kr_v4l2_info_patch;

typedef enum {
  KR_V4L2_OPEN_INFO_DEV,
  KR_V4L2_OPEN_INFO_PRIORITY,
  KR_V4L2_OPEN_INFO_MODE
} kr_v4l2_open_info_member;

typedef struct {
  int integer;
  float real;
} kr_v4l2_open_info_patch_value;

typedef struct {
  kr_v4l2_open_info_member member;
  kr_v4l2_open_info_patch_value value;
} kr_v4l2_open_info_patch;

int kr_v4l2_info_patch_apply(struct kr_v4l2_info *info, kr_v4l2_info_patch *patch);
int kr_v4l2_open_info_patch_apply(struct kr_v4l2_open_info *info, kr_v4l2_open_info_patch *patch);
int kr_v4l2_mode_init(struct kr_v4l2_mode *st);
int kr_v4l2_mode_valid(struct kr_v4l2_mode *st);
int kr_v4l2_mode_random(struct kr_v4l2_mode *st);
int kr_v4l2_info_init(struct kr_v4l2_info *st);
int kr_v4l2_info_valid(struct kr_v4l2_info *st);
int kr_v4l2_info_random(struct kr_v4l2_info *st);
int kr_v4l2_open_info_init(struct kr_v4l2_open_info *st);
int kr_v4l2_open_info_valid(struct kr_v4l2_open_info *st);
int kr_v4l2_open_info_random(struct kr_v4l2_open_info *st);
int kr_v4l2_state_to_index(int val);
int kr_strto_kr_v4l2_state(char *string);
char *kr_strfr_kr_v4l2_state(int val);
#endif
