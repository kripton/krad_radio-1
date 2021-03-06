#ifndef KRAD_JACK_COMMON_HELPERS_GEN_H
#define KRAD_JACK_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_jack_common.h"
typedef enum {
  KR_JACK_SETUP_INFO_CLIENT_NAME = 1,
  KR_JACK_SETUP_INFO_SERVER_NAME = 2
} kr_jack_setup_info_member;

typedef struct {
  kr_var var;
} kr_jack_setup_info_patch_value;

typedef struct {
  kr_jack_setup_info_member member;
  kr_jack_setup_info_patch_value value;
} kr_jack_setup_info_patch;

typedef enum {
  KR_JACK_INFO_CLIENT_NAME = 1,
  KR_JACK_INFO_SERVER_NAME = 2,
  KR_JACK_INFO_STATE = 3,
  KR_JACK_INFO_INPUTS = 4,
  KR_JACK_INFO_OUTPUTS = 5,
  KR_JACK_INFO_SAMPLE_RATE = 6,
  KR_JACK_INFO_PERIOD_SIZE = 7,
  KR_JACK_INFO_XRUNS = 8,
  KR_JACK_INFO_FRAMES = 9
} kr_jack_info_member;

typedef struct {
  kr_var var;
} kr_jack_info_patch_value;

typedef struct {
  kr_jack_info_member member;
  kr_jack_info_patch_value value;
} kr_jack_info_patch;

typedef enum {
  KR_JACK_PATH_INFO_NAME = 1,
  KR_JACK_PATH_INFO_CHANNELS = 2,
  KR_JACK_PATH_INFO_DIRECTION = 3
} kr_jack_path_info_member;

typedef struct {
  kr_var var;
} kr_jack_path_info_patch_value;

typedef struct {
  kr_jack_path_info_member member;
  kr_jack_path_info_patch_value value;
} kr_jack_path_info_patch;

int kr_jack_setup_info_patch_apply(kr_jack_setup_info *info, kr_jack_setup_info_patch *patch);
kr_var *kr_jack_setup_info_patch_path(kr_jack_setup_info_patch *patch, kr_path *path);
int kr_jack_info_patch_apply(kr_jack_info *info, kr_jack_info_patch *patch);
kr_var *kr_jack_info_patch_path(kr_jack_info_patch *patch, kr_path *path);
int kr_jack_path_info_patch_apply(kr_jack_path_info *info, kr_jack_path_info_patch *patch);
kr_var *kr_jack_path_info_patch_path(kr_jack_path_info_patch *patch, kr_path *path);
int kr_jack_setup_info_init(kr_jack_setup_info *st);
int kr_jack_setup_info_valid(kr_jack_setup_info *st);
int kr_jack_setup_info_random(kr_jack_setup_info *st);
int kr_jack_info_init(kr_jack_info *st);
int kr_jack_info_valid(kr_jack_info *st);
int kr_jack_info_random(kr_jack_info *st);
int kr_jack_path_info_init(kr_jack_path_info *st);
int kr_jack_path_info_valid(kr_jack_path_info *st);
int kr_jack_path_info_random(kr_jack_path_info *st);
kr_jack_setup_info_member kr_jack_setup_info_strto_member(char *string, int len);
kr_jack_info_member kr_jack_info_strto_member(char *string, int len);
kr_jack_path_info_member kr_jack_path_info_strto_member(char *string, int len);
int kr_jack_direction_to_index(int val);
int kr_strto_kr_jack_direction(char *string);
char *kr_strfr_kr_jack_direction(int val);
int kr_jack_state_to_index(int val);
int kr_strto_kr_jack_state(char *string);
char *kr_strfr_kr_jack_state(int val);
#endif
