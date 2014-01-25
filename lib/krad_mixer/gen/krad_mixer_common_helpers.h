#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_mixer_common.h"
int kr_mixer_path_info_init(void *st);
int kr_mixer_path_info_valid(void *st);
int kr_mixer_path_info_random(void *st);
int kr_mixer_path_patch_init(void *st);
int kr_mixer_path_patch_valid(void *st);
int kr_mixer_path_patch_random(void *st);
int kr_mixer_channels_to_index(int val);
int kr_strto_kr_mixer_channels(char *string);
char *kr_strfr_kr_mixer_channels(int val);
int kr_mixer_control_to_index(int val);
int kr_strto_kr_mixer_control(char *string);
char *kr_strfr_kr_mixer_control(int val);
int kr_mixer_path_type_to_index(int val);
int kr_strto_kr_mixer_path_type(char *string);
char *kr_strfr_kr_mixer_path_type(int val);
int kr_mixer_adv_ctl_to_index(int val);
int kr_strto_kr_mixer_adv_ctl(char *string);
char *kr_strfr_kr_mixer_adv_ctl(int val);
