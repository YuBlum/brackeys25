#include <string.h>
#include "engine/string.h"

void
string_builder_init(struct arena *arena, struct string_builder *out) {
    if (!out) {
        log_errorlf("%s: out is null", __func__);
        return;
    }
    if (!arena) {
        log_errorlf("%s: arena is null", __func__);
        *out = (struct string_builder){ 0 };
        return;
    }
    out->arena = arena;
    out->result.data = arena_get_top(arena);
    out->result.length = 0;
}

void
string_builder_append(struct string_builder *builder, struct string *string) {
    auto buf = arena_push_array(builder->arena, true, char, string->length);
    if (!buf) {
        log_errorlf("%s: couldn't allocate enough memory", __func__);
        return;
    }
    builder->result.length += string->length;
    (void)memcpy(buf, string->data, string->length);
}

