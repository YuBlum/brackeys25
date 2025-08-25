#ifndef __ENTITY_FLAGS_H__
#define __ENTITY_FLAGS_H__

enum entity_flag {
  NO_FLAGS = 0,
  ALIVE = 1 << 0,
  RENDER_COLLIDER = 1 << 1,
  RENDER_SPRITE = 1 << 2,
  RENDER_ANIMATION = 1 << 3,
  MOVABLE = 1 << 4,
  KEYBOARD_CONTROLLED = 1 << 5,
  HAS_WEAPON = 1 << 6,
  FOLLOW_CURSOR = 1 << 7,
  STATE_MACHINE = 1 << 8,
  LOOPABLE = 1 << 9,
};

#endif/*__ENTITY_FLAGS_H__*/
