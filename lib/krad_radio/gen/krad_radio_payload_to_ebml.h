#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_ebmlx.h"
#include "krad_radio_payload.h"
int kr_radio_payload_type_to_ebml(kr_ebml *ebml, void *st);
int kr_radio_payload_to_ebml(kr_ebml *ebml, void *st);