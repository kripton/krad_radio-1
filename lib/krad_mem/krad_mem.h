#ifndef KRAD_MEM_H
#define KRAD_MEM_H

#include <stddef.h>

void *kr_allocz(size_t nelem, size_t elsize);
void *kr_alloc(size_t size);

#endif
