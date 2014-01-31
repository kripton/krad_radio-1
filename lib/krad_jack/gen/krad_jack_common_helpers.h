#ifndef KRAD_JACK_COMMON_HELPERS_GEN_H
#define KRAD_JACK_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_jack_common.h"
typedef enum {
  KR_JACK_SETUP_INFO_CLIENT_NAME,
  KR_JACK_SETUP_INFO_SERVER_NAME
} kr_jack_setup_info_member;

typedef struct {
  int integer;
  float real;
} kr_jack_setup_info_patch_value;

typedef struct {
  kr_jack_setup_info_member member;
  kr_jack_setup_info_patch_value value;
} kr_jack_setup_info_patch;

typedef enum {
  KR_JACK_INFO_CLIENT_NAME,
  KR_JACK_INFO_SERVER_NAME,
  KR_JACK_INFO_STATE,
  KR_JACK_INFO_INPUTS,
  KR_JACK_INFO_OUTPUTS,
  KR_JACK_INFO_SAMPLE_RATE,
  KR_JACK_INFO_PERIOD_SIZE,
  KR_JACK_INFO_XRUNS,
  KR_JACK_INFO_FRAMES
} kr_jack_info_member;

typedef struct {
  int integer;
  float real;
} kr_jack_info_patch_value;

typedef struct {
  kr_jack_info_member member;
  kr_jack_info_patch_value value;
} kr_jack_info_patch;

typedef enum {
  KR_JACK_PATH_INFO_NAME,
  KR_JACK_PATH_INFO_CHANNELS,
  KR_JACK_PATH_INFO_DIRECTION
} kr_jack_path_info_member;

typedef struct {
  int integer;
  float real;
} kr_jack_path_info_patch_value;

typedef struct {
  kr_jack_path_info_member member;
  kr_jack_path_info_patch_value value;
} kr_jack_path_info_patch;

int kr_jack_setup_info_patch_apply(kr_jack_setup_info *info, kr_jack_setup_info_patch *patch);
int kr_jack_info_patch_apply(kr_jack_info *info, kr_jack_info_patch *patch);
int kr_jack_path_info_patch_apply(kr_jack_path_info *info, kr_jack_path_info_patch *patch);
int kr_jack_setup_info_init(void *st);
int kr_jack_setup_info_valid(void *st);
int kr_jack_setup_info_random(void *st);
int kr_jack_info_init(void *st);
int kr_jack_info_valid(void *st);
int kr_jack_info_random(void *st);
int kr_jack_path_info_init(void *st);
int kr_jack_path_info_valid(void *st);
int kr_jack_path_info_random(void *st);
int kr_jack_direction_to_index(int val);
int kr_strto_kr_jack_direction(char *string);
char *kr_strfr_kr_jack_direction(int val);
int kr_jack_state_to_index(int val);
int kr_strto_kr_jack_state(char *string);
char *kr_strfr_kr_jack_state(int val);
#endif
