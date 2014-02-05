#ifndef KRAD_TRANSPONDER_COMMON_HELPERS_GEN_H
#define KRAD_TRANSPONDER_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_transponder_common.h"
typedef enum {
  KR_TRANSPONDER_PATH_IO_INFO_TYPE = 1,
  KR_TRANSPONDER_PATH_IO_INFO_INFO = 2
} kr_transponder_path_io_info_member;

typedef struct {
  kr_var var;
} kr_transponder_path_io_info_patch_value;

typedef struct {
  kr_transponder_path_io_info_member member;
  kr_transponder_path_io_info_patch_value value;
} kr_transponder_path_io_info_patch;

typedef enum {
  KR_TRANSPONDER_PATH_INFO_INPUT = 1,
  KR_TRANSPONDER_PATH_INFO_OUTPUT = 2
} kr_transponder_path_info_member;

typedef struct {
  kr_var var;
  kr_transponder_path_io_info_patch input_patch;
  kr_transponder_path_io_info_patch output_patch;
} kr_transponder_path_info_patch_value;

typedef struct {
  kr_transponder_path_info_member member;
  kr_transponder_path_info_patch_value value;
} kr_transponder_path_info_patch;

int kr_transponder_path_io_info_patch_apply(struct kr_transponder_path_io_info *info, kr_transponder_path_io_info_patch *patch);
kr_var *kr_transponder_path_io_info_patch_path(kr_transponder_path_io_info_patch *patch, kr_path *path);
int kr_transponder_path_info_patch_apply(struct kr_transponder_path_info *info, kr_transponder_path_info_patch *patch);
kr_var *kr_transponder_path_info_patch_path(kr_transponder_path_info_patch *patch, kr_path *path);
int kr_transponder_path_io_path_info_init(kr_transponder_path_io_path_info *st, int idx);
int kr_transponder_path_io_path_info_valid(kr_transponder_path_io_path_info *st, int idx);
int kr_transponder_path_io_path_info_random(kr_transponder_path_io_path_info *st, int idx);
int kr_transponder_path_io_info_init(struct kr_transponder_path_io_info *st);
int kr_transponder_path_io_info_valid(struct kr_transponder_path_io_info *st);
int kr_transponder_path_io_info_random(struct kr_transponder_path_io_info *st);
int kr_transponder_path_info_init(struct kr_transponder_path_info *st);
int kr_transponder_path_info_valid(struct kr_transponder_path_info *st);
int kr_transponder_path_info_random(struct kr_transponder_path_info *st);
kr_transponder_path_io_info_member kr_transponder_path_io_info_strto_member(char *string, int len);
kr_transponder_path_info_member kr_transponder_path_info_strto_member(char *string, int len);
int krad_link_av_mode_t_to_index(int val);
int kr_strto_krad_link_av_mode_t(char *string);
char *kr_strfr_krad_link_av_mode_t(int val);
int kr_txpdr_su_type_t_to_index(int val);
int kr_strto_kr_txpdr_su_type_t(char *string);
char *kr_strfr_kr_txpdr_su_type_t(int val);
int krad_link_transport_mode_t_to_index(int val);
int kr_strto_krad_link_transport_mode_t(char *string);
char *kr_strfr_krad_link_transport_mode_t(int val);
int kr_transponder_path_io_type_to_index(int val);
int kr_strto_kr_transponder_path_io_type(char *string);
char *kr_strfr_kr_transponder_path_io_type(int val);
#endif
