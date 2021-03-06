#ifndef KRAD_MIXER_COMMON_HELPERS_GEN_H
#define KRAD_MIXER_COMMON_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_mixer_common.h"
typedef enum {
  KR_MIXER_PATH_INFO_TYPE = 1,
  KR_MIXER_PATH_INFO_CHANNELS = 2,
  KR_MIXER_PATH_INFO_LOWPASS = 3,
  KR_MIXER_PATH_INFO_HIGHPASS = 4,
  KR_MIXER_PATH_INFO_ANALOG = 5,
  KR_MIXER_PATH_INFO_EQ = 6,
  KR_MIXER_PATH_INFO_VOLUME = 7
} kr_mixer_path_info_member;

typedef struct {
  kr_var var;
  kr_lowpass_info_patch lowpass_patch;
  kr_highpass_info_patch highpass_patch;
  kr_analog_info_patch analog_patch;
  kr_eq_info_patch eq_patch;
  kr_volume_info_patch volume_patch;
} kr_mixer_path_info_patch_value;

typedef struct {
  kr_mixer_path_info_member member;
  kr_mixer_path_info_patch_value value;
} kr_mixer_path_info_patch;

int kr_mixer_path_info_patch_apply(struct kr_mixer_path_info *info, kr_mixer_path_info_patch *patch);
kr_var *kr_mixer_path_info_patch_path(kr_mixer_path_info_patch *patch, kr_path *path);
int kr_mixer_path_info_init(struct kr_mixer_path_info *st);
int kr_mixer_path_info_valid(struct kr_mixer_path_info *st);
int kr_mixer_path_info_random(struct kr_mixer_path_info *st);
kr_mixer_path_info_member kr_mixer_path_info_strto_member(char *string, int len);
int kr_mixer_channels_to_index(int val);
int kr_strto_kr_mixer_channels(char *string);
char *kr_strfr_kr_mixer_channels(int val);
int kr_mixer_path_type_to_index(int val);
int kr_strto_kr_mixer_path_type(char *string);
char *kr_strfr_kr_mixer_path_type(int val);
#endif
