#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include "game/entity.h"

void keyboard_control(struct entity *self, float dt);
void check_to_follow_target(struct entity *self, float dt);
void follow_target(struct entity *self, float dt);
void slime_attack(struct entity *self, float dt);
void update_invincibility(struct entity *self, float dt);
void get_next_position(struct entity *self, float dt);
void collide_with_map_walls(struct entity *self, float dt);
void move(struct entity *self, float dt);
void camera_follow(struct entity *self, float dt);
void wiggle_animation(struct entity *self, float dt);
void squishy_animation(struct entity *self, float dt);
void change_sprite_looking_direction(struct entity *self, float dt);
void knockback(struct entity *self, float dt);
void update_weapon_when_not_attacking(struct entity *self, float dt);
void update_weapon_general(struct entity *self, float dt);
void update_weapon_when_attacking(struct entity *self, float dt);
void receive_damage(struct entity *self, float dt);
void update_depth_by_bottom(struct entity *self, float dt);
void render_sprite(struct entity *self);
void render_collider(struct entity *self);
void render_hitbox(struct entity *self);
void render_view_radius(struct entity *self);

#endif/*__SYSTEMS_H__*/
