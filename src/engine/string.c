#include <string.h>
#include "engine/string.h"

struct str *
str_make_from_view(struct arena *arena, const struct str_view *view) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return 0;
  }
  if (!view || !view->data) {
    log_errorlf("%s: passing invalid string view", __func__);
    return 0;
  }
#endif
  struct str *str = arena_push(arena, true, sizeof (struct str) + view->length + 1);
  if (!str) {
#if DEV
    log_errorlf("%s: couldn't make string", __func__);
#endif
    return 0;
  }
  str->length = view->length;
  str->capacity = arena_last_alloc_real_length(arena) - sizeof (struct str);
  str->data[str->length] = '\0';
  (void)memcpy(str->data, view->data, str->length);
  return str;
}

struct str *
str_make_from_joint(struct arena *arena, const struct str_joint *joint) {
#if DEV
  if (!arena) {
    log_errorlf("%s: passing invalid arena", __func__);
    return 0;
  }
  if (!joint) {
    log_errorlf("%s: passing invalid string joint", __func__);
    return 0;
  }
#endif
  size_t length = 0;
  for (size_t i = 0; i < joint->amount; i++) {
#if DEV
    if (!joint->views[i].data) {
      log_errorlf("%s: passing invalid view inside string joint", __func__);
      return 0;
    }
#endif
    length += joint->views[i].length;
  }
  struct str *str = arena_push(arena, true, sizeof (struct str) + length + 1);
  if (!str) {
#if DEV
    log_errorlf("%s: couldn't make string", __func__);
#endif
    return 0;
  }
  str->length = length;
  str->capacity = arena_last_alloc_real_length(arena) - sizeof (struct str);
  char *data = str->data;
  for (size_t i = 0; i < joint->amount; i++) {
    memcpy(data, joint->views[i].data, joint->views[i].length);
    data += joint->views[i].length;
  }
  str->data[str->length] = '\0';
  return str;
}

bool
str_view_make_from_str(struct str_view *out, const struct str *str, size_t start, size_t end) {
#if DEV
  if (!out) {
    log_errorlf("%s: invalid string view output", __func__);
    return false;
  }
  if (!str) {
    log_errorlf("%s: invalid string input", __func__);
    return false;
  }
  if (start >= str->length) {
    log_errorlf("%s: 'start' index is greater than or equal to input string length", __func__);
    return false;
  }
  if (end && end < start) {
    log_errorlf("%s: 'end' index is less than 'start' index", __func__);
    return false;
  }
  if (end > str->length) {
    log_errorlf("%s: 'end' index is greater than input string length", __func__);
    return false;
  }
#endif
  out->length = end ? end - start : str->length - start;
  out->data = &str->data[start];
  return true;
}

// TODO: concat string view and string joint to string
