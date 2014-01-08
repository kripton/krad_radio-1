#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_ebmlx.h"
#include "krad_adapter_common.h"
int kr_adapter_path_direction_to_ebml(kr_ebml *ebml, void *st);
int kr_adapter_api_to_ebml(kr_ebml *ebml, void *st);
int kr_adapter_api_info_to_ebml(kr_ebml *ebml, void *st);
int kr_adapter_api_path_info_to_ebml(kr_ebml *ebml, void *st);
int kr_adapter_info_to_ebml(kr_ebml *ebml, void *st);
int kr_adapter_path_info_to_ebml(kr_ebml *ebml, void *st);
