#include "krad_pool.h"

/* FIXME TEMP */
#include "../xpdr/krad_av.c"

#define KR_POOL_MAX 64

struct kr_pool {
  size_t slice_size;
  uint64_t use;
  /*uint8_t type[KR_POOL_MAX];
  uint8_t state[KR_POOL_MAX];*/
  void *overlay;
  void *data;
  void *map;
  size_t info_size;
  size_t total_size;
  size_t overlay_sz;
  size_t overlay_actual_sz;
  uint16_t slices;
  uint16_t active;
  uint8_t shared;
  int fd;
  uint8_t ref[KR_POOL_MAX];
};

int kr_pool_fd(kr_pool *pool) {
  if (pool == NULL) return -1;
  if (pool->shared != 1) return -1;
  return pool->fd;
}

int kr_pool_size(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->total_size;
}

int kr_pool_offsetof(kr_pool *pool, void *slice) {
  if (pool == NULL) return -1;
  if (slice == NULL) return -1;
  /* FIXME check that slice is in pool */
  return slice - pool->map;
}

int kr_pool_avail(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->slices - pool->active;
}

int kr_pool_active(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->active;
}

int kr_pool_slices(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->slices;
}

void *kr_pool_iterate_active(kr_pool *pool, int *count) {

  uint64_t mask;

  if ((pool == NULL) || (count == NULL)) return NULL;
  if ((*count < 0) || (*count >= pool->slices)) return NULL;

  mask = 1;
  mask = mask << ((*count));
  while (*count < pool->slices) {
    if ((pool->use & mask) != 0) {
      return pool->data + (pool->slice_size * (*count)++);
    } else {
      (*count)++;
      mask = mask << 1;
    }
  }
  (*count) = 0;
  return NULL;
}

void *kr_pool_slice_num(kr_pool *pool, int num) {

  uint64_t mask;

  if (pool == NULL) return NULL;
  if ((num < 0) || (num >= pool->slices)) return NULL;

  mask = 1;
  mask = mask << (num);
  if ((pool->use & mask) != 0) {
    return pool->data + (pool->slice_size * num);
  }
  return NULL;
}

/*
void *kr_pool_iterate_state(kr_pool *pool, int *count) {

  return NULL;
}

void *kr_pool_iterate_type(kr_pool *pool, int *count) {

  return NULL;
}

void *kr_pool_iterate_type_state(kr_pool *pool, int *count) {
  return NULL;
}
*/

void *kr_pool_overlay_get(kr_pool *pool) {
  if (pool == NULL) return NULL;
  if (pool->overlay == NULL) return NULL;
  if (pool->overlay_sz == 0) return NULL;
  return pool->overlay;
}

int kr_pool_overlay_set(kr_pool *pool, void *overlay) {
  if (pool == NULL) return -1;
  if (pool->overlay_sz == 0) return -2;
  memcpy(pool->overlay, overlay, pool->overlay_sz);
  return 0;
}

int kr_pool_overlay_get_copy(kr_pool *pool, void *overlay) {
  if ((pool == NULL) || (overlay == NULL)) return -2;
  if (pool->overlay == NULL) return -3;
  memcpy(overlay, pool->overlay, pool->overlay_sz);
  return pool->overlay_sz;
}

int kr_pool_recycle(kr_pool *pool, void *slice) {
  uint64_t mask;
  size_t num;
  if ((pool == NULL) || (slice == NULL)) return -2;
  num = (slice - pool->data) / pool->slice_size;
  mask = 1 + num;
  if (((pool->use & mask) != 0)
   && (slice == (pool->data + (pool->slice_size * num)))) {
      pool->use = pool->use ^ mask;
      pool->active--;
      return 0;
  }
  return -1;
}

int kr_pool_slice_ref(kr_pool *pool, void *slice) {

  int i;
  uint64_t mask;

  if ((pool == NULL) || (slice == NULL)) return -2;

  mask = 1;
  for (i = 0; i < pool->slices; i++) {
    if (((pool->use & mask) != 0)
        && (slice == (pool->data + (pool->slice_size * i)))) {
      pool->ref[i]++;
      return pool->ref[i];
    }
    mask = mask << 1;
  }
  return -1;
}

