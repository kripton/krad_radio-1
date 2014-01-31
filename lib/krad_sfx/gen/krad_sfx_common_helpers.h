#ifndef KRAD_SFX_COMMON_HELPERS_GEN_H
#define KRAD_SFX_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_sfx_common.h"
typedef enum {
  KR_EQ_BAND_INFO_DB,
  KR_EQ_BAND_INFO_BW,
  KR_EQ_BAND_INFO_HZ
} kr_eq_band_info_member;

typedef struct {
  int integer;
  float real;
} kr_eq_band_info_patch_value;

typedef struct {
  kr_eq_band_info_member member;
  kr_eq_band_info_patch_value value;
} kr_eq_band_info_patch;

typedef enum {
  KR_EQ_INFO_BAND
} kr_eq_info_member;

typedef struct {
  int integer;
  float real;
  kr_eq_band_info_patch band_patch;
} kr_eq_info_patch_value;

typedef struct {
  kr_eq_info_member member;
  kr_eq_info_patch_value value;
} kr_eq_info_patch;

typedef enum {
  KR_LOWPASS_INFO_BW,
  KR_LOWPASS_INFO_HZ
} kr_lowpass_info_member;

typedef struct {
  int integer;
  float real;
} kr_lowpass_info_patch_value;

typedef struct {
  kr_lowpass_info_member member;
  kr_lowpass_info_patch_value value;
} kr_lowpass_info_patch;

typedef enum {
  KR_HIGHPASS_INFO_BW,
  KR_HIGHPASS_INFO_HZ
} kr_highpass_info_member;

typedef struct {
  int integer;
  float real;
} kr_highpass_info_patch_value;

typedef struct {
  kr_highpass_info_member member;
  kr_highpass_info_patch_value value;
} kr_highpass_info_patch;

typedef enum {
  KR_VOLUME_INFO_LEVEL
} kr_volume_info_member;

typedef struct {
  int integer;
  float real;
} kr_volume_info_patch_value;

typedef struct {
  kr_volume_info_member member;
  kr_volume_info_patch_value value;
} kr_volume_info_patch;

typedef enum {
  KR_ANALOG_INFO_DRIVE,
  KR_ANALOG_INFO_BLEND
} kr_analog_info_member;

typedef struct {
  int integer;
  float real;
} kr_analog_info_patch_value;

typedef struct {
  kr_analog_info_member member;
  kr_analog_info_patch_value value;
} kr_analog_info_patch;

int kr_eq_band_info_patch_apply(struct kr_eq_band_info *info, kr_eq_band_info_patch *patch);
int kr_eq_info_patch_apply(struct kr_eq_info *info, kr_eq_info_patch *patch);
int kr_lowpass_info_patch_apply(struct kr_lowpass_info *info, kr_lowpass_info_patch *patch);
int kr_highpass_info_patch_apply(struct kr_highpass_info *info, kr_highpass_info_patch *patch);
int kr_volume_info_patch_apply(struct kr_volume_info *info, kr_volume_info_patch *patch);
int kr_analog_info_patch_apply(struct kr_analog_info *info, kr_analog_info_patch *patch);
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
int kr_volume_info_init(void *st);
int kr_volume_info_valid(void *st);
int kr_volume_info_random(void *st);
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
#endif
