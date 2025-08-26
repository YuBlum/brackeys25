#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include "game/entity.h"

void follow_cursor(struct entity *self, float dt);
void keyboard_control(struct entity *self, float dt);
void stop_moving_on_attack(struct entity *self, float dt);
void get_next_position(struct entity *self, float dt);
void move(struct entity *self, float dt);
void wiggle_animation(struct entity *self, float dt);
void change_sprite_looking_direction(struct entity *self, float dt);
void update_weapon(struct entity *self, float dt);
void update_attack(struct entity *self, float dt);
void update_cursor_state(struct entity *self, float dt);
void loop_animation(struct entity *self, float dt);
void update_animation(struct entity *self, float dt);
void render_animation(struct entity *self);
void render_sprite(struct entity *self);
void render_collider(struct entity *self);

#endif/*__SYSTEMS_H__*/
