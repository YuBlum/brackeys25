#include "game/global.h"
#include "engine/window.h"

struct global global;

void
global_init(void) {
  static_assert(
    sizeof (struct global) ==
      (sizeof (float)) +
      (sizeof (bool) + 3), /* 3 bytes of padding */
    "added global variable but didn't set it up"
  );
  global.show_colliders = false;
  global.time = 0;
}

void
global_update(float dt) {
  if (window_is_key_press(KEY_DEBUG0)) global.show_colliders = !global.show_colliders;
  global.time += dt;
}
