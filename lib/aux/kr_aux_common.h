#ifndef KR_AUX_COMMON_H
#define KR_AUX_COMMON_H

typedef struct kr_aux_info kr_aux_info;
typedef struct kr_aux_path_info kr_aux_path_info;

#include "gen/kr_aux_common_to_ebml.h"
#include "gen/kr_aux_common_from_ebml.h"
#include "gen/kr_aux_common_to_text.h"
#include "gen/kr_aux_common_to_json.h"
#include "gen/kr_aux_common_from_json.h"
#include "gen/kr_aux_common_helpers.h"

struct kr_aux_info {
  char monkeyname[64];
};

struct kr_aux_path_info {
  int ncoconuts;
};

#endif
