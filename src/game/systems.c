#include <assert.h>
#include "game/systems.h"
#include "game/global.h"
#include "game/attributes_impl.h"
#include "game/map.h"
#include "engine/renderer.h"
#include "engine/window.h"
#include "engine/collision.h"

#define ON_UPDATE_SYSTEM(system, must_have, must_not_have) void system(struct entity *self, [[maybe_unused]] float dt)
#define ON_RENDER_SYSTEM(system, must_have, must_not_have) void system(struct entity *self)

ON_UPDATE_SYSTEM(particle, PARTICLE, _) {
    auto t = min(self->lifetime_current/self->lifetime, 1.0f);
    self->opacity = lerp(1.0f, 0.0f, t);
    self->lifetime_current += dt;
    if (self->lifetime_current >= self->lifetime) {
        entity_destroy(self);
    }
}

ON_UPDATE_SYSTEM(keyboard_control, KEYBOARD_CONTROLLED, WEAPON_ATTACK|KNOCKBACK) {
    self->speed     = self->walk_speed;
    self->direction = v2_unit(V2(
        window_is_key_down(KEY_RIGHT) - window_is_key_down(KEY_LEFT),
        window_is_key_down(KEY_UP)    - window_is_key_down(KEY_DOWN)
    ));
}

ON_UPDATE_SYSTEM(check_to_follow_target, AI_CONTROLLED, FOLLOWING|SLIME_ATTACK) {
    auto target = entity_get_data(self->target);
    if (check_rect_circle(target->position, target->collider_size, self->position, self->start_following_radius) &&
        !map_line_intersects_wall(self->position, target->position, 0)) {
        entity_add_flags(self, FOLLOWING);
        entity_remove_flags(self, WANDER);
        self->stopped_timer = 0.0f;
        self->direction = V2S(0.0f);
        self->wait_to_follow = 0.3f;
        auto attention_part = entity_make(RENDER_SPRITE|MOVABLE|PARTICLE);
        attention_part->position          = V2(self->position.x, self->position.y + renderer_sprite_get_size(self->sprite).y * 0.5f);
        attention_part->direction         = V2(0.0f, 1.0f);
        attention_part->speed             = 2.0f;
        attention_part->sprite            = SPR_ATTENTION;
        attention_part->opacity           = 1.0f;
        attention_part->scale             = V2S(1.0f);
        attention_part->lifetime          = 0.5f;
        attention_part->looking_direction = 1.0f;
    }
}

ON_UPDATE_SYSTEM(start_wandering, AI_CONTROLLED, WANDER|FOLLOWING|SLIME_ATTACK) {
    self->stopped_timer -= dt;
    if (self->stopped_timer <= 0.0f) {
        self->wander_timer = randf_from_to(0.5f, 2.5f);
        self->direction = v2_from_angle(randf() * PI*2);
        self->speed = self->walk_speed;
        entity_add_flags(self, WANDER);
    }
}

ON_UPDATE_SYSTEM(wander, WANDER|AI_CONTROLLED, FOLLOWING|SLIME_ATTACK) {
    self->wander_timer -= dt;
    if (self->wander_timer <= 0.0f) {
        self->stopped_timer = randf_from_to(0.1f, 0.5f);
        self->direction = V2S(0.0f);
        entity_remove_flags(self, WANDER);
    }
}

