#include <string.h>
#include "engine/arena.h"
#include "engine/os.h"

#if DEV
/* NOTE: assumes non-zero alignment; if 0 is passed, ensure it is handled later */
static inline bool
is_power_of_2_non_zero(size_t alignment, const char *func_name) {
  if ((alignment & (alignment - 1)) != 0) {
    log_errorlf("%s: 'alignment' has to be a power of 2", func_name);
    return false;
  }
  return true;
}
#endif

static inline size_t
__arena_align__(size_t length, size_t alignment
#if DEV
    , const char *func_name
#endif
) {
#if DEV
  if (!alignment) {
    log_errorlf("%s: passing 0 alignment", func_name);
    return length;
  }
#endif
  return (length + alignment - 1) & ~(alignment - 1);
}
#if DEV
#  define arena_align(length, alignment) __arena_align__(length, alignment, __func__)
#else
#  define arena_align __arena_align__
#endif

struct arena *
arena_make(size_t capacity, size_t alignment) {
  if (!capacity) capacity = os_reasonable_default_capacity() - sizeof (struct arena); // defaults to 4G
  struct arena *arena = os_mem_reserve(capacity + sizeof (struct arena));
  if (!arena) {
    log_errorlf("%s: couldn't allocate memory for the arena", __func__);
    return 0;
  }
  size_t ps = arena_align(sizeof (struct arena), os_page_size());
  os_mem_commit(arena, ps);
# if DEV
  if (!is_power_of_2_non_zero(alignment, "arena_make")) return 0;
#endif
  arena->alignment = alignment ? alignment : sizeof (void *);
  arena->capacity = capacity;
  arena->position = 0;
  arena->position_prv = 0;
  arena->array_length = 0;
  arena->commited = ps;
  return arena;
}

bool
arena_destroy(struct arena *arena) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return false;
  }
#endif
  if (!os_mem_free(arena, sizeof (struct arena) + arena->capacity)) {
    log_errorlf("%s: failed to free arena memory", __func__);
    return false;
  }
  return true;
}

void *
arena_get_base(struct arena *arena) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return 0;
  }
#endif
  return arena->data;
}

bool
arena_is_last_alloc(struct arena *arena, void *ptr) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return false;
  }
  if ((uint8_t *)ptr < arena->data || (uint8_t *)ptr >= arena->data + arena->position) {
    log_errorlf("%s: pointer is not valid on this arena", __func__);
    return false;
  }
#endif
  size_t ptr_position = (size_t)ptr - (size_t)arena->data;
  return ptr_position == arena->position_prv;
}

size_t
arena_last_alloc_real_length(struct arena *arena) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return 0;
  }
#endif
  return arena->position - arena->position_prv;
}

void *
arena_push(struct arena *arena, bool not_zeroed, size_t length) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return 0;
  }
  if ((uintptr_t)arena->data % arena->alignment != 0) log_warnlf("%s: poorly aligned arena", __func__);
#endif
  if (arena->position >= arena->capacity) {
    log_errorlf("%s: arena ran out of memory", __func__);
    return 0;
  }
  length = arena_align(length, arena->alignment);
  if (length > arena->capacity - arena->position) {
    log_errorlf("%s: arena does not have enough memory to allocate %zu bytes", __func__, length);
    return 0;
  }
  void *res = &arena->data[arena->position];
  arena->position_prv = arena->position;
  arena->position += length;
  if (arena->position > arena->commited) {
    arena->commited = arena_align(arena->position, os_page_size());
    if (!os_mem_commit(arena, arena->commited)) return 0;
  }
  if (!not_zeroed) (void)memset(res, 0, length);
  return res;
}

bool
arena_pop(struct arena *arena, size_t length) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return false;
  }
#endif
  length = arena_align(length, arena->alignment);
#if DEV
  if (length > arena->position) {
    log_errorlf("%s: length '%zu' is greater than the arena position '%zu'", __func__, length, arena->position);
    return false;
  }
#endif
  arena->position -= length;
  arena->position_prv = arena->position;
  return true;
}

bool
arena_clear(struct arena *arena) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return false;
  }
#endif
  arena->position_prv = 0;
  arena->position = 0;
  return true;
}
