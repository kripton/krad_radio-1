#include <poll.h>

#include "krad_radio_common.h"
#include "krad_compositor_common.h"
#include "krad_transponder_common.h"
#include "krad_mixer_common.h"
#include "krad_app_client.h"
#include "krad_radio_client.h"
#include "krad_compositor_client.h"
#include "krad_transponder_client.h"
#include "krad_mixer_client.h"

struct kr_client {
  kr_app_client *krad_app_client;
  char *name;
  int autosync;
  int subscriber;
  int last_delivery_was_final;
  kr_ebml2_t *ebml2;
  kr_io2_t *io;
  kr_ebml2_t *ebml_in;
  kr_io2_t *io_in;
};

int kr_send_fd(kr_client *client, int fd);

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

int kr_delete(kr_client *client, char *address) {
  int ret;
  kr_crate2 crate;
  if (client == NULL) return -1;
  if (address == NULL) return -2;
  memset(&crate, 0, sizeof(kr_crate2));
  crate.method = KR_DELETE;
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
  client = kr_allocz (1, sizeof(kr_client));
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

void kr_delivery_recv(kr_client *client) {
  kr_io2_read(client->io_in);
}

int kr_streamer45(kr_client *client) {
  kr_ebml2_t ebml;
  kr_crate2 crate;
  uint32_t element;
  uint64_t size;
  int ret;
  if (!(kr_io2_has_in(client->io_in))) {
    return 0;
  }
  memset(&crate, 0, sizeof(kr_crate2));
  kr_ebml2_set_buffer(&ebml, client->io_in->rd_buf, client->io_in->len);
  ret = kr_ebml2_unpack_id(&ebml, &element, &size);
  if (ret < 0) {
    printke("full crate EBML ID Not found");
    return 0;
  }
  size += ebml.pos;
  if (client->io_in->len < size) {
    printke("Crate not full...");
    return 0;
  }
  if (element == KR_EID_CRATE) {
    ret = kr_crate2_fr_ebml(&ebml, &crate);
    if (ret == 0) {
      char string[8192];
      ret = kr_crate2_to_text(string, &crate, sizeof(string));
      if (ret > 0) {
        printk("Got a %"PRIu64" Byte Crate: \n%s\n", size, string);
      }
      kr_io2_pulled(client->io_in, ebml.pos);
      return 1;
    }
  }
  return 0;
}