ON_UPDATE_SYSTEM(follow_target, FOLLOWING, SLIME_ATTACK|KNOCKBACK) {
    self->wait_to_follow -= dt;
    if (self->wait_to_follow > 0.0f) return;
    auto target = entity_get_data(self->target);
    struct v2 target_pos = target->position;
    bool way_is_blocked;
    {
        auto test_position = v2_add(self->position, v2_muls(v2_direction(self->position, target_pos), self->walk_speed * dt));
        for (int i = -1; i <= 1 && !way_is_blocked; i++) {
            for (int j = -1; j <= 1 && !way_is_blocked; j++) {
                struct v2 tile_position = V2(roundf(test_position.x) + i, roundf(test_position.y) + j);
                if (map_get_tile(tile_position) != '#') continue;
                if (check_rect_rect(test_position, self->collider_size, tile_position, V2S(1.0f))) way_is_blocked = true;
            }
        }
    }
    if (map_line_intersects_wall(self->position, target->position, 0) || way_is_blocked) {
        if (!map_indirect_los_to_player(self, dt, &target_pos)) {
            self->speed = 0.0f;
            entity_remove_flags(self, FOLLOWING);
            return;
        }
    }
    self->direction = V2S(0.0f);
    if (entity_has_flags(self, INVINCIBLE)) return;
    if (v2_distance(self->position, target->position) <= self->attack_distance) {
        entity_add_flags(self, self->attack_flag);
        self->speed          = self->attack_speed;
        self->wait_to_attack = 0.1f; // in seconds
    } else {
        auto seek = v2_direction(self->position, target_pos);
        auto cached = entity_manager_get_cached();
        constexpr auto REPEL_RADIUS = 1.0f;
        auto avoid = V2S(0.0f);
        for (uint32_t i = 0; i < cached.amount; i++) {
            auto other = cached.data[i];
            if (!entity_has_flags(other, FOLLOWING) || !entity_handle_compare(other->target, self->target) || other == self) continue;
            auto distance = v2_distance(self->position, other->position);
            if (distance < REPEL_RADIUS) {
                auto dir_to_self = v2_direction(other->position, self->position);
                auto falloff = clamp(1.0f - (distance / REPEL_RADIUS), 0.0f, 1.0f); // to avoid jittering
                avoid = v2_add(avoid, v2_muls(dir_to_self, falloff));
            }
        }
        self->direction = v2_unit(lerp(seek, avoid, 0.5f));
        self->speed     = self->walk_speed;
    }
}

ON_UPDATE_SYSTEM(slime_attack, SLIME_ATTACK, _) {
    auto target = entity_get_data(self->target);
    self->wait_to_attack -= dt;
    if (self->wait_to_attack > 0.0f) return;
    if (self->direction.x == 0.0f && self->direction.y == 0.0f) self->direction = v2_direction(self->position, target->position);
    if (entity_has_flags(self, KNOCKBACK)) {
        entity_remove_flags(self, HITABLE|SLIME_ATTACK);
        return;
    }
    self->speed = lerp_smooth(self->speed, 0.0f, 0.9f, dt);
    if (self->speed <= 6.0f) {
        entity_add_flags(self, HITABLE);
    }
    else if (check_rect_rect(self->position, self->collider_size, target->position, target->hitbox_size) &&
            !entity_has_one_of_flags(target, KNOCKBACK|INVINCIBLE)) {
        entity_add_flags(target, KNOCKBACK|INVINCIBLE);
        target->direction        = self->direction;
        target->speed            = self->recoil_speed;
        target->invincible_timer = target->invincible_max;
    }
    if (self->speed <= 0.5f) {
        self->speed = 0.0f;
        entity_remove_flags(self, SLIME_ATTACK|HITABLE);
    }
}

ON_UPDATE_SYSTEM(update_invincibility, INVINCIBLE, _) {
    self->invincible_timer -= dt;
    self->opacity = !self->opacity;
    if (self->invincible_timer <= 0.0f) {
        entity_remove_flags(self, INVINCIBLE);
        self->opacity = 1.0f;
    }
}

ON_UPDATE_SYSTEM(get_next_position, MOVABLE, _) {
    self->next_position = v2_add(self->position, v2_muls(self->direction, self->speed * dt));
}

ON_UPDATE_SYSTEM(collide_with_map_walls, MOVABLE|COLLIDE_WITH_WALL, _) {
    auto x_test = V2(self->next_position.x, self->position.y);
    auto y_test = V2(self->position.x, self->next_position.y);
    float tile_position_x, tile_position_y;
    bool tile_found_x = false, tile_found_y = false;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            struct v2 tile_position = V2(roundf(self->position.x) + i, roundf(self->position.y) + j);
            if (map_get_tile(tile_position) != '#') continue;
            if (check_rect_rect(x_test, self->collider_size, tile_position, V2S(1.0f))) {
                tile_found_x = true;
                tile_position_x = tile_position.x;
            }
            if (check_rect_rect(y_test, self->collider_size, tile_position, V2S(1.0f))) {
                tile_found_y = true;
                tile_position_y = tile_position.y;
            }
        }
    }
    if (tile_found_x) {
        self->next_position.x = resolve_rect_rect_axis(self->position.x, self->collider_size.x, tile_position_x, 1.0f);
        self->direction.x = entity_has_flags(self, BOUNCE_OF_WALL) ? self->direction.x * -1.0f : 0.0f;
    }
    if (tile_found_y) {
        self->next_position.y = resolve_rect_rect_axis(self->position.y, self->collider_size.y, tile_position_y, 1.0f);
        self->direction.y = entity_has_flags(self, BOUNCE_OF_WALL) ? self->direction.y * -1.0f : 0.0f;
    }
}

