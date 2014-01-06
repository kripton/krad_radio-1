#ifndef KRAD_APP_COMMON_H
#define KRAD_APP_COMMON_H

#define KR_EID_CRATE 0xF1
#define KR_EID_ADDRESS 0xAE
#define KR_EID_METHOD 0xD7
#define KR_EID_PAYLOAD 0xB9
#define KR_EID_TERMINATOR 0xE2

typedef struct kr_crate2 kr_crate2;

typedef enum {
  KR_GET = 1,
  KR_PUT,
  KR_POST,
  KR_PATCH,
  KR_DELETE
} kr_app_method;

struct kr_crate2 {
  char address[64];
  kr_app_method method;
  void *payload;
};

#endif
