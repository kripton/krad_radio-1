#ifndef KRAD_ALSA_COMMON_HELPERS_GEN_H
#define KRAD_ALSA_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_alsa_common.h"
typedef enum {
  KR_ALSA_INFO_CARD,
  KR_ALSA_INFO_NAME
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
  KR_ALSA_PATH_INFO_CARD_NUM
} kr_alsa_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_alsa_path_info_patch_value;

typedef struct {
  kr_alsa_path_info_member member;
  kr_alsa_path_info_patch_value value;
} kr_alsa_path_info_patch;

int kr_alsa_info_init(void *st);
int kr_alsa_info_valid(void *st);
int kr_alsa_info_random(void *st);
int kr_alsa_path_info_init(void *st);
int kr_alsa_path_info_valid(void *st);
int kr_alsa_path_info_random(void *st);
#endif
