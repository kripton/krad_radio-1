#ifndef KRAD_APP_COMMON_HELPERS_GEN_H
#define KRAD_APP_COMMON_HELPERS_GEN_H
#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_app_common.h"
int kr_crate2_init(void *st);
int kr_crate2_valid(void *st);
int kr_crate2_random(void *st);
int kr_app_method_to_index(int val);
int kr_strto_kr_app_method(char *string);
char *kr_strfr_kr_app_method(int val);
#endif
