#ifndef KRAD_MIXER_COMMON_H
#define KRAD_MIXER_COMMON_H

#define KR_MXR_PATHS_DEF 12
#define KR_MXR_MAX_CHANNELS 8
#define KR_MXR_SRATE_MIN 44100
#define KR_MXR_SRATE_MAX 48000
#define KR_MXR_PERIOD_MIN 16
#define KR_MXR_PERIOD_MAX 1024

#define KR_MXR_SRATE_DEF 48000
#define KR_MXR_PERIOD_DEF 1024

#include "krad_sfx_common.h"
#include "krad_easing_common.h"

#include "gen/krad_mixer_common_to_json.h"
#include "gen/krad_mixer_common_from_json.h"
#include "gen/krad_mixer_common_to_ebml.h"
#include "gen/krad_mixer_common_from_ebml.h"
#include "gen/krad_mixer_common_to_text.h"
#include "gen/krad_mixer_common_helpers.h"

typedef struct kr_mixer_path_info kr_mixer_path_info;
typedef struct kr_mixer_path_info kr_mixer_source_info;
typedef struct kr_mixer_path_info kr_mixer_input_info;
typedef struct kr_mixer_path_info kr_mixer_bus_info;
typedef struct kr_mixer_path_info kr_mixer_output_info;
typedef struct kr_mixer_path_patch kr_mixer_path_patch;

typedef enum {
  NIL,
  MONO,
  STEREO,
  THREE,
  QUAD,
  FIVE,
  SIX,
  SEVEN,
  EIGHT
} kr_mixer_channels;

typedef enum {
  KR_MXR_SOURCE = 1,
  KR_MXR_INPUT,
  KR_MXR_BUS,
  KR_MXR_OUTPUT
} kr_mixer_path_type;

struct kr_mixer_path_info {
  kr_mixer_path_type type;
  kr_mixer_channels channels;
  kr_lowpass_info lowpass;
  kr_highpass_info highpass;
  kr_analog_info analog;
  kr_eq_info eq;
  kr_volume_info volume;
};

struct kr_mixer_path_patch {
  char ctl[16];
  float val;
  int ms;
};

#endif
