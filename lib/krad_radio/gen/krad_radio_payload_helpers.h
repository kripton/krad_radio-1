#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_radio_payload.h"
int kr_radio_payload_init(void *st, int idx);
int kr_radio_payload_valid(void *st, int idx);
int kr_radio_payload_random(void *st, int idx);
int kr_radio_payload_type_to_index(int val);
int kr_strto_kr_radio_payload_type(char *string);
char *kr_strfr_kr_radio_payload_type(int val);
