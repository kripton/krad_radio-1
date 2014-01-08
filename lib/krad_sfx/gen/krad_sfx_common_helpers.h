#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_sfx_common.h"
int kr_eq_band_info_init(void *st);
int kr_eq_band_info_valid(void *st);
int kr_eq_band_info_random(void *st);
int kr_eq_info_init(void *st);
int kr_eq_info_valid(void *st);
int kr_eq_info_random(void *st);
int kr_lowpass_info_init(void *st);
int kr_lowpass_info_valid(void *st);
int kr_lowpass_info_random(void *st);
int kr_highpass_info_init(void *st);
int kr_highpass_info_valid(void *st);
int kr_highpass_info_random(void *st);
int kr_analog_info_init(void *st);
int kr_analog_info_valid(void *st);
int kr_analog_info_random(void *st);
int kr_sfx_control_to_index(int val);
int kr_strto_kr_sfx_control(char *string);
char *kr_strfr_kr_sfx_control(int val);
int kr_sfx_effect_type_to_index(int val);
int kr_strto_kr_sfx_effect_type(char *string);
char *kr_strfr_kr_sfx_effect_type(int val);
int kr_sfx_effect_control_to_index(int val);
int kr_strto_kr_sfx_effect_control(char *string);
char *kr_strfr_kr_sfx_effect_control(int val);
