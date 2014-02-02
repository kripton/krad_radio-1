#include "krad_radio_client.h"
#include "krad_transponder_common.h"

int kr_xpdr_mkpath(kr_client *client, char *name, kr_transponder_path_info *info) {
  int ret;
  kr_crate2 crate;
  if (client == NULL) return -1;
  if (name == NULL) return -2;
  if (info == NULL) return -3;
  //if (kr_transponder_path_info_valid(info) < 0) {
  // return -1;
  //}
  snprintf(crate.address, sizeof(crate.address), "/transponder/%s", name);
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_TRANSPONDER_PATH_INFO;
  memcpy(&crate.payload.transponder_path_info, info,
   sizeof(kr_transponder_path_info));
  ret = kr_crate_send(client, &crate);
  return ret;
}
