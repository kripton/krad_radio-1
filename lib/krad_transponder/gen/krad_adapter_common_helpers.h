#ifndef KRAD_ADAPTER_COMMON_HELPERS_GEN_H
#define KRAD_ADAPTER_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_adapter_common.h"
typedef enum {
  KR_ADAPTER_INFO_API = 1,
  KR_ADAPTER_INFO_API_INFO = 2
} kr_adapter_info_member;

typedef struct {
  kr_var var;
} kr_adapter_info_patch_value;

typedef struct {
  kr_adapter_info_member member;
  kr_adapter_info_patch_value value;
} kr_adapter_info_patch;

typedef enum {
  KR_ADAPTER_PATH_INFO_NAME = 1,
  KR_ADAPTER_PATH_INFO_DIR = 2,
  KR_ADAPTER_PATH_INFO_API = 3,
  KR_ADAPTER_PATH_INFO_INFO = 4
} kr_adapter_path_info_member;

typedef struct {
  kr_var var;
} kr_adapter_path_info_patch_value;

typedef struct {
  kr_adapter_path_info_member member;
  kr_adapter_path_info_patch_value value;
} kr_adapter_path_info_patch;

int kr_adapter_info_patch_apply(struct kr_adapter_info *info, kr_adapter_info_patch *patch);
kr_var *kr_adapter_info_patch_path(kr_adapter_info_patch *patch, kr_path *path);
int kr_adapter_path_info_patch_apply(struct kr_adapter_path_info *info, kr_adapter_path_info_patch *patch);
kr_var *kr_adapter_path_info_patch_path(kr_adapter_path_info_patch *patch, kr_path *path);
int kr_adapter_api_info_init(kr_adapter_api_info *st, int idx);
int kr_adapter_api_info_valid(kr_adapter_api_info *st, int idx);
int kr_adapter_api_info_random(kr_adapter_api_info *st, int idx);
int kr_adapter_api_path_info_init(kr_adapter_api_path_info *st, int idx);
int kr_adapter_api_path_info_valid(kr_adapter_api_path_info *st, int idx);
int kr_adapter_api_path_info_random(kr_adapter_api_path_info *st, int idx);
int kr_adapter_info_init(struct kr_adapter_info *st);
int kr_adapter_info_valid(struct kr_adapter_info *st);
int kr_adapter_info_random(struct kr_adapter_info *st);
int kr_adapter_path_info_init(struct kr_adapter_path_info *st);
int kr_adapter_path_info_valid(struct kr_adapter_path_info *st);
int kr_adapter_path_info_random(struct kr_adapter_path_info *st);
kr_adapter_info_member kr_adapter_info_strto_member(char *string, int len);
kr_adapter_path_info_member kr_adapter_path_info_strto_member(char *string, int len);
int kr_adapter_path_direction_to_index(int val);
int kr_strto_kr_adapter_path_direction(char *string);
char *kr_strfr_kr_adapter_path_direction(int val);
int kr_adapter_api_to_index(int val);
int kr_strto_kr_adapter_api(char *string);
char *kr_strfr_kr_adapter_api(int val);
#endif
