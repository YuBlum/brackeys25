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
  RENDER_LINE_TO_TARGET = 1 << 5,
  MOVABLE = 1 << 6,
  KEYBOARD_CONTROLLED = 1 << 7,
  AI_CONTROLLED = 1 << 8,
  HAS_WEAPON = 1 << 9,
  WEAPON_ATTACK = 1 << 10,
  WIGGLE = 1 << 11,
  HITABLE = 1 << 12,
  WAS_HIT = 1 << 13,
  DEPTH_BY_BOTTOM = 1 << 14,
  KNOCKBACK = 1 << 15,
  FOLLOWING = 1 << 16,
  SLIME_ATTACK = 1 << 17,
  INVINCIBLE = 1 << 18,
  SQUISHY = 1 << 19,
  COLLIDE_WITH_WALL = 1 << 20,
  CAMERA_FOCUS = 1 << 21,
};

#endif/*__ENTITY_FLAGS_H__*/
