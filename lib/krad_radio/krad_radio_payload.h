#ifndef KRAD_RADIO_PAYLOAD_H
#define KRAD_RADIO_PAYLOAD_H

#include "krad_radio_common.h"
#include "krad_transponder_common.h"
#include "krad_compositor_common.h"
#include "krad_mixer_common.h"

#include "gen/krad_radio_payload_to_ebml.h"
#include "gen/krad_radio_payload_from_ebml.h"
#include "gen/krad_radio_payload_to_text.h"
#include "gen/krad_radio_payload_helpers.h"

typedef enum {
  PL_KR_TRANSPONDER_PATH_INFO,
  PL_KR_MIXER_PATH_INFO
} kr_radio_payload_type;

typedef union {
  kr_transponder_path_info transponder_path_info;
  kr_mixer_path_info mixer_path_info;
  kr_compositor_path_info compositor_path_info;
} kr_radio_payload;

#endif
