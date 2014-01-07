#ifndef KRAD_APP_COMMON_H
#define KRAD_APP_COMMON_H

#define KR_EID_CRATE 0xF1
#define KR_EID_ADDRESS 0xAE
#define KR_EID_METHOD 0xD7
#define KR_EID_PAYLOAD 0xB9
#define KR_EID_TERMINATOR 0xE2

#include "gen/krad_app_to_ebml.h"
#include "gen/krad_app_from_ebml.h"
#include "gen/krad_app_to_text.h"
#include "gen/krad_app_helpers.h"

/* here is were we link our payload */
#include "krad_radio_payload.h"

typedef struct kr_crate2 kr_crate2;

#include "krad_router.h"

typedef enum {
  KR_GET = 1,
  KR_PUT,
  KR_POST,
  KR_PATCH,
  KR_DELETE
} kr_app_method;

struct kr_crate2 {
  char address[64];
  kr_app_method method;
  kr_radio_payload_type payload_type;
  kr_radio_payload payload;
};

#endif
