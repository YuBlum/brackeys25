#include "game/global.h"
#include "engine/window.h"

struct global global;

void
global_init(void) {
    static_assert(sizeof (struct global) == sizeof (int), "added global variable but didn't set it up");
}

void
global_update(float dt) {
    (void)dt;
}
