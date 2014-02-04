#ifndef KRAD_CRATE_HELPERS_GEN_H
#define KRAD_CRATE_HELPERS_GEN_H
#include "krad_path.h"
#include "krad_variant.h"
#include "gen.h"
#include "krad_crate.h"
int kr_crate_init(struct kr_crate *st);
int kr_crate_valid(struct kr_crate *st);
int kr_crate_random(struct kr_crate *st);
int kr_method_to_index(int val);
int kr_strto_kr_method(char *string);
char *kr_strfr_kr_method(int val);
#endif
