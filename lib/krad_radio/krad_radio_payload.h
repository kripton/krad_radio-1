#ifndef KRAD_RADIO_PAYLOAD_H
#define KRAD_RADIO_PAYLOAD_H

#include "krad_radio_common.h"
#include "kr_xpdr_common.h"
#include "krad_compositor_common.h"
#include "krad_mixer_common.h"

typedef struct kr_nil kr_nil;

struct kr_nil {
  int n;
};

typedef enum {
  PL_KR_NIL,
  PL_KR_XPDR_PATH_INFO,
  PL_KR_MIXER_PATH_INFO,
  PL_KR_COMPOSITOR_PATH_INFO
} kr_payload_type;

typedef union {
  kr_nil nil;
  kr_xpdr_path_info xpdr_path_info;
  kr_mixer_path_info mixer_path_info;
  kr_compositor_path_info compositor_path_info;
} kr_payload;

#include "gen/krad_radio_payload_to_ebml.h"
#include "gen/krad_radio_payload_from_ebml.h"
#include "gen/krad_radio_payload_to_json.h"
#include "gen/krad_radio_payload_from_json.h"
#include "gen/krad_radio_payload_to_text.h"
#include "gen/krad_radio_payload_helpers.h"

#endif
