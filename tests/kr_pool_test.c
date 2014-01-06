#include "krad_pool.h"

static int slice_check(void *slice) {
  printf("slice addres: %p\n", slice);
  if ((uint64_t)slice % KR_CACHELINE) {
    fprintf(stderr, "Unaligned slice!\n");
    exit(1);
  }
  return 0;
}

static int pool_test(int sz, int len) {
  int i;
  int ret;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  void *slice[128];
  void *aslice;

  aslice = NULL;
  memset(&slice, 0, sizeof(slice));
  memset(&pool_setup, 0, sizeof(kr_pool_setup));

  pool_setup.shared = 1;
  pool_setup.size = sz;
  pool_setup.slices = len;

  pool = kr_pool_create(&pool_setup);

  if (pool == NULL) {
    fprintf(stderr, "failed to make pool\n");
    return 1;
  }

  kr_pool_debug(pool);

  slice[0] = kr_pool_slice(pool);
  if (slice[0] == NULL) {
    fprintf(stderr, "failed to get slice\n");
    return 1;
  }
  slice_check(slice[0]);
  kr_pool_debug(pool);
  slice[1] = kr_pool_slice(pool);
  if (slice[1] == NULL) {
    fprintf(stderr, "failed to get slice\n");
    return 1;
  }
  slice_check(slice[1]);
  kr_pool_debug(pool);

  i = 0;
  while ((aslice = kr_pool_iterate_active(pool, &i))) {
    printf("slice %d is active!\n", i - 1);
  }

  ret = kr_pool_recycle(pool, slice[1]);
  if (ret != 0) {
    fprintf(stderr, "failed to return slice %d\n", ret);
    return 1;
  }
  kr_pool_debug(pool);

  i = 0;
  while ((aslice = kr_pool_iterate_active(pool, &i))) {
    printf("slice %d is active!\n", i - 1);
  }

  ret = kr_pool_recycle(pool, slice[0]);
  if (ret != 0) {
    fprintf(stderr, "failed to return slice %d\n", ret);
    return 1;
  }

  kr_pool_debug(pool);

  kr_pool_destroy(pool);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  int slices;
  int slice_sz;
  int i;
  ret = 0;
  srand(time(NULL));
  for (i = 0; i < 4; i++) {
    slices = rand() % 64;
    slice_sz = rand() % (5 * 1000 * 1000);
    ret = pool_test(slice_sz, slices);
    if (ret) break;
  }
  return ret;
}
