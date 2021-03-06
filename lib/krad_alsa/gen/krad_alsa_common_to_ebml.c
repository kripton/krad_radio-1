#include "krad_alsa_common_to_ebml.h"

int kr_alsa_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_alsa_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_alsa_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->card);
  res += kr_ebml_pack_string(ebml, 0xe1, actual->name);

  return res;
}
int kr_alsa_path_info_to_ebml(kr_ebml *ebml, void *st) {
  int res;
  struct kr_alsa_path_info *actual;
  res = 0;

  if ((ebml == NULL) || (st == NULL)) {
    return -1;
  }

  actual = (struct kr_alsa_path_info *)st;
  res += kr_ebml_pack_int32(ebml, 0xe1, actual->card_num);

  return res;
}
