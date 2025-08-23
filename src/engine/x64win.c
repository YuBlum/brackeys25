#include <windows.h>
#include "engine/core.h"
#include "engine/os.h"

void *
os_mem_reserve(size_t amount) {
  void *mem = VirtualAlloc(0, amount, MEM_RESERVE, PAGE_READWRITE);
#if DEV
  if (!mem) {
    DWORD err = GetLastError();
    log_errorlf("%s: failed with error code: %ld", __func__, err);
  }
#endif
  return mem; /* already returns '0' on error by default */
}

bool
os_mem_commit(void *buf, size_t amount) {
  void *mem = VirtualAlloc(buf, amount, MEM_COMMIT, PAGE_READWRITE);
  if (!mem) {
#if DEV
    DWORD err = GetLastError();
    log_errorlf("%s: failed with error code: %ld", __func__, err);
#endif
    return false;
  }
  return true;
}

bool
os_mem_free(void *buf, size_t amount) {
  (void)amount; /* not used on windows version */
  if (VirtualFree(buf, 0, MEM_RELEASE) == 0) {
#if DEV
    DWORD err = GetLastError();
    log_errorlf("%s: failed with error code: %ld", __func__, err);
#endif
    return false;
  }
  return true;
}

size_t
os_page_size(void) {
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return (size_t)si.dwPageSize;
}

size_t
os_reasonable_default_capacity(void) {
  return (1ull << 32); 
}
