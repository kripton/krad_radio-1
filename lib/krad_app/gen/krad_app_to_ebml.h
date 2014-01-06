#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_ebmlx.h"
#include "krad_app_common.h"
int kr_radio_payload_type_to_ebml(kr_ebml *ebml, void *st);
int kr_radio_payload_to_ebml(kr_ebml *ebml, void *st);
int kr_app_method_to_ebml(kr_ebml *ebml, void *st);
int kr_crate2_to_ebml(kr_ebml *ebml, void *st);
