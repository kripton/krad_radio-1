#ifndef KR_XPDR_COMMON_HELPERS_GEN_H
#define KR_XPDR_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "kr_xpdr_common.h"
typedef enum {
  KR_XPDR_PATH_INFO_TYPE = 1,
  KR_XPDR_PATH_INFO_ADP = 2
} kr_xpdr_path_info_member;

typedef struct {
  kr_var var;
} kr_xpdr_path_info_patch_value;

typedef struct {
  kr_xpdr_path_info_member member;
  kr_xpdr_path_info_patch_value value;
} kr_xpdr_path_info_patch;

int kr_xpdr_path_info_patch_apply(struct kr_xpdr_path_info *info, kr_xpdr_path_info_patch *patch);
kr_var *kr_xpdr_path_info_patch_path(kr_xpdr_path_info_patch *patch, kr_path *path);
int kr_xpdr_type_info_init(kr_xpdr_type_info *st, int idx);
int kr_xpdr_type_info_valid(kr_xpdr_type_info *st, int idx);
int kr_xpdr_type_info_random(kr_xpdr_type_info *st, int idx);
int kr_xpdr_path_info_init(struct kr_xpdr_path_info *st);
int kr_xpdr_path_info_valid(struct kr_xpdr_path_info *st);
int kr_xpdr_path_info_random(struct kr_xpdr_path_info *st);
kr_xpdr_path_info_member kr_xpdr_path_info_strto_member(char *string, int len);
int kr_xpdr_type_to_index(int val);
int kr_strto_kr_xpdr_type(char *string);
char *kr_strfr_kr_xpdr_type(int val);
#endif
