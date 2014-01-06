#include "krad_mixer_server.h"

void kr_mixer_server_info_cb(kr_mixer_info_cb_arg *arg) {
  printk("got a mixer info cb");
}

static void bus_info_cb(kr_mixer_path_info_cb_arg *arg) {
  printk("got a mixer bus info cb");
}

static void masterbus_setup(kr_mixer *mixer) {
  kr_mixer_path *masterbus;
  kr_mixer_path_setup mbs;
  memset(&mbs, 0, sizeof(kr_mixer_path_setup));
  mbs.info.type = KR_MXR_BUS;
  strncpy(mbs.info.name, "Master", sizeof(mbs.info.name));
  mbs.info.channels = 2;
  mbs.info.volume[0] = KR_MXR_DEF_MBUS_LVL;
  mbs.info.volume[1] = KR_MXR_DEF_MBUS_LVL;
  mbs.info_cb = bus_info_cb;
  mbs.user = mixer;
  masterbus = kr_mixer_mkpath(mixer, &mbs);
  if (masterbus == NULL) {
    printke("radio couldn't create mixer master bus");
  }
}
