#include "krad_radio_client_internal.h"
#include "krad_radio_client.h"

#include "krad_radio_common.h"
#include "krad_compositor_common.h"
#include "krad_transponder_common.h"
#include "krad_mixer_common.h"

#include "krad_compositor_client.h"
#include "krad_transponder_client.h"
#include "krad_mixer_client.h"

static void kr_crate_payload_ebml_reset(kr_crate *crate);
static void kr_crate_destroy(kr_crate **crate);

int kr_client_sync(kr_client *client) {
  int32_t ret;
  ret = kr_io2_output(client->io);
  if (ret != 0) {
    printke("oh i am so sad I could not do it all!");
  }
  kr_ebml2_set_buffer(client->ebml2, client->io->buf, client->io->space);
  return 0;
}

int kr_poll_out(kr_client *client, uint32_t timeout_ms) {
  int ret;
  struct pollfd pollfds[1];
  pollfds[0].fd = kr_app_client_get_fd(client->krad_app_client);
  pollfds[0].events = POLLOUT;
  ret = poll(pollfds, 1, timeout_ms);
  if (pollfds[0].revents & POLLHUP) {
    ret = -1;
  }
  return ret;
}

int kr_client_want_out (kr_client *client) {
  return kr_io2_want_out(client->io);
}

int kr_crate_send(kr_client *client, kr_crate2 *crate) {
  if (client == NULL) return -1;
  if (crate == NULL) return -2;
  //if (kr_crate2_valid(info) < 0) {
  // return -1;
  //}
  uint8_t *ebml_crate;
  kr_ebml2_start_element(client->ebml2, KR_EID_CRATE, &ebml_crate);
  kr_crate2_to_ebml(client->ebml2, crate);
  kr_ebml2_finish_element(client->ebml2, ebml_crate);
  kr_client_push(client);
  return 0;
}

int kr_get(kr_client *client, char *address) {
  int ret;
  kr_crate2 crate;
  if (client == NULL) return -1;
  if (address == NULL) return -2;
  memset(&crate, 0, sizeof(kr_crate2));
  crate.method = KR_GET;
  strncpy(crate.address, address, sizeof(crate.address));
  /* need a no payload type or something */
  ret = kr_crate_send(client, &crate);
  return ret;
}

int kr_client_push(kr_client *client) {
  kr_io2_advance(client->io, client->ebml2->pos);
  if (kr_client_want_out(client)) {
    if ((client->autosync == 1) && (kr_poll_out(client, 0) > 0)) {
      kr_client_sync(client);
    }
  }
  return 0;
}

kr_client *kr_client_create(char *client_name) {
  kr_client *client;
  int len;
  if (client_name == NULL) {
    return NULL;
  }
  len = strlen(client_name);
  if ((len == 0) || (len > 255)) {
    return NULL;
  }
  client = calloc (1, sizeof(kr_client));
  client->name = strdup(client_name);
  client->autosync = 1;
  return client;
}

int kr_connect_remote(kr_client *client, char *host, int port, int timeout_ms) {
  char url[532];
  int len;
  if ((client == NULL) || (host == NULL) ||
      ((port < 1) || (port > 65535))) {
    return 0;
  }
  len = strlen (host);
  if ((len == 0) || (len > 512)) {
    return 0;
  }
  snprintf(url, sizeof(url), "%s:%d", host, port);
  return kr_connect_full(client, url, timeout_ms);
}

