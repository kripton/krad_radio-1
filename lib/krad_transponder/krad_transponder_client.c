#include "krad_radio_client.h"
#include "krad_radio_client_internal.h"
#include "krad_transponder_common.h"

int kr_xpdr_mkpath(kr_client *client, kr_transponder_path_info *info) {

  if (client == NULL) return -1;
  if (info == NULL) return -1;

  /* check that info is valid */
  /* serialize info */
  /* send info
   * (perhaps send and serialize is part of one magical crate send?) */

  if (kr_transponder_path_info_valid(info) < 0) {
   return -1;
  }
  unsigned char *command;
  unsigned char *test_command;
  unsigned char *payload;

  kr_ebml2_start_element(client->ebml2, EBML_ID_KRAD_TRANSPONDER_CMD, &command);
  kr_ebml2_start_element(client->ebml2, EBML_ID_KRAD_TRANSPONDER_CMD_SUBUNIT_CREATE, &test_command);

  kr_transponder_path_info_to_ebml(client->ebml2,info);
  kr_ebml2_finish_element(client->ebml2, test_command);
  kr_ebml2_finish_element(client->ebml2, command);

  kr_client_push(client);
  return 0;
}
