#include <stdio.h>
#include <stdint.h>
#include "gen.h"
#include "krad_transponder_common.h"
int kr_transponder_info_init(void *st);
int kr_transponder_info_valid(void *st);
int kr_transponder_info_random(void *st);
int kr_transponder_path_io_path_info_init(void *st, int idx);
int kr_transponder_path_io_path_info_valid(void *st, int idx);
int kr_transponder_path_io_path_info_random(void *st, int idx);
int kr_transponder_path_io_info_init(void *st);
int kr_transponder_path_io_info_valid(void *st);
int kr_transponder_path_io_info_random(void *st);
int kr_transponder_path_info_init(void *st);
int kr_transponder_path_info_valid(void *st);
int kr_transponder_path_info_random(void *st);
int krad_link_av_mode_t_to_index(int val);
int kr_strto_krad_link_av_mode_t(char *string);
char *kr_strfr_krad_link_av_mode_t(int val);
int kr_txpdr_su_type_t_to_index(int val);
int kr_strto_kr_txpdr_su_type_t(char *string);
char *kr_strfr_kr_txpdr_su_type_t(int val);
int krad_link_transport_mode_t_to_index(int val);
int kr_strto_krad_link_transport_mode_t(char *string);
char *kr_strfr_krad_link_transport_mode_t(int val);
int kr_transponder_path_io_type_to_index(int val);
int kr_strto_kr_transponder_path_io_type(char *string);
char *kr_strfr_kr_transponder_path_io_type(int val);
