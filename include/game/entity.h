#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "engine/core.h"
#include "engine/math.h"
#include "engine/renderer.h"
#include "game/attributes.h"

#include "game/entity_flags.h"

struct entity_handle {
    uint32_t index;
    uint32_t generation;
};

static inline bool entity_handle_compare(struct entity_handle a, struct entity_handle b) { return a.index==b.index && a.generation==b.generation; }

/* basically the generations starts to be valid at number one
 * because of this the 'ENTITY_NONE' handle would just be valid if the entity specifically at index 0
 * is at its 4294967296th generation, what is highly unlikely to happen */
#define ENTITY_NONE ((struct entity_handle){0})

struct entity {
    enum entity_flag flags;
    enum entity_flag next_flags;
    struct v2 position;
    struct v2 next_position;
    struct v2 direction;
    struct v2 collider_size;
    struct v2 collider_offset;
    struct v2 hitbox_size;
    struct v2 scale;
    struct v2 origin;
    struct v2 offset;
    float speed;
    float walk_speed;
    float recoil_speed;
    float angle;
    float depth;
    float hit_points;
    float wiggle_time;
    float looking_direction;
    float attack_animation_timer;
    float start_angle;
    float end_angle;
    float attack_anticipation;
    float view_radius;
    float following_radius;
    float wait_to_attack;
    enum sprite sprite;
    struct entity_handle target;
    struct entity_handle weapon;
    struct heaviness heaviness;
    bool ending_attack;
};

struct entities {
    struct entity **data;
    uint32_t amount;
};

void entity_add_flags(struct entity *entity, enum entity_flag flags);
void entity_remove_flags(struct entity *entity, enum entity_flag flags);
bool entity_has_flags(struct entity *entity, enum entity_flag flags);
bool entity_has_one_of_flags(struct entity *entity, enum entity_flag flags);
struct entity *entity_make(enum entity_flag flags);
void entity_destroy(struct entity *entity);
struct entity *entity_get_data(struct entity_handle handle);
struct entity_handle entity_get_handle(struct entity *entity);
struct entities entity_manager_get_cached(void);
void entity_manager_init(void);
void entity_manager_update(float dt);

#endif/*__ENTITY_H__*/
