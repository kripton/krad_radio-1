#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/mman.h>

#include "krad_radio_version.h"
#include "krad_system.h"

#include "krad_ebml.h"
#include "krad_io.h"
#include "krad_app_common.h"

#ifndef KRAD_APP_CLIENT_H
#define KRAD_APP_CLIENT_H

#define KRAD_APP_CLIENT_DOCTYPE "kr_app_client"
#define KRAD_APP_SERVER_DOCTYPE "kr_app_server"
#define KRAD_APP_DOCTYPE_VERSION KR_VERSION
#define KRAD_APP_DOCTYPE_READ_VERSION KR_VERSION
#define EBML_ID_KRAD_APP_CMD 0x4444

typedef struct kr_app_client kr_app_client;

int kr_app_client_local(kr_app_client *client);
int kr_app_client_get_fd(kr_app_client *client);
kr_app_client *kr_app_connect(char *sysname, int timeout_ms);
void kr_app_disconnect(kr_app_client *client);
int kr_app_client_send_fd(kr_app_client *client, int fd);

#endif