int kr_check_connection(kr_client *client) {
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  kr_ebml_pack_header (client->ebml2, KRAD_APP_CLIENT_DOCTYPE, KRAD_APP_DOCTYPE_VERSION, KRAD_APP_DOCTYPE_READ_VERSION);
  kr_client_push (client);
  kr_poll (client, 25);
  kr_io2_read (client->io_in);
  kr_ebml2_set_buffer ( client->ebml_in, client->io_in->rd_buf, client->io_in->len );
  ret = kr_ebml2_unpack_header (client->ebml_in, doctype, sizeof(doctype),
                                &version, &read_version);
  if (ret > 0) {
    if ((version == KRAD_APP_DOCTYPE_VERSION) && (read_version == KRAD_APP_DOCTYPE_READ_VERSION) &&
        (strlen(KRAD_APP_SERVER_DOCTYPE) == strlen(doctype)) &&
        (strncmp(doctype, KRAD_APP_SERVER_DOCTYPE, strlen(KRAD_APP_SERVER_DOCTYPE)) == 0)) {

        kr_io2_pulled (client->io_in, client->io_in->len);
        kr_ebml2_set_buffer ( client->ebml_in, client->io_in->rd_buf, client->io_in->len );
        return ret;

    } else {
      printf ("frak %u %u %s \n", version, read_version, doctype);
    }
  } else {
    printf ("frakr %d\n", ret);
  }
  return 0;
}

int kr_connect(kr_client *client, char *sysname) {
  return kr_connect_full (client, sysname, 3000);
}

int kr_connect_full(kr_client *client, char *sysname, int timeout_ms) {
  int fd;
  if (client == NULL) {
    return 0;
  }
  if (kr_connected(client)) {
    kr_disconnect(client);
  }
  client->krad_app_client = kr_app_connect(sysname, timeout_ms);
  if (client->krad_app_client != NULL) {

    fd = kr_app_client_get_fd(client->krad_app_client);
    client->io = kr_io2_create ();
    client->ebml2 = kr_ebml2_create ();

    kr_io2_set_fd ( client->io, fd );
    kr_ebml2_set_buffer ( client->ebml2, client->io->buf, client->io->space );

    client->io_in = kr_io2_create ();
    client->ebml_in = kr_ebml2_create ();

    kr_io2_set_fd ( client->io_in, fd );
    kr_ebml2_set_buffer ( client->ebml_in, client->io_in->buf, client->io_in->space );

    if (kr_check_connection (client) > 0) {
      return 1;
    } else {
      kr_disconnect(client);
    }
  }

  return 0;
}

int kr_connected(kr_client *client) {
  if (client->krad_app_client != NULL) {
    return 1;
  }
  return 0;
}

int kr_disconnect(kr_client *client) {
  if (client != NULL) {
    if (kr_connected (client)) {
      kr_app_disconnect(client->krad_app_client);
      client->krad_app_client = NULL;
      if (client->io != NULL) {
        kr_io2_destroy (&client->io);
      }
      if (client->ebml2 != NULL) {
        kr_ebml2_destroy (&client->ebml2);
      }

      if (client->io_in != NULL) {
        kr_io2_destroy (&client->io_in);
      }
      if (client->ebml_in != NULL) {
        kr_ebml2_destroy (&client->ebml_in);
      }
      return 1;
    }
    return -2;
  }
  return -1;
}

int kr_client_destroy(kr_client **client) {
  if (*client != NULL) {
    if (kr_connected(*client)) {
      kr_disconnect(*client);
    }
    if ((*client)->re_crate != NULL) {
      kr_crate_destroy(&(*client)->re_crate);
    }
    if ((*client)->name != NULL) {
      free ((*client)->name);
      (*client)->name = NULL;
    }
    free(*client);
    *client = NULL;
    return 1;
  }
  return -1;
}

int kr_client_local(kr_client *client) {
  if (client != NULL) {
    if (kr_connected(client)) {
      return kr_app_client_local(client->krad_app_client);
    }
  }
  return -1;
}

int kr_client_get_fd(kr_client *client) {
  if (client != NULL) {
    if (kr_connected(client)) {
      return kr_app_client_get_fd(client->krad_app_client);
    }
  }
  return -1;
}

void kr_subscribe_all(kr_client *client) {
}

void kr_subscribe(kr_client *client, uint32_t broadcast_id) {
}

int kr_send_fd(kr_client *client, int fd) {
  return kr_app_client_send_fd(client->krad_app_client, fd);
}

void kr_crate_free_string(char **string) {
  free (*string);
}

char *kr_crate_alloc_string(int length) {
  return calloc (1, length + 16);
}

