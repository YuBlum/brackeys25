#include "game/systems.h"
#include "game/global.h"
#include "engine/renderer.h"
#include "engine/window.h"
#include "engine/collision.h"

#define ON_UPDATE_SYSTEM(system, ...) void system(struct entity *self, float dt)
#define ON_RENDER_SYSTEM(system, ...) void system(struct entity *self)

ON_UPDATE_SYSTEM(follow_cursor, FOLLOW_CURSOR) {
  (void)dt;
  self->position = window_get_cursor_position();
}

ON_UPDATE_SYSTEM(keyboard_control, KEYBOARD_CONTROLLED) {
    (void)dt;
    self->direction = v2_unit(V2(
        window_is_key_down(KEY_RIGHT) - window_is_key_down(KEY_LEFT),
        window_is_key_down(KEY_UP)    - window_is_key_down(KEY_DOWN)
    ));
}

ON_UPDATE_SYSTEM(get_next_position, MOVABLE) {
    self->next_position = v2_add(self->position, v2_muls(self->direction, self->speed * dt));
}

ON_UPDATE_SYSTEM(move, MOVABLE) {
    (void)dt;
    self->position = self->next_position;
}

ON_UPDATE_SYSTEM(wiggle_animation, MOVABLE, WIGGLE) {
    if (self->direction.x == 0.0f && self->direction.y == 0.0f) {
        constexpr auto WIGGLE_STOP_SPEED = 0.9999f;
        self->wiggle_time = 0.0f;
        self->angle = lerp_smooth(self->angle, 0.0f, WIGGLE_STOP_SPEED, dt);
        self->scale = v2_lerp_smooth(self->scale, V2S(1.0f), WIGGLE_STOP_SPEED, dt);
    } else {
        constexpr auto WIGGLE_FREQUENCY = 2.0f;
        constexpr auto WIGGLE_ANGLE     = (float)PI/6.0;
        constexpr auto WIGGLE_SCALE     = 0.1f;
        self->wiggle_time += dt;
        float t = self->wiggle_time * WIGGLE_FREQUENCY * PI * 2;
        self->angle = sinf(t) * WIGGLE_ANGLE;
        self->scale.x = 1.0f + sinf(t) * WIGGLE_SCALE;
        self->scale.y = 1.0f + cosf(t) * WIGGLE_SCALE;
    }
}

ON_UPDATE_SYSTEM(change_sprite_looking_direction, MOVABLE, RENDER_SPRITE) {
    (void)dt;
    if (self->direction.x != 0.0f) {
        self->looking_direction = self->direction.x > 0.0f ? 1.0f : -1.0f;
    }
}

ON_UPDATE_SYSTEM(update_weapon, HAS_WEAPON) {
    (void)dt;
    constexpr auto WEAPON_SLOPE = (float)PI/6.0;
    auto weapon = entity_get_data(self->weapon);
    weapon->position          = v2_add(self->position, V2(-0.4f * self->looking_direction, 0.0f));
    weapon->angle             = self->angle - WEAPON_SLOPE * self->looking_direction;
    weapon->looking_direction = self->looking_direction;
    weapon->scale             = self->scale;
    weapon->depth             = self->depth+0.1f;
}

ON_UPDATE_SYSTEM(update_cursor_state, FOLLOW_CURSOR, STATE_MACHINE) {
  (void)dt;
  auto prv_state = self->state;
  if (window_is_button_down(BTN_LEFT)) {
    self->state = STM_PRESSED;
  } else {
    self->state = STM_IDLE;
  }
  if (self->state != prv_state) {
    self->change_frame_timer = 0.0f;
    self->current_frame = 0.0f;
  }
  self->animation = self->state_animation[self->state];
}

ON_UPDATE_SYSTEM(loop_animation, RENDER_ANIMATION, LOOPABLE) {
  (void)dt;
  auto animation = renderer_animation_get_data(self->animation);
  if (self->current_frame >= animation->frames_amount-1 && self->previous_frame >= animation->frames_amount-1) self->current_frame = 0;
}

ON_UPDATE_SYSTEM(update_animation, RENDER_ANIMATION) {
  auto animation = renderer_animation_get_data(self->animation);
  self->change_frame_timer += dt;
  if (self->change_frame_timer < animation->durations[self->current_frame]) return;
  self->change_frame_timer = 0;
  self->previous_frame = self->current_frame;
  self->current_frame++;
  if (self->current_frame > animation->frames_amount-1) self->current_frame = animation->frames_amount-1;
}

ON_RENDER_SYSTEM(render_animation, RENDER_ANIMATION) {
  renderer_request_animation(
    self->animation,
    self->current_frame,
    self->position,
    .origin  = self->origin,
    .angle   = self->angle,
    .scale   = self->scale,
    .depth   = self->depth
  );
}

ON_RENDER_SYSTEM(render_sprite, RENDER_SPRITE) {
    renderer_request_sprite(
        self->sprite,
        self->position,
        .angle   = self->angle,
        .origin  = { self->origin.x * self->looking_direction, self->origin.y },
        .scale   = { self->scale .x * self->looking_direction, self->scale .y },
        .depth   = self->depth
    );
}

ON_RENDER_SYSTEM(render_collider, RENDER_COLLIDER) {
    renderer_request_rect(
        self->position,
        self->size,
        GREEN,
        0.4f,
        -100.0f
    );
}