ON_UPDATE_SYSTEM(move, MOVABLE, _) {
    self->position = self->next_position;
}

ON_UPDATE_SYSTEM(camera_follow, CAMERA_FOCUS, _) {
    constexpr auto CAMERA_FOLLOW_SPEED = 0.999f;
    auto camera_position = lerp_smooth(renderer_get_offset(), self->position, CAMERA_FOLLOW_SPEED, dt);
    if (camera_position.x+GAME_LEFT   <= 0.0f)       camera_position.x = 0.0f  - GAME_LEFT;
    if (camera_position.x+GAME_RIGHT  >= MAP_W-0.5f) camera_position.x = MAP_W - GAME_RIGHT-0.5f;
    if (camera_position.y+GAME_BOTTOM <= 0.5f)       camera_position.y = 0.0f  - GAME_BOTTOM+0.5f;
    if (camera_position.y+GAME_TOP    >= MAP_H)      camera_position.y = MAP_H - GAME_TOP;
    renderer_set_offset(camera_position);
}

ON_UPDATE_SYSTEM(wiggle_animation, MOVABLE|WIGGLE, WEAPON_ATTACK|KNOCKBACK) {
    if (self->direction.x == 0.0f && self->direction.y == 0.0f) {
        constexpr auto WIGGLE_STOP_SPEED = 0.9999f;
        self->wiggle_timer = 0.0f;
        self->angle = lerp_smooth(self->angle, 0.0f, WIGGLE_STOP_SPEED, dt);
        self->scale = v2_lerp_smooth(self->scale, V2S(1.0f), WIGGLE_STOP_SPEED, dt);
    } else {
        constexpr auto WIGGLE_FREQUENCY = 2.0f;
        constexpr auto WIGGLE_ANGLE     = (float)PI/6.0;
        constexpr auto WIGGLE_SCALE     = 0.1f;
        self->wiggle_timer += dt;
        float t = self->wiggle_timer * WIGGLE_FREQUENCY * PI * 2;
        self->angle = sinf(t) * WIGGLE_ANGLE;
        self->scale.x = 1.0f + sinf(t) * WIGGLE_SCALE;
        self->scale.y = 1.0f + cosf(t) * WIGGLE_SCALE;
    }
}

ON_UPDATE_SYSTEM(squishy_animation, SQUISHY, KNOCKBACK) {
    if (self->direction.x == 0.0f && self->direction.y == 0.0f) return;
    constexpr auto SQUISHY_FREQUENCY = 2.0f;
    constexpr auto SQUISHY_SCALE     = 0.25f;
    self->squishy_timer += dt;
    float t = self->squishy_timer * SQUISHY_FREQUENCY * PI * 2;
    self->scale.x = 1.0f + sinf(t) * SQUISHY_SCALE;
    self->scale.y = 1.0f - sinf(t) * SQUISHY_SCALE;
}

ON_UPDATE_SYSTEM(change_sprite_looking_direction, MOVABLE|RENDER_SPRITE, KNOCKBACK) {
    if (self->direction.x != 0.0f) {
        self->looking_direction = self->direction.x > 0.0f ? 1.0f : -1.0f;
    }
}

ON_UPDATE_SYSTEM(knockback, KNOCKBACK, _) {
    self->speed = lerp_smooth(self->speed, 0.0f, 0.999f, dt);
    if (self->speed <= 0.5f) {
        self->speed = 0.0f;
        entity_remove_flags(self, KNOCKBACK);
    }
}

