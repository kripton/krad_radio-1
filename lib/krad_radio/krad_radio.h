#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <stddef.h>

#ifndef KRAD_RADIO
#define KRAD_RADIO 1
#endif

#ifndef KRAD_RADIO_H
#define KRAD_RADIO_H

/*
 * Here we are dealing with Krad Radio the application server
 * in specific, such as it is and as it does.
 */

typedef struct kr_radio kr_radio;
typedef struct kr_log kr_log;
typedef struct kr_remote_control kr_remote_control;

#include "krad_radio_version.h"
#include "krad_system.h"
#include "krad_timer.h"
#include "krad_ticker.h"
#include "krad_tags.h"
#include "krad_ebml.h"
#include "krad_app_server.h"
#include "krad_radio_ipc.h"
#include "krad_mixer.h"
#include "krad_compositor.h"
#include "krad_transponder.h"
#include "krad_interweb.h"

extern krad_system_t krad_system;

struct kr_log {
  kr_timer *startup_timer;
  int verbose;
  char filename[512];
};

struct kr_radio {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_transponder *transponder;
  kr_app_server *app;
  kr_web_server *web;
  kr_app_broadcaster *system_broadcaster;
  krad_tags *tags;
  kr_log log;
  char sysname[KRAD_SYSNAME_SZ];
};

int kr_radio_daemon(char *sysname);

#endif
