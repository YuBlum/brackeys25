#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include "engine/core.h"
#include "engine/os.h"

void *
os_mem_reserve(size_t amount) {
  void *mem = mmap(0, amount, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (mem == MAP_FAILED) {
    log_errorlf("%s: failed with error code: %d", __func__, errno);
    return 0;
  }
  return mem;
}

bool
os_mem_commit(void *buf, size_t amount) {
  (void)buf; (void)amount;
  /* already lazily commiting on linux */
  return true;
}

bool
os_mem_free(void *buf, size_t amount) {
  if (munmap(buf, amount) != 0) {
    log_errorlf("%s: failed with error code: %d", __func__, errno);
    return false;
  }
  return true;
}

size_t
os_page_size(void) {
  size_t ps = (size_t)sysconf(_SC_PAGESIZE);
  return ps > 0 ? ps : 4096;
}

size_t
os_reasonable_default_capacity(void) {
  return (1ull << 32); 
}
