#ifndef KR_AUX_COMMON_HELPERS_GEN_H
#define KR_AUX_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "kr_aux_common.h"
typedef enum {
  KR_AUX_INFO_MONKEYNAME = 1
} kr_aux_info_member;

typedef struct {
  kr_var var;
} kr_aux_info_patch_value;

typedef struct {
  kr_aux_info_member member;
  kr_aux_info_patch_value value;
} kr_aux_info_patch;

typedef enum {
  KR_AUX_PATH_INFO_NCOCONUTS = 1
} kr_aux_path_info_member;

typedef struct {
  kr_var var;
} kr_aux_path_info_patch_value;

typedef struct {
  kr_aux_path_info_member member;
  kr_aux_path_info_patch_value value;
} kr_aux_path_info_patch;

int kr_aux_info_patch_apply(struct kr_aux_info *info, kr_aux_info_patch *patch);
kr_var *kr_aux_info_patch_path(kr_aux_info_patch *patch, kr_path *path);
int kr_aux_path_info_patch_apply(struct kr_aux_path_info *info, kr_aux_path_info_patch *patch);
kr_var *kr_aux_path_info_patch_path(kr_aux_path_info_patch *patch, kr_path *path);
int kr_aux_info_init(struct kr_aux_info *st);
int kr_aux_info_valid(struct kr_aux_info *st);
int kr_aux_info_random(struct kr_aux_info *st);
int kr_aux_path_info_init(struct kr_aux_path_info *st);
int kr_aux_path_info_valid(struct kr_aux_path_info *st);
int kr_aux_path_info_random(struct kr_aux_path_info *st);
kr_aux_info_member kr_aux_info_strto_member(char *string, int len);
kr_aux_path_info_member kr_aux_path_info_strto_member(char *string, int len);
#endif