ON_UPDATE_SYSTEM(update_weapon_when_not_attacking, HAS_WEAPON, WEAPON_ATTACK) {
    constexpr auto WEAPON_SLOPE = (float)PI/6.0;
    auto weapon = entity_get_data(self->weapon);
    weapon->offset            = V2(-0.4f * self->looking_direction, -self->origin.y);
    weapon->angle             = self->angle - WEAPON_SLOPE * self->looking_direction;
    weapon->looking_direction = self->looking_direction;
    weapon->scale             = self->scale;
    weapon->depth             = self->depth+0.1f;
    weapon->origin            = V2S(0.0f);
    int8_t attack_x = window_is_key_down(KEY_ATTACK_RIGHT) - window_is_key_down(KEY_ATTACK_LEFT);
    int8_t attack_y = window_is_key_down(KEY_ATTACK_UP)    - window_is_key_down(KEY_ATTACK_DOWN);
    if ((attack_x || attack_y) && !entity_has_flags(self, KNOCKBACK)) {
        self->attack_animation_timer = g_speed_by_heaviness[weapon->heaviness.value];
        self->ending_attack          = false;
        if (!attack_y) {
            weapon->start_angle     = 0;
            weapon->end_angle       = (2*PI/3) * attack_x;
            weapon->offset          = V2(0.5f * attack_x, -0.25f - self->origin.y);
            weapon->origin          = V2(0.0f, -0.5f);
            weapon->scale.x         = attack_x;
            weapon->collider_offset = V2(0.25f*attack_x, 0.25f);
            weapon->collider_size   = V2(1.25f, 1.0f);
        } else {
            weapon->start_angle = (attack_y * weapon->looking_direction) > 0.0f ? -PI/2 : 3*PI/2;
            weapon->end_angle   = PI/2;
            weapon->offset      = V2(0.0f, 0.5f * attack_y - self->origin.y);
            weapon->origin      = V2(0.0f, -0.5f);
            weapon->scale.y     = weapon->looking_direction;
            weapon->collider_offset = V2(0.5f*attack_x, 0.25f*attack_y);
            weapon->collider_size   = V2(1.0f, 1.0f);
            weapon->collider_size   = V2(1.0f, 1.25f);
        }
        weapon->depth             = self->depth-0.1f;
        weapon->angle             = weapon->start_angle;
        weapon->looking_direction = 1.0f;
        self->direction = v2_unit(V2(-attack_x, -attack_y));
        self->speed     = self->recoil_speed;
        entity_add_flags(self, WEAPON_ATTACK|KNOCKBACK);
        entity_add_flags(weapon, WEAPON_ATTACK|RENDER_COLLIDER);
    }
}

ON_UPDATE_SYSTEM(update_weapon_general, HAS_WEAPON, _) {
    auto weapon = entity_get_data(self->weapon);
    weapon->position = v2_add(self->position, weapon->offset);
    weapon->opacity  = self->opacity;
}

static uint8_t
roll_damage(struct entity *damage_src, bool extra_crit_roll) {
    auto roll = randf();
    return roll < g_attack_threshold_by_strength[damage_src->strength.value][ATTACK_BAD]
            ? (uint8_t)rand_from_to(damage_src->bad_damage_min, damage_src->bad_damage_max)
            : roll < g_attack_threshold_by_strength[damage_src->strength.value][ATTACK_MID]
            ? (uint8_t)rand_from_to(damage_src->mid_damage_min, damage_src->mid_damage_max)
            : roll < g_attack_threshold_by_strength[damage_src->strength.value][ATTACK_GOOD]
            ? (uint8_t)rand_from_to(damage_src->good_damage_min, damage_src->good_damage_max)
            : (uint8_t)(damage_src->good_damage_max + (extra_crit_roll ? roll_damage(damage_src, false) : 0));
}

static inline void
deal_damage(struct entity *damage_src, struct entity *damage_dest) {
    assert(damage_src->strength.value < 6);
    damage_dest->received_damage = roll_damage(damage_src, true);
    entity_add_flags(damage_dest, WAS_HIT);
}

