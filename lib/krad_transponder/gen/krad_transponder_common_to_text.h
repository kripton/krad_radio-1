#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_transponder_common.h"
int krad_link_av_mode_t_to_text(char *text, void *st, int32_t max);
int kr_txpdr_su_type_t_to_text(char *text, void *st, int32_t max);
int krad_link_transport_mode_t_to_text(char *text, void *st, int32_t max);
int kr_transponder_path_io_type_to_text(char *text, void *st, int32_t max);
int kr_transponder_info_to_text(char *text, void *st, int32_t max);
int kr_transponder_path_io_path_info_to_text(char *text, void *st, int32_t max);
int kr_transponder_path_io_info_to_text(char *text, void *st, int32_t max);
int kr_transponder_path_info_to_text(char *text, void *st, int32_t max);