int kr_poll(kr_client *client, uint32_t timeout_ms) {
  int ret;
  struct pollfd pollfds[1];
  pollfds[0].fd = kr_app_client_get_fd(client->krad_app_client);
  if ((kr_client_want_out (client)) && (client->autosync == 1)) {
    pollfds[0].events = POLLIN | POLLOUT;
  } else {
    pollfds[0].events = POLLIN;
  }
  ret = poll (pollfds, 1, timeout_ms);
  if (pollfds[0].revents & POLLOUT) {
    kr_client_sync (client);
  }
  if (pollfds[0].revents & POLLHUP) {
    ret = -1;
  }
  return ret;
}

int kr_delivery_final(kr_client *client) {
  return client->last_delivery_was_final;
}

void kr_delivery_final_reset(kr_client *client) {
  client->last_delivery_was_final = 0;
}

void kr_crate_address(kr_crate *crate, kr_address_t **address) {
  *address = &crate->address;
}

static void kr_crate_payload_ebml_reset (kr_crate *crate) {
  kr_ebml2_set_buffer ( &crate->payload_ebml, crate->buffer, crate->size );
}

int kr_uncrate_rep(kr_crate *crate) {
  if (!(crate->notice == KR_GET)) {
    return 0;
  }
  if (crate->size == 0) {
    return 0;
  }
  kr_crate_payload_ebml_reset (crate);
  switch ( crate->address.path.unit ) {
    case KR_STATION:
      //kr_radio_crate_to_rep (crate);
      return 1;
    case KR_MIXER:
      //kr_mixer_crate_to_rep (crate);
      return 1;
    case KR_COMPOSITOR:
      //kr_compositor_crate_to_info(crate);
      return 1;
    case KR_TRANSPONDER:
      break;
  }
  return 0;
}

uint32_t kr_crate_size(kr_crate *crate) {
  return crate->size;
}

static void kr_crate_destroy (kr_crate **crate) {
  if (*crate != NULL) {
    if ((*crate)->buffer != NULL) {
      free((*crate)->buffer);
    }
    free((*crate));
    *crate = NULL;
  }
}

void kr_crate_reset(kr_crate *crate) {
  kr_client *client;
  unsigned char *buffer;
  client = NULL;
  buffer = NULL;
  if (crate != NULL) {
    client = crate->client;
    if (crate->buffer != NULL) {
      buffer = crate->buffer;
    }
    memset (crate, 0, sizeof(kr_crate));
    crate->client = client;
    if (buffer != NULL) {
      crate->buffer = buffer;
    }
  }
}

void kr_crate_recycle(kr_crate **crate) {
  if (*crate != NULL) {
    if ((*crate)->client->re_crate == NULL) {
      kr_crate_reset (*crate);
      (*crate)->client->re_crate = *crate;
      *crate = NULL;
    } else {
      kr_crate_destroy (crate);
    }
  }
}

kr_crate *kr_crate_create(kr_client *client) {
  kr_crate *crate;
  crate = calloc (1, sizeof(kr_crate));
  crate->client = client;
  return crate;
}

int kr_have_full_crate(kr_io2_t *in) {
  kr_ebml2_t ebml;
  uint32_t element;
  uint64_t size;
  int ret;
  if (!(kr_io2_has_in (in))) {
    return 0;
  }
  kr_ebml2_set_buffer( &ebml, in->rd_buf, in->len );
  ret = kr_ebml2_unpack_id(&ebml, &element, &size);
  if (ret < 0) {
    printf ("full_command EBML ID Not found");
    return 0;
  }
  size += ebml.pos;
  if (in->len < size) {
    //printf ("full_command Not Enough bytes.. have %zu need %zu\n", in->len, size);
    return 0;
  } else {
    //printf ("Got command have %zu need %zu\n", in->len, size);
  }
  return size;
}

