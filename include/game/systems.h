#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include "game/entity.h"

void keyboard_control(struct entity *self, float dt);
void check_to_follow_target(struct entity *self, float dt);
void follow_target(struct entity *self, float dt);
void get_next_position(struct entity *self, float dt);
void move(struct entity *self, float dt);
void wiggle_animation(struct entity *self, float dt);
void change_sprite_looking_direction(struct entity *self, float dt);
void knockback(struct entity *self, float dt);
void update_weapon(struct entity *self, float dt);
void update_weapon_position(struct entity *self, float dt);
void update_attack(struct entity *self, float dt);
void update_depth_by_bottom(struct entity *self, float dt);
void render_sprite(struct entity *self);
void render_collider(struct entity *self);
void render_hitbox(struct entity *self);
void render_view_radius(struct entity *self);

#endif/*__SYSTEMS_H__*/
