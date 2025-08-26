#include <stdlib.h>
#include <time.h>
#include "engine/arena.h"
#include "engine/window.h"
#include "engine/mixer.h"
#include "engine/renderer.h"
#include "game/global.h"
#include "game/sound.h"
#include "game/entity.h"

void
test_entities(void) {
    auto player = entity_make(RENDER_SPRITE|MOVABLE|KEYBOARD_CONTROLLED|HAS_WEAPON|WIGGLE);
    auto sword  = entity_make(RENDER_SPRITE|WEAPON);

    player->position          = V2S(0.0f);
    player->size              = V2S(1.0f);
    player->speed             = 5.0f;
    player->sprite            = SPR_PLAYER;
    player->scale             = V2S(1.0f);
    player->looking_direction = 1.0f;
    player->weapon            = entity_get_handle(sword);

    sword->sprite = SPR_REGULAR_SWORD;

    //auto cursor = entity_make(RENDER_ANIMATION|STATE_MACHINE|FOLLOW_CURSOR);
    //cursor->state_animation[STM_IDLE]    = ANIM_AIM_IDLE;
    //cursor->state_animation[STM_PRESSED] = ANIM_AIM_PRESSED;
    //cursor->scale                        = V2S(1.0f);
    //cursor->depth                        = -INFINITY;
}

bool
game_loop(struct arena *arena) {
    float dt = window_frame_begin();
    (void)arena_clear(arena);
    if (window_is_key_down(KEY_EXIT)) window_close();
    global_update(dt);
    entity_manager_update(dt);
    //renderer_request_text(&string_make(" !\"#$%&'()*+,-./0123456789:;<=>?\n@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]\n^_`abcdefghijklmnopqrstuvwxyz{|}~"), V2(-5.0f, 0.0f));
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
