#ifndef KRAD_APP_PAYLOAD_H
#define KRAD_APP_PAYLOAD_H

typedef enum {
  KR_GET = 1,
  KR_PUT,
  KR_POST,
  KR_PATCH,
  KR_DELETE
} kr_method;

typedef struct kr_crate kr_crate;

/* here is were we link our payload */
#include "krad_radio_payload.h"

struct kr_crate {
  char address[64];
  kr_method method;
  kr_payload_type payload_type;
  kr_payload payload;
};

#include "gen/krad_crate_to_ebml.h"
#include "gen/krad_crate_from_ebml.h"
#include "gen/krad_crate_to_json.h"
#include "gen/krad_crate_from_json.h"
#include "gen/krad_crate_to_text.h"
#include "gen/krad_crate_helpers.h"

#endif
