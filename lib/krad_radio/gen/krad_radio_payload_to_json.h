#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_radio_payload_helpers.h"
#include "krad_radio_payload.h"
int kr_nil_to_json(char *json, void *st, int32_t max);
int kr_radio_payload_type_to_json(char *json, void *st, int32_t max);
int kr_radio_payload_to_json(char *json, void *st, int32_t max);
