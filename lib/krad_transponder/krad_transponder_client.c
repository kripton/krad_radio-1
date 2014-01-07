#include "krad_radio_client.h"
#include "krad_radio_client_internal.h"
#include "krad_transponder_common.h"

int kr_xpdr_mkpath(kr_client *client, kr_transponder_path_info *info) {
  if (client == NULL) return -1;
  if (info == NULL) return -1;
  if (kr_transponder_path_info_valid(info) < 0) {
   return -1;
  }
  uint8_t *ebml_crate;
  kr_crate2 crate;
  strcpy(crate.address, "/transponder/bongo");
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_TRANSPONDER_PATH_INFO;
  memcpy(&crate.payload.transponder_path_info, info,
   sizeof(kr_transponder_path_info));
  kr_ebml2_start_element(client->ebml2, KR_EID_CRATE, &ebml_crate);
  kr_crate2_to_ebml(client->ebml2, &crate);
  kr_ebml2_finish_element(client->ebml2, ebml_crate);
  kr_client_push(client);
  return 0;
}