int kr_delivery_get(kr_client *client, kr_crate **crate) {
  kr_crate *response;
  uint32_t ebml_id;
  uint64_t ebml_data_size;
  int have_crate;
  have_crate = 0;
  ebml_id = 0;
  ebml_data_size = 0;
  have_crate = kr_have_full_crate(client->io_in);
  if (have_crate) {
    kr_ebml2_set_buffer(client->ebml_in, client->io_in->rd_buf, client->io_in->len);
    if (client->re_crate != NULL) {
      response = client->re_crate;
      client->re_crate = NULL;
    } else {
      response = kr_crate_create(client);
    }
    *crate = response;
    response->inside.actual = &response->rep.actual;
    //FIXME DOUBLE FIXME
    //krad_read_address_from_ebml(client->ebml_in, &response->address);
    kr_ebml2_unpack_element_uint32(client->ebml_in, NULL, &response->notice);
    response->addr = &response->address;
    if (!client->subscriber) {
      if (response->notice == KR_EID_TERMINATOR) {
        client->last_delivery_was_final = 1;
      } else {
        client->last_delivery_was_final = 0;
      }
    }
    //kr_address_debug_print (&response->address);
    //kr_message_notice_debug_print (response->notice);
    //if (krad_message_notice_has_payload (response->notice)) {
    //FIXME
    if (response->notice != KR_EID_TERMINATOR) {
      kr_ebml2_unpack_id(client->ebml_in, &ebml_id, &ebml_data_size);
      if (ebml_data_size > 0) {
        //printf ("KR Client Response payload size: %"PRIu64"\n", ebml_data_size);
        response->size = ebml_data_size;
        if (response->buffer == NULL) {
          response->buffer = malloc(2048);
        }
        kr_ebml2_unpack_data(client->ebml_in, response->buffer, ebml_data_size);
      }
    }

/*    if (kr_uncrate_int (response, &response->integer)) {
      response->has_int = 1;
    }
    if (kr_uncrate_float (response, &response->real)) {
      response->has_float = 1;
    }
*/
    kr_io2_pulled(client->io_in, have_crate);
  }
  return have_crate;
}

int kr_crate_has_float(kr_crate *crate) {
  return crate->has_float;
}

int kr_crate_has_int(kr_crate *crate) {
  return crate->has_int;
}

void kr_client_crate_wait(kr_client *client, kr_crate **crate) {
  kr_poll(client, 250);
  kr_delivery_get(client, crate);
}

void kr_delivery_recv(kr_client *client) {
  kr_io2_read (client->io_in);
}

void kr_delivery_accept_and_report(kr_client *client) {
  kr_crate *crate;
  char *string;
  int wait_time_ms;
  int length;
  int got_all_delivery;
  got_all_delivery = 0;
  string = NULL;
  crate = NULL;
  wait_time_ms = 250;
  for (;;) {
    kr_delivery_get(client, &crate);
    if (crate == NULL) {
      if (kr_poll(client, wait_time_ms)) {
        kr_delivery_recv(client);
      } else {
        break;
      }
    } else {
      //length = kr_uncrate_string(crate, &string);
      length = 0;
      if (length > 0) {
        printf ("%s\n", string);
        kr_string_recycle (&string);
      }
      kr_crate_recycle (&crate);
      if (kr_delivery_final (client)) {
        kr_delivery_final_reset (client);
        got_all_delivery = 1;
        break;
      }
    }
  }
  if (got_all_delivery == 0) {
    printf ("No response after waiting %dms\n", wait_time_ms);
  }
}

int kr_delivery_get_until_final(kr_client *client, kr_crate **crate, uint32_t timeout_ms) {
  kr_crate *lcrate;
  lcrate = NULL;
  if (client == NULL) {
    failfast("kr_delivery_get_until_final called with NULL client pointer");
  }
  if (crate == NULL) {
    failfast("kr_delivery_get_until_final called with NULL crate pointer");
  }
  if ((crate != NULL) && (*crate != NULL)) {
    kr_crate_recycle(crate);
  }
  for (;;) {
    kr_delivery_get(client, &lcrate);
    if (lcrate == NULL) {
      if (kr_poll(client, timeout_ms)) {
        kr_delivery_recv(client);
      } else {
        break;
      }
    } else {
      if (kr_delivery_final(client)) {
        kr_crate_recycle(&lcrate);
        kr_delivery_final_reset(client);
        return 0;
      }
      *crate = lcrate;
      return 1;
    }
  }
  printf("No response after waiting %dms\n", timeout_ms);
  return 0;
}
