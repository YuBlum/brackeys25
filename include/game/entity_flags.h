#ifndef __ENTITY_FLAGS_H__
#define __ENTITY_FLAGS_H__

enum entity_flag {
  NO_FLAGS = 0,
  ANY_FLAG = 0,
  ALIVE = 1 << 0,
  RENDER_COLLIDER = 1 << 1,
  RENDER_HITBOX = 1 << 2,
  RENDER_SPRITE = 1 << 3,
  RENDER_VIEW_RADIUS = 1 << 4,
  MOVABLE = 1 << 5,
  KEYBOARD_CONTROLLED = 1 << 6,
  AI_CONTROLLED = 1 << 7,
  HAS_WEAPON = 1 << 8,
  WEAPON_ATTACK = 1 << 9,
  WIGGLE = 1 << 10,
  HITABLE = 1 << 11,
  DEPTH_BY_BOTTOM = 1 << 12,
  KNOCKBACK = 1 << 13,
  CHECK_TO_FOLLOW = 1 << 14,
  FOLLOW = 1 << 15,
  SLIME_ATTACK = 1 << 16,
  INVINCIBLE = 1 << 17,
};

#endif/*__ENTITY_FLAGS_H__*/
