#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#include "engine/core.h"
#include "engine/arena.h"

struct string {
    const char *data;
    size_t length;
};
#define string_make(str_lit) (struct string) { str_lit, sizeof (str_lit)-1 }

struct string_builder {
    struct arena *arena;
    struct string result;
};

void string_builder_init(struct arena *arena, struct string_builder *out);
void string_builder_append(struct string_builder *builder, struct string *string);
#define string_builder_append_lit(builder, str_lit) string_builder_append_lit(builder, &string_make(str_lit))

#endif/*__MY_STRING_H__*/
