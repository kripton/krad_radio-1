#include <sys/types.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <string.h>
#include <inttypes.h>

#ifndef KRAD_BASE64_H
#define KRAD_BASE64_H
void kr_base64_encode(char *dest, char *src, int maxlen);
int32_t kr_base64(uint8_t *dest, uint8_t *src, int len, int maxlen);
#endif
