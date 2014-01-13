#ifndef KRAD_SYSTEM_H
#define KRAD_SYSTEM_H

#include <inttypes.h>
#include <pthread.h>

#include <math.h>
#include <sys/time.h>

#include "krad_mem.h"

#define KR_LINUX

#define sbyte signed char
#define ubyte unsigned char

#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

typedef struct kr_system kr_system;
typedef struct krad_system_cpu_monitor_St krad_system_cpu_monitor_t;
typedef struct krad_control_St krad_control_t;

#define KRAD_SYSNAME_MIN 4
#define KRAD_SYSNAME_MAX 32
#define KRAD_SYSNAME_SZ 64
#define KRAD_BUFLEN_CPUSTAT 128
#define KRAD_CPU_MONITOR_INTERVAL 4000

struct krad_control_St {
  int sockets[2];
};

int krad_controller_get_controller_fd(krad_control_t *krad_control);
int krad_control_init(krad_control_t *krad_control);
int krad_controller_get_client_fd(krad_control_t *krad_control);
void krad_controller_destroy(krad_control_t *krad_control, pthread_t *thread);
int krad_controller_shutdown(krad_control_t *krad_control, pthread_t *thread,
 int timeout);
int krad_controller_client_wait(krad_control_t *krad_control, int timeout);
int krad_controller_client_close(krad_control_t *krad_control);

int kr_sys_port_valid(int port);
uint64_t krad_unixtime();
void failfast(char* format, ...);
void printke(char* format, ...);
void printk(char* format, ...);
#define printkd printk

void kr_systm_get_thread_name(char *name);
void krad_system_set_thread_name(char *name);
int krad_system_set_socket_nonblocking(int sd);
int krad_system_set_socket_blocking(int sd);
int dir_exists(char *dir);
int kr_sysname_valid(char *sysname);
int krad_valid_host_and_port(char *string);
void krad_get_host_and_port(char *string, char *host, int *port);
int krad_system_is_adapter(char *adapter);
#endif
