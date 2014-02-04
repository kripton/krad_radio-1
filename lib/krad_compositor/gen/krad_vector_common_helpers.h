#ifndef KRAD_VECTOR_COMMON_HELPERS_GEN_H
#define KRAD_VECTOR_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_vector_common.h"
typedef enum {
  KR_VECTOR_INFO_TYPE = 1,
  KR_VECTOR_INFO_RED = 2,
  KR_VECTOR_INFO_GREEN = 3,
  KR_VECTOR_INFO_BLUE = 4
} kr_vector_info_member;

typedef struct {
  kr_var var;
} kr_vector_info_patch_value;

typedef struct {
  kr_vector_info_member member;
  kr_vector_info_patch_value value;
} kr_vector_info_patch;

int kr_vector_info_patch_apply(struct kr_vector_info *info, kr_vector_info_patch *patch);
kr_var *kr_vector_info_patch_path(kr_vector_info_patch *patch, kr_path *path);
int kr_vector_info_init(struct kr_vector_info *st);
int kr_vector_info_valid(struct kr_vector_info *st);
int kr_vector_info_random(struct kr_vector_info *st);
kr_vector_info_member kr_vector_info_strto_member(char *string, int len);
int kr_vector_type_to_index(int val);
int kr_strto_kr_vector_type(char *string);
char *kr_strfr_kr_vector_type(int val);
#endif
