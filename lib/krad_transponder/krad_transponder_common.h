#ifndef KRAD_TRANSPONDER_COMMON_H
#define KRAD_TRANSPONDER_COMMON_H

#include <string.h>
#include "krad_ebml.h"
#include "krad_codec_header.h"
#include "krad_mixer_common.h"
#include "krad_compositor_common.h"
#include "krad_adapter_common.h"

typedef struct kr_transponder_path_info kr_xpdr_path_info;
typedef struct kr_transponder_path_io_info kr_xpdr_path_io_info;
typedef struct kr_transponder_path_info kr_transponder_path_info;
typedef struct kr_transponder_path_io_info kr_transponder_path_io_info;

typedef enum {
  KR_XPDR_MIXER,
  KR_XPDR_COMPOSITOR,
  KR_XPDR_ADAPTER
} kr_transponder_path_io_type;

typedef kr_transponder_path_io_type kr_xpdr_path_io_type;

typedef union {
  kr_mixer_path_info mixer_path_info;
  kr_compositor_path_info compositor_path_info;
  kr_adapter_path_info adapter_path_info;
} kr_transponder_path_io_path_info;

struct kr_transponder_path_io_info {
  kr_transponder_path_io_type type;
  kr_transponder_path_io_path_info info;
};

struct kr_transponder_path_info {
  kr_transponder_path_io_info input;
  kr_transponder_path_io_info output;
};

#include "gen/krad_transponder_common_to_ebml.h"
#include "gen/krad_transponder_common_from_ebml.h"
#include "gen/krad_transponder_common_helpers.h"
#include "gen/krad_transponder_common_to_text.h"
#include "gen/krad_transponder_common_to_json.h"
#include "gen/krad_transponder_common_from_json.h"

#endif