void *kr_pool_slice(kr_pool *pool) {

  int i;
  uint64_t mask;

  if (pool == NULL) return NULL;

  mask = 1;
  for (i = 0; i < pool->slices; i++) {
    if ((pool->use & mask) == 0) {
      pool->use = pool->use | mask;
      pool->active++;
      return pool->data + (pool->slice_size * i);
    }
    mask = mask << 1;
  }
  return NULL;
}

void kr_pool_debug(kr_pool *pool) {
  if (pool == NULL) return;
  printk("\npool info");
  printk("pool slices: %d", kr_pool_slices(pool));
  printk("pool active: %d", kr_pool_active(pool));
  printk("pool avail: %d", kr_pool_avail(pool));
  printk("pool use: %"PRIu64"", pool->use);
  printk("pool info size: %zu", sizeof(kr_pool));
  printk("pool info act size: %zu", pool->info_size);
  printk("pool overlay size: %zu", pool->overlay_sz);
  printk("pool overlay act size: %zu", pool->overlay_actual_sz);
  printk("pool slice size: %zu", pool->slice_size);
  printk("pool slices size: %zu", pool->slice_size * pool->slices);
  printk("pool total size: %zu\n", pool->total_size);
}

int kr_pool_destroy(kr_pool *pool) {
  int ret;
  if (pool == NULL) return -1;
  if (pool->shared == 1) {
    close(pool->fd);
  }
  ret = munmap(pool->map, pool->total_size);
  return ret;
}

kr_pool *kr_pool_create(kr_pool_setup *setup) {
  char filename[] = "/tmp/test-shm-XXXXXX";
  int fd;
  int flags;
  kr_pool pool;
  if (setup == NULL) return NULL;
  if (setup->slices == 0) return NULL;
  if (setup->slices > KR_POOL_MAX) return NULL;
  if (setup->size == 0) return NULL;
  memset(&pool, 0, sizeof(kr_pool));
  pool.overlay_sz = setup->overlay_sz;
  pool.overlay_actual_sz = pool.overlay_sz;
  pool.info_size = sizeof(kr_pool);
  if (pool.info_size % KR_CACHELINE) {
    pool.info_size += KR_CACHELINE - (pool.info_size % KR_CACHELINE);
  }
  if (pool.overlay_sz % KR_CACHELINE) {
    pool.overlay_actual_sz += KR_CACHELINE - (pool.overlay_sz % KR_CACHELINE);
  }
  pool.slices = setup->slices;
  pool.slice_size = setup->size;
  if (pool.slice_size % KR_CACHELINE) {
    pool.slice_size += KR_CACHELINE - (pool.slice_size % KR_CACHELINE);
  }
  pool.total_size = (pool.slices * pool.slice_size) + pool.info_size;
  pool.total_size += pool.overlay_actual_sz;
  if (pool.total_size % KR_PAGESIZE) {
    pool.total_size += KR_PAGESIZE - (pool.total_size % KR_PAGESIZE);
  }
  if (setup->shared != 0) {
    pool.shared = 1;
    flags = MAP_SHARED;
  } else {
    flags = MAP_PRIVATE;
    pool.shared = 0;
  }
  fd = mkstemp(filename);
  if (fd < 0) {
    printke("open %s failed: %m\n", filename);
    return NULL;
  }
  if (ftruncate(fd, pool.total_size) < 0) {
    printke("ftruncate failed: %m\n");
    close(fd);
    return NULL;
  }
  pool.map = mmap(NULL, pool.total_size, PROT_READ | PROT_WRITE, flags, fd, 0);
  unlink(filename);
  if (pool.map == MAP_FAILED) {
    printke("mmap failed\n");
    close(fd);
    return NULL;
  }
  if (pool.shared == 1) {
    pool.fd = fd;
  } else {
    close(fd);
  }
  pool.data = pool.map + (pool.info_size + pool.overlay_actual_sz);
  if (pool.overlay_sz != 0) {
    pool.overlay = pool.map + pool.info_size;
    if (setup->overlay != NULL) {
      kr_pool_overlay_set(&pool, setup->overlay);
    }
  }
  memcpy(pool.map, &pool, sizeof(kr_pool));
  return (kr_pool *)pool.map;
}
