//#ifndef KRAD_CLIENT_INTERNAL_H
//#define KRAD_CLIENT_INTERNAL_H

#include "krad_app_client.h"
#include "krad_radio_client.h"

/* This should be called kr_radio_client
 * but we need to rename the client on the server
 * side and in all the clients then..
 */
struct kr_client {
  kr_app_client *krad_app_client;
  char *name;
  int autosync;
  int subscriber;
  int last_delivery_was_final;
  kr_crate *re_crate;
  kr_ebml2_t *ebml2;
  kr_io2_t *io;
  kr_ebml2_t *ebml_in;
  kr_io2_t *io_in;
  /* The below should be replaced with a
   * proper kr_****_info cache
   */
  uint32_t period_size;
  uint32_t sample_rate;
  uint32_t width;
  uint32_t height;
  uint32_t fps_num;
  uint32_t fps_den;
};

struct kr_shm_St {
  int fd;
  char *buffer;
  uint64_t size;
};

typedef int (*rep_to_string_t)(unsigned char *, uint64_t, char **);
int kr_send_fd(kr_client *client, int fd);

//#endif
