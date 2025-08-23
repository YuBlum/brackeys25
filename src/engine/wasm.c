#include <stdlib.h>
#include <errno.h>
#include "engine/core.h"
#include "engine/os.h"

/* page size is always 64KB */
#define PAGE_SIZE (1<<16)

void *
os_mem_reserve(size_t amount) {
  void *mem = malloc(amount);
  if (!mem) {
    log_errorlf("%s: failed with error code: %d", __func__, errno);
    return 0;
  }
  return mem;
}

bool
os_mem_commit(void *buf, size_t amount) {
  (void)buf; (void)amount;
  /* lazily commition is not possible with WASM */
  return true;
}

bool
os_mem_free(void *buf, size_t amount) {
  (void)amount;
  free(buf);
  return true;
}

size_t
os_page_size(void) {
  return PAGE_SIZE;
}

size_t
os_reasonable_default_capacity(void) {
  return PAGE_SIZE; 
}
