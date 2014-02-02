#ifndef KRAD_ALSA_COMMON_HELPERS_GEN_H
#define KRAD_ALSA_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_alsa_common.h"
typedef enum {
  KR_ALSA_INFO_CARD = 1,
  KR_ALSA_INFO_NAME = 2
} kr_alsa_info_member;

typedef struct {
  int integer;
  float real;
} kr_alsa_info_patch_value;

typedef struct {
  kr_alsa_info_member member;
  kr_alsa_info_patch_value value;
} kr_alsa_info_patch;

typedef enum {
  KR_ALSA_PATH_INFO_CARD_NUM = 1
} kr_alsa_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_alsa_path_info_patch_value;

typedef struct {
  kr_alsa_path_info_member member;
  kr_alsa_path_info_patch_value value;
} kr_alsa_path_info_patch;

int kr_alsa_info_patch_apply(struct kr_alsa_info *info, kr_alsa_info_patch *patch);
kr_var *kr_alsa_info_address_to_patch(kr_alsa_info_patch *patch, kr_address2 *addr);
int kr_alsa_path_info_patch_apply(struct kr_alsa_path_info *info, kr_alsa_path_info_patch *patch);
kr_var *kr_alsa_path_info_address_to_patch(kr_alsa_path_info_patch *patch, kr_address2 *addr);
int kr_alsa_info_init(struct kr_alsa_info *st);
int kr_alsa_info_valid(struct kr_alsa_info *st);
int kr_alsa_info_random(struct kr_alsa_info *st);
int kr_alsa_path_info_init(struct kr_alsa_path_info *st);
int kr_alsa_path_info_valid(struct kr_alsa_path_info *st);
int kr_alsa_path_info_random(struct kr_alsa_path_info *st);
kr_alsa_info_member kr_alsa_info_strto_member(char *string, int len);
kr_alsa_path_info_member kr_alsa_path_info_strto_member(char *string, int len);
#endif
