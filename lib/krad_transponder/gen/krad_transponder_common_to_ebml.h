#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_ebmlx.h"
#include "krad_transponder_common.h"
int krad_link_av_mode_t_to_ebml(kr_ebml *ebml, void *st);
int kr_txpdr_su_type_t_to_ebml(kr_ebml *ebml, void *st);
int krad_link_transport_mode_t_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_path_io_type_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_info_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_path_io_path_info_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_path_io_info_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_path_info_to_ebml(kr_ebml *ebml, void *st);
int kr_transponder_path_patch_to_ebml(kr_ebml *ebml, void *st);
