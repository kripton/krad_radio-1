#ifndef KRAD_TRANSPONDER_COMMON_HELPERS_GEN_H
#define KRAD_TRANSPONDER_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_transponder_common.h"
typedef enum {
  KR_TRANSPONDER_INFO_ACTIVE_PATHS
} kr_transponder_info_member;

typedef struct {
  int integer;
  float real;
} kr_transponder_info_patch_value;

typedef struct {
  kr_transponder_info_member member;
  kr_transponder_info_patch_value value;
} kr_transponder_info_patch;

typedef enum {
  KR_TRANSPONDER_PATH_IO_INFO_TYPE,
  KR_TRANSPONDER_PATH_IO_INFO_INFO
} kr_transponder_path_io_info_member;

typedef struct {
  int integer;
  float real;
} kr_transponder_path_io_info_patch_value;

typedef struct {
  kr_transponder_path_io_info_member member;
  kr_transponder_path_io_info_patch_value value;
} kr_transponder_path_io_info_patch;

typedef enum {
  KR_TRANSPONDER_PATH_INFO_INPUT,
  KR_TRANSPONDER_PATH_INFO_OUTPUT
} kr_transponder_path_info_member;

typedef struct {
  int integer;
  float real;
  kr_transponder_path_io_info_patch input_patch;
  kr_transponder_path_io_info_patch output_patch;
} kr_transponder_path_info_patch_value;

typedef struct {
  kr_transponder_path_info_member member;
  kr_transponder_path_info_patch_value value;
} kr_transponder_path_info_patch;

int kr_transponder_info_patch_apply(struct kr_transponder_info *info, kr_transponder_info_patch *patch);
int kr_transponder_path_io_info_patch_apply(struct kr_transponder_path_io_info *info, kr_transponder_path_io_info_patch *patch);
int kr_transponder_path_info_patch_apply(struct kr_transponder_path_info *info, kr_transponder_path_info_patch *patch);
int kr_transponder_info_init(void *st);
int kr_transponder_info_valid(void *st);
int kr_transponder_info_random(void *st);
int kr_transponder_path_io_path_info_init(void *st, int idx);
int kr_transponder_path_io_path_info_valid(void *st, int idx);
int kr_transponder_path_io_path_info_random(void *st, int idx);
int kr_transponder_path_io_info_init(void *st);
int kr_transponder_path_io_info_valid(void *st);
int kr_transponder_path_io_info_random(void *st);
int kr_transponder_path_info_init(void *st);
int kr_transponder_path_info_valid(void *st);
int kr_transponder_path_info_random(void *st);
int kr_transponder_path_patch_init(void *st);
int kr_transponder_path_patch_valid(void *st);
int kr_transponder_path_patch_random(void *st);
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
