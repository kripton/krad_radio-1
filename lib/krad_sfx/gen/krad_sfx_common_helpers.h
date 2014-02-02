#ifndef KRAD_SFX_COMMON_HELPERS_GEN_H
#define KRAD_SFX_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_sfx_common.h"
typedef enum {
  KR_EQ_BAND_INFO_DB = 1,
  KR_EQ_BAND_INFO_BW = 2,
  KR_EQ_BAND_INFO_HZ = 3
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
  KR_EQ_INFO_BAND = 1
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
  KR_LOWPASS_INFO_BW = 1,
  KR_LOWPASS_INFO_HZ = 2
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
  KR_HIGHPASS_INFO_BW = 1,
  KR_HIGHPASS_INFO_HZ = 2
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
  KR_VOLUME_INFO_LEVEL = 1
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
  KR_ANALOG_INFO_DRIVE = 1,
  KR_ANALOG_INFO_BLEND = 2
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
kr_value *kr_eq_band_info_address_to_patch(kr_eq_band_info_patch *patch, kr_address2 *addr);
int kr_eq_info_patch_apply(struct kr_eq_info *info, kr_eq_info_patch *patch);
kr_value *kr_eq_info_address_to_patch(kr_eq_info_patch *patch, kr_address2 *addr);
int kr_lowpass_info_patch_apply(struct kr_lowpass_info *info, kr_lowpass_info_patch *patch);
kr_value *kr_lowpass_info_address_to_patch(kr_lowpass_info_patch *patch, kr_address2 *addr);
int kr_highpass_info_patch_apply(struct kr_highpass_info *info, kr_highpass_info_patch *patch);
kr_value *kr_highpass_info_address_to_patch(kr_highpass_info_patch *patch, kr_address2 *addr);
int kr_volume_info_patch_apply(struct kr_volume_info *info, kr_volume_info_patch *patch);
kr_value *kr_volume_info_address_to_patch(kr_volume_info_patch *patch, kr_address2 *addr);
int kr_analog_info_patch_apply(struct kr_analog_info *info, kr_analog_info_patch *patch);
kr_value *kr_analog_info_address_to_patch(kr_analog_info_patch *patch, kr_address2 *addr);
int kr_eq_band_info_init(struct kr_eq_band_info *st);
int kr_eq_band_info_valid(struct kr_eq_band_info *st);
int kr_eq_band_info_random(struct kr_eq_band_info *st);
int kr_eq_info_init(struct kr_eq_info *st);
int kr_eq_info_valid(struct kr_eq_info *st);
int kr_eq_info_random(struct kr_eq_info *st);
int kr_lowpass_info_init(struct kr_lowpass_info *st);
int kr_lowpass_info_valid(struct kr_lowpass_info *st);
int kr_lowpass_info_random(struct kr_lowpass_info *st);
int kr_highpass_info_init(struct kr_highpass_info *st);
int kr_highpass_info_valid(struct kr_highpass_info *st);
int kr_highpass_info_random(struct kr_highpass_info *st);
int kr_volume_info_init(struct kr_volume_info *st);
int kr_volume_info_valid(struct kr_volume_info *st);
int kr_volume_info_random(struct kr_volume_info *st);
int kr_analog_info_init(struct kr_analog_info *st);
int kr_analog_info_valid(struct kr_analog_info *st);
int kr_analog_info_random(struct kr_analog_info *st);
kr_eq_band_info_member kr_eq_band_info_strto_member(char *string, int len);
kr_eq_info_member kr_eq_info_strto_member(char *string, int len);
kr_lowpass_info_member kr_lowpass_info_strto_member(char *string, int len);
kr_highpass_info_member kr_highpass_info_strto_member(char *string, int len);
kr_volume_info_member kr_volume_info_strto_member(char *string, int len);
kr_analog_info_member kr_analog_info_strto_member(char *string, int len);
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
