#ifndef KRAD_CLIENT_H
#define KRAD_CLIENT_H

#include <inttypes.h>

#define ALL_BROADCASTS 1

typedef struct kr_client kr_client;

#include "krad_app_common.h"
#include "krad_easing_common.h"
#include "krad_mixer_common.h"
#include "krad_compositor_common.h"
#include "krad_radio_common.h"
#include "krad_tags.h"

typedef struct kr_shm_St kr_shm_t;
typedef struct kr_crate kr_crate;
typedef struct kr_unit_path_St kr_unit_path_t;
typedef struct kr_unit_control_St kr_unit_control_t;

/*
*  An address can be:
*    Unit:           compositor/sprite/2
*    Unit & Control: compositor/sprite/2/opacity
*/

typedef struct kr_address_St kr_address_t;
typedef struct kr_address_St kr_address;

/* Top level Units */

typedef enum {
  KR_STATION = 1,
  KR_MIXER,
  KR_COMPOSITOR,
  KR_TRANSPONDER,
} kr_unit_t;

#define KR_UNIT 0
#define KR_TAGS 666

/* Subunits */

typedef enum {
  KR_PORTGROUP = 1,
  KR_EFFECT,
} kr_mixer_subunit_t;

typedef enum {
  KR_TRANSMITTER = 1,
  KR_RECEIVER,
  KR_ADAPTER,
  KR_RAWIN,
  KR_RAWOUT,
  KR_DEMUXER,
  KR_MUXER,
  KR_ENCODER,
  KR_DECODER,
} kr_transponder_subunit_type_t;

typedef enum {
  KR_STATION_UNIT = 1,
  KR_CPU,
  KR_REMOTE,
} kr_station_subunit_t;

/* Control Types */

typedef enum {
  KR_FLOAT,
  KR_INT32,
  KR_CHAR,
  KR_STRING,
} kr_unit_control_data_t;

typedef union {
  int integer;
  char *string;
  float real;
  char byte;
} kr_unit_control_value_t;

/* Control Names */

typedef enum {
  KR_BUFFER = 1,
  KR_BITRATE,
} kr_transponder_control_t;

typedef union {
  void *ptr;
  uint32_t zero;
  kr_mixer_subunit_t mixer_subunit;
  kr_compositor_subunit_type compositor_subunit;
  kr_transponder_subunit_type_t transponder_subunit;
  kr_station_subunit_t station_subunit;
} kr_subunit_t;

typedef union {
  uint32_t unit_control;
  kr_sfx_effect_control effect_control;
  kr_compositor_control compositor_control;
  kr_transponder_control_t transponder_control;
} kr_unit_control_name_t;

struct kr_unit_path_St {
  kr_unit_t unit;
  kr_subunit_t subunit;
};

typedef union {
  uint32_t number;
  char name[64];
} kr_unit_id_t;

struct kr_address_St {
  kr_unit_path_t path;
  kr_unit_control_name_t control;
  kr_unit_id_t id;
  uint32_t sub_id;
  uint32_t sub_id2;
};

struct kr_unit_control_St {
  kr_address_t address;
  kr_unit_control_data_t data_type;
  kr_unit_control_value_t value;
  uint32_t duration;
};

typedef union {
  void *actual;
  kr_tag *tag;
  kr_mixer_path_info *portgroup;
  kr_sprite_info *sprite;
  kr_text_info *text;
  kr_vector_info *vector;
  kr_compositor_path_info *videoport;
} kr_rep_ptr_t;

typedef union {
  char actual;
  kr_tag tag;
  kr_mixer_path_info portgroup;
  kr_sprite_info sprite;
  kr_text_info text;
  kr_vector_info vector;
  kr_compositor_path_info videoport;
} kr_rep_actual_t;

