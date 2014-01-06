#include "krad_radio_server.h"

void krad_radio_pack_shipment_terminator(kr_ebml2_t *ebml) {
/*  kr_address_t address;
  unsigned char *response;
  memset(&address, 0, sizeof (kr_address_t));
  address.path.unit = KR_STATION;
  address.path.subunit.station_subunit = KR_STATION_UNIT;
  krad_radio_address_to_ebml2(ebml, &response, &address);
  kr_ebml_pack_uint32(ebml, EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
   EBML_ID_KRAD_SHIPMENT_TERMINATOR);
  kr_ebml2_finish_element(ebml, response);
*/
}

void *kr_radio_client_create(void *ptr) {
  kr_radio *radio;
  kr_radio_client *client;
  radio = (kr_radio *)ptr;
  client = calloc(1, sizeof(kr_radio_client));
  client->radio = radio;
  return client;
}

void kr_radio_client_destroy(void *ptr) {
  kr_radio_client *client;
  client = (kr_radio_client *)ptr;
  free(client);
}

int validate_header(kr_io2_t *in) {
  kr_ebml2_t ebml;
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  kr_ebml2_set_buffer(&ebml, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_header(&ebml, doctype, 32, &version, &read_version);
  if (ret > 0) {
    if ((version == KRAD_APP_DOCTYPE_VERSION)
     && (read_version == KRAD_APP_DOCTYPE_READ_VERSION)
     && (strlen(KRAD_APP_CLIENT_DOCTYPE) == strlen(doctype))
     && (strncmp(doctype, KRAD_APP_CLIENT_DOCTYPE, strlen(KRAD_APP_CLIENT_DOCTYPE)) == 0)) {
        kr_io2_pulled(in, ret);
        return ret;
    } else {
      printke("validate header mismatch %u %u %s \n", version, read_version, doctype);
    }
  } else {
    printke("validate header err %d\n", ret);
  }
  return -1;
}

void pack_client_header(kr_io2_t *out) {
  kr_ebml2_t ebml;
  kr_ebml2_set_buffer(&ebml, out->buf, out->space);
  kr_ebml_pack_header(&ebml, KRAD_APP_SERVER_DOCTYPE, KRAD_APP_DOCTYPE_VERSION,
   KRAD_APP_DOCTYPE_READ_VERSION);
  kr_io2_advance(out, ebml.pos);
}

int validate_client(kr_io2_t *in, kr_io2_t *out, kr_radio_client *client) {
  int ret;
  ret = validate_header(in);
  if (ret > 0) {
    client->valid = 1;
    pack_client_header(out);
    return 1;
  } else {
    printke ("Invalid client!");
    return -1;
  }
}

uint32_t full_command(kr_io2_t *in) {
  kr_ebml2_t ebml;
  uint32_t element;
  uint64_t size;
  int ret;
  if (!(kr_io2_has_in (in))) {
    return 0;
  }
  kr_ebml2_set_buffer(&ebml, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_id(&ebml, &element, &size);
  if (ret < 0) {
    printke ("full_command EBML ID Not found");
    return 0;
  }
  size += ebml.pos;
  if (in->len < size) {
    printke ("full_command Not Enough bytes..");
    return 0;
  }
  return element;
}

int kr_radio_cmd(kr_app_server_request *request) {
/*
  kr_io2_t *in;
  kr_io2_t *out;
  kr_radio_client *client;
  in = request->in;
  out = request->out;
  kr_radio *radio;
  radio = (kr_radio *)request->ptr;
  kr_address_t address;
  unsigned char *response;
  unsigned char *payload;
  kr_ebml2_t ebml_in;
  kr_ebml2_t ebml_out;
  uint32_t command;
  uint32_t element;
  uint64_t size;
  int ret;
  unsigned char *tag;
  char *tag_name;
  char *tag_val;
  char string[512];
  char string1[512];
  char string2[512];
  char string3[512];
  uint32_t numbers[10];
  kr_remote_t remote;
  kr_app_server *app;
  int i;
  kr_tags *tags;
  tags = NULL;
  i = 0;
  string1[0] = '\0';
  string2[0] = '\0';
  string3[0] = '\0';
  app = request->app;
  if (!(kr_io2_has_in (in))) {
    return 0;
  }
  kr_ebml2_set_buffer(&ebml_in, in->rd_buf, in->len);
  ret = kr_ebml2_unpack_id(&ebml_in, &command, &size);
  if ((ret < 0) || (command != EBML_ID_KRAD_RADIO_CMD)) {
    printke ("krad_radio_client_command invalid EBML ID Not found");
    return 0;
  }
  ret = kr_ebml2_unpack_id(&ebml_in, &command, &size);
  if (ret < 0) {
    printke ("krad_radio_client_command EBML ID Not found");
    return 0;
  }
  kr_ebml2_set_buffer(&ebml_out, out->buf, out->space);
  switch (command) {
    case EBML_ID_KRAD_RADIO_CMD_GET_SYSTEM_INFO:
      break;
    default:
      printke ("Krad Radio Command Unknown! %u", command);
      return -1;
  }
  if ((ebml_out.pos > 0) && (!kr_app_server_current_client_is_subscriber(app))) {
    krad_radio_pack_shipment_terminator(&ebml_out);
  }
  kr_io2_pulled(in, ebml_in.pos);
  kr_io2_advance(out, ebml_out.pos);
*/
  return 0;
}

int kr_radio_server_handle(kr_app_server_request *request) {
  kr_io2_t *in;
  kr_io2_t *out;
  void *ptr;
  ptr = request->ptr;
  in = request->in;
  out = request->out;
  kr_radio_client *client;
  int ret;
  uint32_t command;
  if (in == NULL) {
    printke("krad_radio_client handler called with null input buffer");
    return -1;
  }
  if (out == NULL) {
    printke("krad_radio_client handler called with null output buffer");
    return -1;
  }
  if (ptr == NULL) {
    printke("krad_radio_client handler called with null client pointerr");
    return -1;
  }
  client = (kr_radio_client *)ptr;
  if (!client->valid) {
    ret = validate_client(in, out, client);
    if (ret != 1) {
      return -1;
    }
  }
  for (;;) {
    command = full_command(in);
    if (command == 0) {
      return 0;
    }
    //printk ("we have a full command la de da its %zu bytes", in->len);
/*
    switch (command) {
      case EBML_ID_KRAD_MIXER_CMD:
        //ret = kr_mixer_server_handle(request);
        break;
      case EBML_ID_KRAD_COMPOSITOR_CMD:
        //ret = kr_compositor_cmd(in, out, client);
        break;
      case EBML_ID_KRAD_TRANSPONDER_CMD:
        //ret = kr_transponder_cmd(in, out, client);
        break;
      case EBML_ID_KRAD_RADIO_CMD:
        //ret = kr_radio_cmd(request);
        break;
      default:
        ret = -1;
        break;
    }
*/
    if (ret != 0) {
      return -1;
    }
  }
  return 0;
}
