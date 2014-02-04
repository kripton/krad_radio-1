#ifndef KRAD_SPRITE_COMMON_HELPERS_GEN_H
#define KRAD_SPRITE_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_sprite_common.h"
typedef enum {
  KR_SPRITE_INFO_FILENAME = 1,
  KR_SPRITE_INFO_RATE = 2,
  KR_SPRITE_INFO_W = 3,
  KR_SPRITE_INFO_H = 4
} kr_sprite_info_member;

typedef struct {
  kr_var var;
} kr_sprite_info_patch_value;

typedef struct {
  kr_sprite_info_member member;
  kr_sprite_info_patch_value value;
} kr_sprite_info_patch;

int kr_sprite_info_patch_apply(struct kr_sprite_info *info, kr_sprite_info_patch *patch);
kr_var *kr_sprite_info_patch_path(kr_sprite_info_patch *patch, kr_path *path);
int kr_sprite_info_init(struct kr_sprite_info *st);
int kr_sprite_info_valid(struct kr_sprite_info *st);
int kr_sprite_info_random(struct kr_sprite_info *st);
kr_sprite_info_member kr_sprite_info_strto_member(char *string, int len);
#endif