struct kr_crate {
  kr_client *client;
  kr_address_t *addr;
  kr_address_t address;
  kr_rep_ptr_t inside;
  kr_rep_actual_t rep;
  kr_ebml2_t payload_ebml;
  uint32_t contains;
  uint32_t notice;
  unsigned char *buffer;
  uint32_t size;
  int integer;
  float real;
  int has_int;
  int has_float;
};

int32_t kr_address_has_control (kr_address_t *address);
int kr_unit_control_data_type_from_address (kr_address_t *address, kr_unit_control_data_t *data_type);
uint32_t kr_crate_notice(kr_crate *crate);
int kr_string_to_address (char *string, kr_address_t *addr);
int kr_unit_control_set (kr_client *client, kr_unit_control_t *uc);
int kr_unit_control_get (kr_client *client, kr_unit_control_t *uc);
void kr_unit_destroy (kr_client *client, kr_address_t *address);
void kr_unit_info (kr_client *client, kr_address_t *address);
int krad_radio_address_to_ebml2 (kr_ebml2_t *ebml2, unsigned char **element_loc, kr_address_t *address);
kr_client *kr_client_create (char *client_name);
void kr_delivery_recv (kr_client *client);
int kr_connect (kr_client *client, char *sysname);
int kr_connect_full (kr_client *client, char *sysname, int timeout_ms);
int kr_connect_remote (kr_client *client, char *host, int port, int timeout_ms);
int kr_connected (kr_client *client);
int kr_disconnect (kr_client *client);
int kr_client_destroy (kr_client **client);
int kr_client_local (kr_client *client);
int kr_client_get_fd (kr_client *client);
int kr_client_sync (kr_client *client);
int kr_client_push (kr_client *client);
int kr_client_want_out (kr_client *client);
void kr_subscribe (kr_client *client, uint32_t broadcast_id);
void kr_subscribe_all (kr_client *client);
int kr_poll (kr_client *client, uint32_t timeout_ms);
int kr_crate_addr_path_match (kr_crate *crate, int unit, int subunit);
void kr_crate_recycle (kr_crate **crate);
int kr_crateo_string (kr_crate *crate, char **string);
int kr_crateo_int (kr_crate *crate, int *number);
int kr_crateo_float (kr_crate *crate, float *number);
char *kr_crate_alloc_string (int length);
void kr_crate_free_string (char **string);
int kr_uncrate_rep (kr_crate *crate);
void kr_crate_address_get(kr_crate *crate, kr_address_t **address);
void kr_address_debug_print (kr_address_t *addr);
int krad_read_address_from_ebml (kr_ebml2_t *ebml, kr_address_t *address);
int krad_message_notice_has_payload (uint32_t type);
void kr_delivery_accept_and_report(kr_client *client);
int kr_delivery_get (kr_client *client, kr_crate **crate);
#define kr_crate_loaded kr_uncrate_rep
int kr_crate_loaded (kr_crate *crate);
int kr_crate_has_int (kr_crate *crate);
int kr_crate_has_float (kr_crate *crate);
#define kr_crate_contains_float kr_crate_has_float
#define kr_crate_contains_int kr_crate_has_int
#define kr_crate_contains_integer kr_crate_has_int
#define kr_delivery_wait kr_poll
#define kr_wait kr_poll
#define kr_address_get kr_crate_address
#define kr_uncrate_string kr_crateo_string
#define kr_uncrate_to_string kr_crateo_string
#define kr_uncrate_int kr_crateo_int
#define kr_uncrate kr_uncrate_rep
#define kr_uncrate_float kr_crateo_float
#define kr_string_release kr_crate_free_string
#define kr_string_recycle kr_crate_free_string
#define kr_string_goodbye kr_crate_free_string
int kr_delivery_get_until_final (kr_client *client, kr_crate **crate, uint32_t timeout_ms);
int kr_delivery_final (kr_client *client);
void kr_client_crate_wait(kr_client *client, kr_crate **crate);
int kr_crate_send(kr_client *client, kr_crate2 *crate);
int kr_get(kr_client *client, char *address);
int kr_streamer45(kr_client *client);
#endif
