#include "krad_transponder_interface.h"

void krad_transponder_to_rep ( krad_transponder_t *krad_transponder, kr_transponder_t *transponder_rep ) {
  transponder_rep->receiver_port = krad_transponder->krad_receiver->port;
  transponder_rep->transmitter_port = krad_transponder->krad_transmitter->port;
}

void krad_transponder_rep_to_ebml ( kr_ebml2_t *ebml, kr_transponder_t *transponder_rep ) {
  kr_ebml2_pack_uint16 (ebml, EBML_ID_KRAD_RADIO_TCP_PORT, transponder_rep->receiver_port);
  kr_ebml2_pack_uint16 (ebml, EBML_ID_KRAD_RADIO_TCP_PORT, transponder_rep->transmitter_port);
}

void krad_transponder_to_ebml ( kr_ebml2_t *ebml, krad_transponder_t *krad_transponder ) {
  kr_transponder_t transponder_rep;
  memset (&transponder_rep, 0, sizeof (kr_transponder_t));
  krad_transponder_to_rep (krad_transponder, &transponder_rep);
  krad_transponder_rep_to_ebml (ebml, &transponder_rep);
}

int krad_transponder_command ( kr_io2_t *in, kr_io2_t *out, krad_radio_client_t *client ) {

  int i;
  uint32_t num;
  int devices;
  krad_radio_t *krad_radio;
  krad_transponder_t *krad_transponder;
  kr_address_t address;
  krad_ipc_server_t *kr_ipc;
  unsigned char *response;
  unsigned char *payload;
  kr_ebml2_t ebml_in;
  kr_ebml2_t ebml_out;
  uint32_t command;
  uint32_t element;
  uint64_t size;
  int ret;
  char string[512];
  char string2[512];  
  uint16_t port;
  krad_link_t *link;
  
  link = NULL;  
  string[0] = '\0';
  string2[0] = '\0';  
  port = 0;
  krad_radio = client->krad_radio;
  krad_transponder = krad_radio->krad_transponder;
  kr_ipc = krad_radio->remote.krad_ipc;

  if (!(kr_io2_has_in (in))) {
    return 0;
  }

  kr_ebml2_set_buffer ( &ebml_in, in->rd_buf, in->len );

  ret = kr_ebml2_unpack_id (&ebml_in, &command, &size);
  if ((ret < 0) || (command != EBML_ID_KRAD_TRANSPONDER_CMD)) {
    printke ("krad_mixer_command invalid EBML ID Not found");
    return 0;
  }

  ret = kr_ebml2_unpack_id (&ebml_in, &command, &size);
  if (ret < 0) {
    printke ("krad_mixer_command EBML ID Not found");
    return 0;
  }

  kr_ebml2_set_buffer ( &ebml_out, out->buf, out->space );

  switch ( command ) {
  
    case EBML_ID_KRAD_TRANSPONDER_CMD_GET_INFO:
      krad_radio_address_to_ebml2 (&ebml_out, &response, &krad_transponder->address);
      kr_ebml2_pack_uint32 ( &ebml_out,
                             EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                             EBML_ID_KRAD_UNIT_INFO);
      kr_ebml2_start_element (&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
      krad_transponder_to_ebml (&ebml_out, krad_transponder);
      kr_ebml2_finish_element (&ebml_out, payload);
      kr_ebml2_finish_element (&ebml_out, response);
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_SUBUNIT_LIST:
      krad_Xtransponder_list (krad_transponder->krad_Xtransponder);
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_SUBUNIT_DESTROY:
      kr_ebml2_unpack_element_uint32 (&ebml_in, &element, &num);
      if (krad_transponder->krad_link[num] != NULL) {
        krad_link_destroy (krad_transponder->krad_link[num]);
        krad_transponder->krad_link[num] = NULL;
        break;
      }
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_SUBUNIT_UPDATE:
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_SUBUNIT_CREATE:
    
      kr_ebml2_unpack_element_string (&ebml_in, &element, string, sizeof(string));
      kr_ebml2_unpack_element_string (&ebml_in, &element, string2, sizeof(string2));
    
      if (krad_link_string_to_operation_mode(string) == FAILURE) {
        break;
      }

      for (i = 0; i < KRAD_TRANSPONDER_MAX_LINKS; i++) {
        if (krad_transponder->krad_link[i] == NULL) {
          krad_transponder->krad_link[i] = krad_link_prepare (i);
          link = krad_transponder->krad_link[i];
          link->link_num = i;
          link->krad_radio = krad_transponder->krad_radio;
          link->krad_transponder = krad_transponder;
          link->operation_mode = krad_link_string_to_operation_mode (string);
          break;
        }
      }
      
      if (link == NULL) {
        break;
      }

      /*
      if (krad_link_string_to_operation_mode(string) == RECORD) {
        krad_transponder->krad_link[i]->operation_mode = RECORD;
        krad_transponder->krad_link[i]->av_mode = AUDIO_ONLY;
        krad_transponder->krad_link[i]->transport_mode = FILESYSTEM;
        sprintf (krad_transponder->krad_link[i]->output,
                 "%s/kr_test_%"PRIu64".ogg", getenv ("HOME"), krad_unixtime ());
      }
      */

      if (link->operation_mode == ENCODE) {
        if (string2[0] == 'v') {
          link->av_mode = VIDEO_ONLY;
          link->codec = THEORA;
        } else {
          link->av_mode = AUDIO_ONLY;
          if (string2[1] == 'o') {
            link->codec = OPUS;
          } else {
            if (string2[1] == 'f') {
             link->codec = FLAC;
            } else {
             link->codec = VORBIS;
            }
          }      
        }
      }

      if (link->operation_mode == DISPLAY) {
        link->av_mode = VIDEO_ONLY;
      }

      if (link->operation_mode == CAPTURE) {
        link->av_mode = VIDEO_ONLY;
        link->video_source = krad_link_string_to_video_source (string2);
        if (link->video_source == NOVIDEO) {
          free (link);
          krad_transponder->krad_link[i] = NULL;
          break;
        }
      }

      krad_link_start (link);

      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_LIST_ADAPTERS:

      address.path.unit = KR_TRANSPONDER;
      address.path.subunit.transponder_subunit = KR_ADAPTER;
#ifdef KR_LINUX
      devices = krad_v4l2_detect_devices ();

      for (i = 0; i < devices; i++) {
        if (krad_v4l2_get_device_filename (i, string) > 0) {
          address.id.number = i;
          krad_radio_address_to_ebml2 (&ebml_out, &response, &address);
          kr_ebml2_pack_uint32 ( &ebml_out,
                                 EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                                 EBML_ID_KRAD_SUBUNIT_INFO);
          kr_ebml2_start_element (&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
          kr_ebml2_pack_string (&ebml_out, EBML_ID_KRAD_TRANSPONDER_V4L2_DEVICE_FILENAME, string);
          kr_ebml2_finish_element (&ebml_out, payload);
          kr_ebml2_finish_element (&ebml_out, response);
        }
      }
#endif
      devices = krad_decklink_detect_devices();

      for (i = 0; i < devices; i++) {
        if (krad_decklink_get_device_name (i, string) > 0) {
          address.id.number = i;
          krad_radio_address_to_ebml2 (&ebml_out, &response, &address);
          kr_ebml2_pack_uint32 ( &ebml_out,
                                 EBML_ID_KRAD_RADIO_MESSAGE_TYPE,
                                 EBML_ID_KRAD_SUBUNIT_INFO);
          kr_ebml2_start_element (&ebml_out, EBML_ID_KRAD_RADIO_MESSAGE_PAYLOAD, &payload);
          kr_ebml2_pack_string (&ebml_out, EBML_ID_KRAD_TRANSPONDER_DECKLINK_DEVICE_NAME, string);
          kr_ebml2_finish_element (&ebml_out, payload);
          kr_ebml2_finish_element (&ebml_out, response);
        }
      }
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_LISTEN_ENABLE:
      kr_ebml2_unpack_element_uint16 (&ebml_in, &element, &port);
      krad_receiver_listen_on (krad_transponder->krad_receiver, port);
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_LISTEN_DISABLE:
      krad_receiver_stop_listening (krad_transponder->krad_receiver);
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_TRANSMITTER_ENABLE:
      kr_ebml2_unpack_element_uint16 (&ebml_in, &element, &port);
      krad_transmitter_listen_on (krad_transponder->krad_transmitter, port);
      break;
    case EBML_ID_KRAD_TRANSPONDER_CMD_TRANSMITTER_DISABLE:
      krad_transmitter_stop_listening (krad_transponder->krad_transmitter);
      break;      
    default:
      return -1;    
  }

  if ((ebml_out.pos > 0) && (!krad_ipc_server_current_client_is_subscriber (kr_ipc))) {
    krad_radio_pack_shipment_terminator (&ebml_out);
  }

  kr_io2_pulled (in, ebml_in.pos);
  kr_io2_advance (out, ebml_out.pos);
  
  return 0;
}
