#include <stdlib.h>
#include <time.h>
#include "engine/arena.h"
#include "engine/window.h"
#include "engine/mixer.h"
#include "engine/renderer.h"
#include "game/global.h"
#include "game/sound.h"
#include "game/entity.h"

static void
test_render(struct entity *self) {
    (void)self;
    renderer_request_animation(ANIM_TEST_TEST, 0, V2S(0.0f));
}

void
test_entities(void) {
    auto test = entity_make(NO_FLAGS);
    test->render = test_render;
}

bool
game_loop(struct arena *arena) {
    float dt = window_frame_begin();
    (void)arena_clear(arena);
    if (window_is_key_down(KEY_EXIT)) window_close();
    global_update(dt);
    entity_manager_update(dt);
    renderer_submit();
    //log_infolf("FPS: %g", 1.0f/dt);
    return window_frame_end();
}

int
main(void) {
    struct arena *arena = arena_make(0, 0);
    //srand(time(0));
    srand(1755281900);
    if (!window_make()) return 1;
    if (!mixer_make()) {
        window_destroy();
        return 1;
    }
    if (!sound_init()) {
        mixer_destroy();
        window_destroy();
        return 1;
    }
    if (!renderer_make()) {
        mixer_destroy();
        window_destroy();
        return 1;
    }
    entity_manager_init();
    global_init();
    test_entities();
    window_run(arena);
    mixer_destroy();
    window_destroy();
    return 0;
}
