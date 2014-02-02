#ifndef KRAD_RADIO_PAYLOAD_HELPERS_GEN_H
#define KRAD_RADIO_PAYLOAD_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_radio_payload.h"
int kr_nil_init(struct kr_nil *st);
int kr_nil_valid(struct kr_nil *st);
int kr_nil_random(struct kr_nil *st);
int kr_radio_payload_init(kr_radio_payload *st, int idx);
int kr_radio_payload_valid(kr_radio_payload *st, int idx);
int kr_radio_payload_random(kr_radio_payload *st, int idx);
int kr_radio_payload_type_to_index(int val);
int kr_strto_kr_radio_payload_type(char *string);
char *kr_strfr_kr_radio_payload_type(int val);
#endif
