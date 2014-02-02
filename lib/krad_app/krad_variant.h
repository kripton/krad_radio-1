#ifndef KRAD_VARIANT_H
#define KRAD_VARIANT_H

typedef struct kr_variant kr_variant;
typedef struct kr_variant kr_var;

typedef enum {
  KR_FLOAT = 1,
  KR_INTEGER,
} kr_value_type;

typedef union {
  int integer;
  float real;
  char str[8];
} kr_value;

struct kr_variant {
  kr_value value;
  kr_value_type type;
};

#endif
