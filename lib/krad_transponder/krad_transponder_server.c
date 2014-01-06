#include "krad_transponder_server.h"

/*
static void xpdr_event_cb(kr_xpdr_event_cb_arg *arg) {
  kr_xpdr *xpdr;
//  xpdr = (kr_xpdr *)arg->user;
  printk("yay xpdr event!");
}
*/

static void path_event(kr_xpdr_path_event_cb_arg *arg) {
  printk("yay xpdr path event!");
}

void test_function(kr_transponder *xpdr, kr_transponder_path_info *info) {
  kr_xpdr_path_setup setup;
  char text[2048];
  printk("path name %s",info->name);
  if (kr_transponder_path_info_to_text(text,info,2048) > 0) {
    printk("\n%s\n",text);
  }
  memcpy(&setup.info,info,sizeof(kr_transponder_path_info));
  setup.ev_cb = path_event;
  setup.user = xpdr;
  kr_transponder_mkpath(xpdr,&setup);
}
