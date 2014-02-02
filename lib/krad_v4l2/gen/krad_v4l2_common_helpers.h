#ifndef KRAD_V4L2_COMMON_HELPERS_GEN_H
#define KRAD_V4L2_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_v4l2_common.h"
typedef enum {
  KR_V4L2_INFO_DEV = 1,
  KR_V4L2_INFO_PRIORITY = 2,
  KR_V4L2_INFO_STATE = 3,
  KR_V4L2_INFO_MODE = 4
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
  KR_V4L2_OPEN_INFO_DEV = 1,
  KR_V4L2_OPEN_INFO_PRIORITY = 2,
  KR_V4L2_OPEN_INFO_MODE = 3
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
kr_var *kr_v4l2_info_address_to_patch(kr_v4l2_info_patch *patch, kr_address2 *addr);
int kr_v4l2_open_info_patch_apply(struct kr_v4l2_open_info *info, kr_v4l2_open_info_patch *patch);
kr_var *kr_v4l2_open_info_address_to_patch(kr_v4l2_open_info_patch *patch, kr_address2 *addr);
int kr_v4l2_mode_init(struct kr_v4l2_mode *st);
int kr_v4l2_mode_valid(struct kr_v4l2_mode *st);
int kr_v4l2_mode_random(struct kr_v4l2_mode *st);
int kr_v4l2_info_init(struct kr_v4l2_info *st);
int kr_v4l2_info_valid(struct kr_v4l2_info *st);
int kr_v4l2_info_random(struct kr_v4l2_info *st);
int kr_v4l2_open_info_init(struct kr_v4l2_open_info *st);
int kr_v4l2_open_info_valid(struct kr_v4l2_open_info *st);
int kr_v4l2_open_info_random(struct kr_v4l2_open_info *st);
kr_v4l2_info_member kr_v4l2_info_strto_member(char *string, int len);
kr_v4l2_open_info_member kr_v4l2_open_info_strto_member(char *string, int len);
int kr_v4l2_state_to_index(int val);
int kr_strto_kr_v4l2_state(char *string);
char *kr_strfr_kr_v4l2_state(int val);
#endif
