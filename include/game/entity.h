#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "engine/core.h"
#include "engine/math.h"
#include "engine/renderer.h"

enum entity_state {
    STM_IDLE = 0,
    STM_WALK,
    STM_PRESSED = STM_WALK,
    STM_AMOUNT
};

enum entity_flag {
    NO_FLAGS = 0,
    IS_ALIVE = 1 << 0,
};

struct entity_handle {
    uint32_t index;
    uint32_t generation;
};
/* bascially the generations starts to be valid at number one
 * because of this the 'ENTITY_HANDLE_NONE' would just be valid if the entity specifically at index 0
 * is at its 4294967296th generation, what is highly unlikely to happen */
#define ENTITY_NONE ((struct entity_handle){0})

struct entity {
    void (*update)(struct entity *, float);
    void (*render)(struct entity *);
    enum entity_flag flags;
    enum entity_flag next_flags;
    struct v2 position;
};

struct entities {
    struct entity **data;
    uint32_t amount;
};

void entity_add_flags(struct entity *entity, enum entity_flag flags);
void entity_remove_flags(struct entity *entity, enum entity_flag flags);
bool entity_get_flags(struct entity *entity, enum entity_flag flags);
struct entity *entity_make(enum entity_flag flags);
void entity_destroy(struct entity *entity);
struct entity *entity_get_data(struct entity_handle handle);
struct entity_handle entity_get_handle(struct entity *entity);
struct entities entity_manager_get_cached(void);
void entity_manager_init(void);
void entity_manager_update(float dt);

#endif/*__ENTITY_H__*/
