#include "game/systems.h"
#include "game/global.h"
#include "game/attributes_impl.h"
#include "engine/renderer.h"
#include "engine/window.h"
#include "engine/collision.h"

#define ON_UPDATE_SYSTEM(system, must_have, must_not_have) void system(struct entity *self, float dt)
#define ON_RENDER_SYSTEM(system, must_have, must_not_have) void system(struct entity *self)

ON_UPDATE_SYSTEM(keyboard_control, KEYBOARD_CONTROLLED, WEAPON_ATTACK|KNOCKBACK) {
    (void)dt;
    self->speed     = self->walk_speed;
    self->direction = v2_unit(V2(
        window_is_key_down(KEY_RIGHT) - window_is_key_down(KEY_LEFT),
        window_is_key_down(KEY_UP)    - window_is_key_down(KEY_DOWN)
    ));
}

ON_UPDATE_SYSTEM(check_to_follow_target, CHECK_TO_FOLLOW, FOLLOW|SLIME_ATTACK) {
    (void)dt;
    auto target = entity_get_data(self->target);
    if (check_rect_circle(target->position, target->collider_size, self->position, self->view_radius)) {
        entity_add_flags(self, FOLLOW);
    }
}

ON_UPDATE_SYSTEM(follow_target, FOLLOW, KNOCKBACK) {
    (void)dt;
    auto target = entity_get_data(self->target);
    if (!check_rect_circle(target->position, target->collider_size, self->position, self->following_radius)) {
        self->speed = 0.0f;
        entity_remove_flags(self, FOLLOW);
        return;
    }
    constexpr auto ATTACK_DIST = 1.5f;
    self->direction = V2S(0.0f);
    if (entity_has_flags(self, INVINCIBLE)) return;
    if (v2_distance(self->position, target->position) <= ATTACK_DIST) {
        entity_add_flags(self, SLIME_ATTACK);
        entity_remove_flags(self, FOLLOW);
        self->speed          = 10.0f; // attack speed
        self->wait_to_attack = 0.1f; // in seconds
    } else {
        self->speed     = self->walk_speed;
        self->direction = v2_direction(self->position, target->position);
    }
}

ON_UPDATE_SYSTEM(slime_attack, SLIME_ATTACK, _) {
    (void)dt;
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

ON_UPDATE_SYSTEM(move, MOVABLE, _) {
    (void)dt;
    self->position = self->next_position;
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

ON_UPDATE_SYSTEM(squishy_animation, FOLLOW|SQUISHY, KNOCKBACK) {
    constexpr auto SQUISHY_FREQUENCY = 2.0f;
    constexpr auto SQUISHY_SCALE     = 0.25f;
    self->squishy_timer += dt;
    float t = self->squishy_timer * SQUISHY_FREQUENCY * PI * 2;
    self->scale.x = 1.0f + sinf(t) * SQUISHY_SCALE;
    self->scale.y = 1.0f - sinf(t) * SQUISHY_SCALE;
}

ON_UPDATE_SYSTEM(change_sprite_looking_direction, MOVABLE|RENDER_SPRITE, KNOCKBACK) {
    (void)dt;
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
    (void)dt;
    constexpr auto WEAPON_SLOPE = (float)PI/6.0;
    auto weapon = entity_get_data(self->weapon);
    weapon->offset            = V2(-0.4f * self->looking_direction, 0.0f);
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
            weapon->offset          = V2(0.5f * attack_x, -0.25f);
            weapon->origin          = V2(0.0f, -0.5f);
            weapon->scale.x         = attack_x;
            weapon->collider_offset = V2(0.25f*attack_x, 0.25f);
            weapon->collider_size   = V2(1.25f, 1.0f);
        } else {
            weapon->start_angle = (attack_y * weapon->looking_direction) > 0.0f ? -PI/2 : 3*PI/2;
            weapon->end_angle   = PI/2;
            weapon->offset      = V2(0.0f, 0.5f * attack_y);
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
    (void)dt;
    auto weapon = entity_get_data(self->weapon);
    weapon->position = v2_add(self->position, weapon->offset);
    weapon->opacity  = self->opacity;
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
                entity_add_flags(other, KNOCKBACK|INVINCIBLE);
                break;
            }
        }
    }
}

ON_UPDATE_SYSTEM(update_depth_by_bottom, DEPTH_BY_BOTTOM, _) {
    (void)dt;
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
        GREEN,
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
        self->following_radius,
        BLUE,
        0.2f
    );
    renderer_request_circle(
        self->position,
        self->view_radius,
        YELLOW,
        0.4f
    );
}
