#include "krad_sfx_helpers.h"

int kr_sfx_control_to_index(int val) {
  switch (val) {
    case KR_SFX_EFFECT_ADD:
      return 0;
    case KR_SFX_EFFECT_RM:
      return 1;
    case KR_SFX_EFFECT_CONTROL:
      return 2;
    case KR_SFX_GET_SAMPLERATE:
      return 3;
    case KR_SFX_SET_SAMPLERATE:
      return 4;
    case KR_SFX_GET_CHANNELS:
      return 5;
    case KR_SFX_SET_CHANNELS:
      return 6;
    case KR_SFX_GET_INFO:
      return 7;
  }
  return -1;
}

char *kr_strfr_kr_sfx_control(int val) {
  switch (val) {
    case KR_SFX_EFFECT_ADD:
      return "kr_sfx_effect_add";
    case KR_SFX_EFFECT_RM:
      return "kr_sfx_effect_rm";
    case KR_SFX_EFFECT_CONTROL:
      return "kr_sfx_effect_control";
    case KR_SFX_GET_SAMPLERATE:
      return "kr_sfx_get_samplerate";
    case KR_SFX_SET_SAMPLERATE:
      return "kr_sfx_set_samplerate";
    case KR_SFX_GET_CHANNELS:
      return "kr_sfx_get_channels";
    case KR_SFX_SET_CHANNELS:
      return "kr_sfx_set_channels";
    case KR_SFX_GET_INFO:
      return "kr_sfx_get_info";
  }
  return NULL;
}

int kr_strto_kr_sfx_control(char *string) {
  if (!strcmp(string,"kr_sfx_effect_add")) {
    return KR_SFX_EFFECT_ADD;
  }
  if (!strcmp(string,"kr_sfx_effect_rm")) {
    return KR_SFX_EFFECT_RM;
  }
  if (!strcmp(string,"kr_sfx_effect_control")) {
    return KR_SFX_EFFECT_CONTROL;
  }
  if (!strcmp(string,"kr_sfx_get_samplerate")) {
    return KR_SFX_GET_SAMPLERATE;
  }
  if (!strcmp(string,"kr_sfx_set_samplerate")) {
    return KR_SFX_SET_SAMPLERATE;
  }
  if (!strcmp(string,"kr_sfx_get_channels")) {
    return KR_SFX_GET_CHANNELS;
  }
  if (!strcmp(string,"kr_sfx_set_channels")) {
    return KR_SFX_SET_CHANNELS;
  }
  if (!strcmp(string,"kr_sfx_get_info")) {
    return KR_SFX_GET_INFO;
  }

  return -1;
}

int kr_sfx_effect_type_to_index(int val) {
  switch (val) {
    case KR_SFX_NONE:
      return 0;
    case KR_SFX_EQ:
      return 1;
    case KR_SFX_LOWPASS:
      return 2;
    case KR_SFX_HIGHPASS:
      return 3;
    case KR_SFX_ANALOG:
      return 4;
  }
  return -1;
}

char *kr_strfr_kr_sfx_effect_type(int val) {
  switch (val) {
    case KR_SFX_NONE:
      return "kr_sfx_none";
    case KR_SFX_EQ:
      return "kr_sfx_eq";
    case KR_SFX_LOWPASS:
      return "kr_sfx_lowpass";
    case KR_SFX_HIGHPASS:
      return "kr_sfx_highpass";
    case KR_SFX_ANALOG:
      return "kr_sfx_analog";
  }
  return NULL;
}

int kr_strto_kr_sfx_effect_type(char *string) {
  if (!strcmp(string,"kr_sfx_none")) {
    return KR_SFX_NONE;
  }
  if (!strcmp(string,"kr_sfx_eq")) {
    return KR_SFX_EQ;
  }
  if (!strcmp(string,"kr_sfx_lowpass")) {
    return KR_SFX_LOWPASS;
  }
  if (!strcmp(string,"kr_sfx_highpass")) {
    return KR_SFX_HIGHPASS;
  }
  if (!strcmp(string,"kr_sfx_analog")) {
    return KR_SFX_ANALOG;
  }

  return -1;
}

int kr_sfx_effect_control_to_index(int val) {
  switch (val) {
    case KR_SFX_DB:
      return 0;
    case KR_SFX_BW:
      return 1;
    case KR_SFX_HZ:
      return 2;
    case KR_SFX_PASSTYPE:
      return 3;
    case KR_SFX_DRIVE:
      return 4;
    case KR_SFX_BLEND:
      return 5;
  }
  return -1;
}

char *kr_strfr_kr_sfx_effect_control(int val) {
  switch (val) {
    case KR_SFX_DB:
      return "kr_sfx_db";
    case KR_SFX_BW:
      return "kr_sfx_bw";
    case KR_SFX_HZ:
      return "kr_sfx_hz";
    case KR_SFX_PASSTYPE:
      return "kr_sfx_passtype";
    case KR_SFX_DRIVE:
      return "kr_sfx_drive";
    case KR_SFX_BLEND:
      return "kr_sfx_blend";
  }
  return NULL;
}

int kr_strto_kr_sfx_effect_control(char *string) {
  if (!strcmp(string,"kr_sfx_db")) {
    return KR_SFX_DB;
  }
  if (!strcmp(string,"kr_sfx_bw")) {
    return KR_SFX_BW;
  }
  if (!strcmp(string,"kr_sfx_hz")) {
    return KR_SFX_HZ;
  }
  if (!strcmp(string,"kr_sfx_passtype")) {
    return KR_SFX_PASSTYPE;
  }
  if (!strcmp(string,"kr_sfx_drive")) {
    return KR_SFX_DRIVE;
  }
  if (!strcmp(string,"kr_sfx_blend")) {
    return KR_SFX_BLEND;
  }

  return -1;
}

int kr_eq_band_info_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_eq_band_info_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_eq_band_info_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_eq_info_init(void *st) {
  struct kr_eq_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_eq_info *)st;
  memset(actual, 0, sizeof(struct kr_eq_info));
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    kr_eq_band_info_init(&actual->band[i]);
  }

  return 0;
}

int kr_eq_info_valid(void *st) {
  struct kr_eq_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_eq_info *)st;
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    kr_eq_band_info_valid(&actual->band[i]);
  }

  return 0;
}

int kr_eq_info_random(void *st) {
  struct kr_eq_info *actual;

  int i;

  if (st == NULL) {
    return -1;
  }

  actual = (struct kr_eq_info *)st;
  memset(actual, 0, sizeof(struct kr_eq_info));
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    kr_eq_band_info_random(&actual->band[i]);
  }

  return 0;
}

int kr_lowpass_info_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_lowpass_info_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_lowpass_info_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_highpass_info_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_highpass_info_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_highpass_info_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_analog_info_init(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_analog_info_valid(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

int kr_analog_info_random(void *st) {
  if (st == NULL) {
    return -1;
  }


  return 0;
}

