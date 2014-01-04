#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include "kr_client.h"

typedef struct kr_osc kr_osc;

void kr_osc_stop_listening(kr_osc *osc);
int kr_osc_listen(kr_osc *osc, int port);
void kr_osc_destroy(kr_osc *osc);
kr_osc *kr_osc_create(char *sysname);
