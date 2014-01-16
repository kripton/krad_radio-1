#include <stdlib.h>
#include "krad_mem.h"
#include "krad_system.h"

void *kr_allocz(size_t nelem, size_t elsize) {
  //printk("Mem: allocZ %zu bytes", nelem * elsize);
  return calloc(nelem, elsize);
}

void *kr_alloc(size_t size) {
  //printk("Mem: alloc %zu bytes", size);
  return malloc(size);
}
