#ifndef __YSTRING_H__
#define __YSTRING_H__

#include "engine/core.h"
#include "engine/arena.h"

struct str {
  size_t length;
  size_t capacity;
  char data[];
};

struct str_view {
  size_t length;
  const char *data;
};
#define str_view_make_from_lit(str_lit) (struct str_view) { sizeof (str_lit)-1, str_lit }

struct str_joint {
  size_t amount;
  struct str_view views[];
};
#define str_join_make(...) (struct str_joint) { sizeof ((struct str_view) { __VA_ARGS__ }) / sizeof (struct str_view), { __VA_ARGS__ } }


struct str *str_make_from_view(struct arena *arena, const struct str_view *view);
#define str_make_from_lit(arena, str_lit) str_make_from_view(arena, str_view_make_from_lit(str_lit))

struct str *str_make_from_joint(struct arena *arena, const struct str_joint *joint);
#define str_make_from_views(arena, ...) str_make_from_joint(arena, str_join_make(__VA_ARGS__))

bool str_view_make_from_str(struct str_view *out, const struct str *str, size_t start, size_t end);

#endif/*__YSTRING_H__*/
