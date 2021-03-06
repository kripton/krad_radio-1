#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#ifndef KRAD_EBML2_H
#define KRAD_EBML2_H

#include "krad_system.h"
#include "krad_ebml_ids.h"

typedef struct kr_ebml kr_ebml2_t;
typedef struct kr_ebml kr_ebml;

struct kr_ebml {
  size_t pos;
  size_t len;
  uint8_t *buf;
  uint8_t *bufstart;
};

int kr_ebml2_destroy(kr_ebml **ebml);
kr_ebml *kr_ebml2_create();
int kr_ebml2_advance(kr_ebml *ebml, size_t bytes);
int kr_ebml2_set_buffer(kr_ebml *ebml, uint8_t *buffer, size_t len);
int kr_ebml_pack(kr_ebml *ebml, void *buffer, size_t len);

void kr_ebml2_revpack2(kr_ebml *ebml, void *buffer);
void kr_ebml2_revpack4(kr_ebml *ebml, void *buffer);

int kr_ebml_pack_header(kr_ebml *e, char *doctype, uint32_t ver, uint32_t read_version);
int kr_ebml_pack_element(kr_ebml *ebml, uint32_t element);
int kr_ebml2_start_element(kr_ebml *ebml, uint32_t element, uint8_t **pos);
int kr_ebml2_finish_element(kr_ebml *ebml, uint8_t *element_position);
int kr_ebml_pack_data(kr_ebml *e, uint32_t element, void *data, uint64_t len);
int kr_ebml_pack_string(kr_ebml *ebml, uint32_t element, char *string);
int kr_ebml_pack_float(kr_ebml *ebml, uint32_t element, float number);
int kr_ebml_pack_double(kr_ebml *ebml, uint32_t element, double number);
int kr_ebml_pack_int8(kr_ebml *ebml, uint32_t element, int8_t number);
int kr_ebml_pack_uint8(kr_ebml *ebml, uint32_t element, uint8_t number);
int kr_ebml_pack_int16(kr_ebml *ebml, uint32_t element, int16_t number);
int kr_ebml_pack_uint16(kr_ebml *ebml, uint32_t element, uint16_t number);
int kr_ebml_pack_int32(kr_ebml *ebml, uint32_t element, int32_t number);
int kr_ebml_pack_uint32(kr_ebml *ebml, uint32_t element, uint32_t number);
int kr_ebml_pack_int64(kr_ebml *kr_ebml, uint32_t element, int64_t number);
int kr_ebml_pack_uint64(kr_ebml *kr_ebml, uint32_t element, uint64_t number);

#endif
