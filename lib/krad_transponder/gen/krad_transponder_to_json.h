#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_transponder_helpers.h"
#include "krad_adapter_common.h"
int kr_adapter_path_direction_to_json(char *json, void *st, int32_t max);
int kr_adapter_api_to_json(char *json, void *st, int32_t max);
int kr_adapter_api_info_to_json(char *json, void *st, int32_t max);
int kr_adapter_api_path_info_to_json(char *json, void *st, int32_t max);
int kr_adapter_info_to_json(char *json, void *st, int32_t max);
int kr_adapter_path_info_to_json(char *json, void *st, int32_t max);
