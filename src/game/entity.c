#include "game/entity.h"
#include "game/systems.h"

#define ENTITY_CAP 1024

static struct entity_manager {
    struct entity   *cached[ENTITY_CAP]; 
    struct entity    data[ENTITY_CAP];
    struct entity    stub;
    uint32_t         generations[ENTITY_CAP];
    uint32_t         cached_index[ENTITY_CAP]; 
    uint32_t         free_list[ENTITY_CAP];
    uint32_t         cached_amount;
    uint32_t         free_list_amount;
} g_entities;

#include "game/entities_update_and_render_impl.h"

void
entity_add_flags(struct entity *entity, enum entity_flag flags) {
    entity->next_flags |= flags;
}

void
entity_remove_flags(struct entity *entity, enum entity_flag flags) {
    entity->next_flags &= ~flags;
}

bool
entity_get_flags(struct entity *entity, enum entity_flag flags) {
    return (entity->flags & flags) == flags;
}

struct entity *
entity_make(enum entity_flag flags) {
#if DEV
    if (!g_entities.free_list_amount) {
        log_warnlf("%s: trying to make too many g_entities", __func__);
        g_entities.stub = (struct entity) { 0 };
        return &g_entities.stub;
    }
#endif
    auto index = g_entities.free_list[--g_entities.free_list_amount];
    g_entities.generations[index]++;
    auto entity = &g_entities.data[index];
    g_entities.cached_index[index] = g_entities.cached_amount;
    g_entities.cached[g_entities.cached_amount++] = entity;
    *entity = (struct entity) { 0 };
    entity->next_flags = flags|ALIVE;
    entity->flags      = entity->next_flags;
    return entity;
}

void
entity_destroy(struct entity *entity) {
    auto index = (uint32_t)(entity - g_entities.data);
#if DEV
    if (index >= ENTITY_CAP) {
        log_errorlf("%s: invalid entity", __func__);
        return;
    }
#endif
    if (!entity_get_flags(entity, ALIVE)) return;
    entity->flags = NO_FLAGS;
    g_entities.free_list[g_entities.free_list_amount++] = index;
    g_entities.cached[g_entities.cached_index[index]] = g_entities.cached[--g_entities.cached_amount];
}

struct entity *
entity_get_data(struct entity_handle handle) {
    if (handle.index >= ENTITY_CAP || handle.generation != g_entities.generations[handle.index]) {
        g_entities.stub = (struct entity) { 0 };
        return &g_entities.stub;
    }
    auto e = &g_entities.data[handle.index];
    if (!entity_get_flags(e, ALIVE)) {
        g_entities.stub = (struct entity) { 0 };
        return &g_entities.stub;
    }
    return e;
}

struct entity_handle
entity_get_handle(struct entity *entity) {
    if (entity == &g_entities.stub) return ENTITY_NONE;
    struct entity_handle handle;
    handle.index = (uint32_t)(entity - g_entities.data);
#if DEV
    if (handle.index >= ENTITY_CAP) {
        log_errorlf("%s: invalid entity", __func__);
        handle.index = 0;
        handle.generation = 0;
        return handle;
    }
#endif
    handle.generation = g_entities.generations[handle.index];
    return handle;
}

struct entities
entity_manager_get_cached(void) {
    return (struct entities) { .data = g_entities.cached, .amount = g_entities.cached_amount };
}

void
entity_manager_init(void) {
    g_entities.free_list_amount = 0;
    for (int64_t i = ENTITY_CAP - 1; i >= 0; i--) {
        g_entities.free_list[g_entities.free_list_amount++] = i;
    }
}

