#ifndef KRAD_DECKLINK_COMMON_HELPERS_GEN_H
#define KRAD_DECKLINK_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_decklink_common.h"
typedef enum {
  KR_DECKLINK_INFO_DEVICE
} kr_decklink_info_member;

typedef struct {
  int integer;
  float real;
} kr_decklink_info_patch_value;

typedef struct {
  kr_decklink_info_member member;
  kr_decklink_info_patch_value value;
} kr_decklink_info_patch;

typedef enum {
  KR_DECKLINK_PATH_INFO_DEVICE,
  KR_DECKLINK_PATH_INFO_WIDTH,
  KR_DECKLINK_PATH_INFO_HEIGHT,
  KR_DECKLINK_PATH_INFO_NUM,
  KR_DECKLINK_PATH_INFO_DEN,
  KR_DECKLINK_PATH_INFO_VIDEO_CONNECTOR,
  KR_DECKLINK_PATH_INFO_AUDIO_CONNECTOR
} kr_decklink_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_decklink_path_info_patch_value;

typedef struct {
  kr_decklink_path_info_member member;
  kr_decklink_path_info_patch_value value;
} kr_decklink_path_info_patch;

int kr_decklink_info_patch_apply(struct kr_decklink_info *info, kr_decklink_info_patch *patch);
int kr_decklink_path_info_patch_apply(struct kr_decklink_path_info *info, kr_decklink_path_info_patch *patch);
int kr_decklink_info_init(struct kr_decklink_info *st);
int kr_decklink_info_valid(struct kr_decklink_info *st);
int kr_decklink_info_random(struct kr_decklink_info *st);
int kr_decklink_path_info_init(struct kr_decklink_path_info *st);
int kr_decklink_path_info_valid(struct kr_decklink_path_info *st);
int kr_decklink_path_info_random(struct kr_decklink_path_info *st);
kr_decklink_info_member kr_decklink_info_strto_member(char *string, int len);
kr_decklink_path_info_member kr_decklink_path_info_strto_member(char *string, int len);
#endif
