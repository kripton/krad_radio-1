#ifndef KRAD_TEXT_COMMON_HELPERS_GEN_H
#define KRAD_TEXT_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_text_common.h"
typedef enum {
  KR_TEXT_INFO_STRING = 1,
  KR_TEXT_INFO_FONT = 2,
  KR_TEXT_INFO_RED = 3,
  KR_TEXT_INFO_GREEN = 4,
  KR_TEXT_INFO_BLUE = 5
} kr_text_info_member;

typedef struct {
  kr_var var;
} kr_text_info_patch_value;

typedef struct {
  kr_text_info_member member;
  kr_text_info_patch_value value;
} kr_text_info_patch;

int kr_text_info_patch_apply(struct kr_text_info *info, kr_text_info_patch *patch);
kr_var *kr_text_info_patch_path(kr_text_info_patch *patch, kr_path *path);
int kr_text_info_init(struct kr_text_info *st);
int kr_text_info_valid(struct kr_text_info *st);
int kr_text_info_random(struct kr_text_info *st);
kr_text_info_member kr_text_info_strto_member(char *string, int len);
#endif
