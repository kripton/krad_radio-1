#ifndef KRAD_ALSA_COMMON_H
#define KRAD_ALSA_COMMON_H

typedef struct kr_alsa_info kr_alsa_info;
typedef struct kr_alsa_path_info kr_alsa_path_info;

struct kr_alsa_info {
  int card;
  char name[40];
};

struct kr_alsa_path_info {
  int card_num;
};

#include "gen/krad_alsa_common_to_ebml.h"
#include "gen/krad_alsa_common_from_ebml.h"
#include "gen/krad_alsa_common_to_text.h"
#include "gen/krad_alsa_common_to_json.h"
#include "gen/krad_alsa_common_from_json.h"
#include "gen/krad_alsa_common_helpers.h"

#endif
