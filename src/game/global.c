#include "game/global.h"
#include "engine/window.h"

struct global global;

void
global_init(void) {
    static_assert(sizeof (struct global) == sizeof (bool), "added global variable but didn't set it up");
    global.show_debug = false;
}

void
global_update(float dt) {
    (void)dt;
    if (window_is_key_press(KEY_DEBUG0)) global.show_debug = !global.show_debug;
}