ON_UPDATE_SYSTEM(update_weapon_when_attacking, HAS_WEAPON|WEAPON_ATTACK, _) {
    auto weapon = entity_get_data(self->weapon);
    auto heaviness = g_speed_by_heaviness[weapon->heaviness.value];
    if (self->ending_attack) {
        self->attack_animation_timer += dt;
        if (self->attack_animation_timer >= heaviness) {
            self->attack_animation_timer = heaviness;
            entity_remove_flags(self, WEAPON_ATTACK);
            entity_remove_flags(weapon, WEAPON_ATTACK|RENDER_COLLIDER);
        }
    } else {
        self->attack_animation_timer -= dt;
        if (self->attack_animation_timer <= 0.0f) {
            self->attack_animation_timer = 0.0f;
            self->ending_attack = true;
        }
    }
    auto t = ease_in_power(min(1.0f - self->attack_animation_timer/heaviness, 1.0f), weapon->attack_anticipation);
    weapon->angle = lerp(weapon->start_angle, weapon->end_angle, t);
    self->scale.x = lerp(1.0f, 1.2f, t);
    self->scale.y = lerp(1.0f, 0.8f, t);
    if (entity_has_flags(weapon, WEAPON_ATTACK)) {
        auto cached = entity_manager_get_cached();
        auto self_handle = entity_get_handle(self);
        for (uint32_t i = 0; i < cached.amount; i++) {
            auto other = cached.data[i];
            auto other_handle = entity_get_handle(other);
            if (!entity_has_flags(other, HITABLE) || entity_handle_compare(other_handle, self_handle)) continue;
            if (check_rect_rect(weapon->position, weapon->collider_size, other->position, other->hitbox_size)) {
                entity_remove_flags(weapon, WEAPON_ATTACK|RENDER_COLLIDER);
                other->direction        = v2_direction(self->position, other->position);
                other->speed            = weapon->recoil_speed;
                other->invincible_timer = other->invincible_max;
                deal_damage(weapon, other);
                entity_add_flags(other, KNOCKBACK|INVINCIBLE);
                break;
            }
        }
    }
}

ON_UPDATE_SYSTEM(receive_damage, WAS_HIT, _) {
    self->hit_points = self->received_damage > self->hit_points ? 0 : self->hit_points-self->received_damage;
    entity_remove_flags(self, WAS_HIT);
    if (self->hit_points == 0) entity_destroy(self);
    log_infolf("damage     = %u", self->received_damage);
    log_infolf("hit points = %u", self->hit_points);
}

ON_UPDATE_SYSTEM(update_depth_by_bottom, DEPTH_BY_BOTTOM, _) {
    self->depth = self->position.y - self->collider_size.y * 0.5f;
}

ON_RENDER_SYSTEM(render_sprite, RENDER_SPRITE, _) {
    renderer_request_sprite(
        self->sprite,
        self->position,
        .angle   = self->angle,
        .origin  = { self->origin.x * self->looking_direction, self->origin.y },
        .scale   = { self->scale .x * self->looking_direction, self->scale .y },
        .depth   = self->depth,
        .opacity = self->opacity
    );
}

ON_RENDER_SYSTEM(render_collider, RENDER_COLLIDER, _) {
    if (!global.show_debug) return;
    renderer_request_rect(
        v2_add(self->position, self->collider_offset),
        self->collider_size,
        RGB(0.2f, 0.8f, 0.2f),
        0.4f,
        -100.0f
    );
}

ON_RENDER_SYSTEM(render_hitbox, RENDER_HITBOX, _) {
    if (!global.show_debug) return;
    renderer_request_rect(
        self->position,
        self->hitbox_size,
        entity_has_flags(self, HITABLE) ? BLUE : RED,
        0.4f,
        -100.0f
    );
}

ON_RENDER_SYSTEM(render_view_radius, RENDER_VIEW_RADIUS, _) {
    if (!global.show_debug) return;
    renderer_request_circle(
        self->position,
        self->start_following_radius,
        YELLOW,
        0.4f
    );
}

ON_RENDER_SYSTEM(render_line_to_target, RENDER_LINE_TO_TARGET, _) {
    if (!global.show_debug) return;
    auto target_pos = entity_get_data(self->target)->position;
    auto color      = map_line_intersects_wall(self->position, target_pos, 0) ? RED : GREEN;
    renderer_request_line(self->position, target_pos, UNIT_ONE_PIXEL, color, 0.6f, INFINITY);
}
