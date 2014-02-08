#include "krad_radio_client.h"
#include "kr_xpdr_common.h"

int kr_xpdr_make(kr_client *client, char *name, kr_xpdr_path_info *info) {
  int ret;
  kr_crate crate;
  if (client == NULL) return -1;
  if (name == NULL) return -2;
  if (info == NULL) return -3;
  //if (kr_xpdr_path_info_valid(info) < 0) {
  // return -1;
  //}
  char string[8192];
  kr_xpdr_path_info_to_text(string, info, sizeof(string));
  printk("XPDR: mkpath-\n%s", string);

  snprintf(crate.address, sizeof(crate.address), "/xpdr/%s", name);
  crate.method = KR_PUT;
  crate.payload_type = PL_KR_XPDR_PATH_INFO;
  crate.payload.xpdr_path_info = *info;
  ret = kr_crate_send(client, &crate);
  return ret;
}
