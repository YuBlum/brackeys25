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
    auto player = entity_make(
        0
        | RENDER_SPRITE
        | RENDER_HITBOX
        | MOVABLE
        | KEYBOARD_CONTROLLED
        | HAS_WEAPON
        | WIGGLE
        | HITABLE
        | DEPTH_BY_BOTTOM
    );
    auto sword  = entity_make(
        0
        | RENDER_SPRITE
    );
    auto slime  = entity_make(
        0
        | RENDER_SPRITE
        | RENDER_HITBOX
        | RENDER_VIEW_RADIUS
        | MOVABLE
        | AI_CONTROLLED
        | DEPTH_BY_BOTTOM
        | CHECK_TO_FOLLOW
        | SQUISHY
    );

    player->sprite            = SPR_PLAYER;
    player->position          = V2S(0.0f);
    player->collider_size     = V2S(1.0f);
    player->walk_speed        = 5.0f;
    player->recoil_speed      = 4.0f;
    player->scale             = V2S(1.0f);
    player->looking_direction = 1.0f;
    player->weapon            = entity_get_handle(sword);
    player->hit_points        = 10.0f;
    player->hitbox_size       = V2(0.5f, 0.75f);
    player->invincible_max    = 0.3f;
    player->opacity           = 1.0f;

    sword->sprite              = SPR_REGULAR_SWORD;
    sword->heaviness.value     = 0;
    sword->attack_anticipation = 3.0f;
    sword->recoil_speed        = 10.0f;

    slime->sprite            = SPR_SLIME;
    slime->position          = V2S(-4.0f);
    slime->scale             = V2S(1.0f);
    slime->looking_direction = 1.0f;
    slime->collider_size     = V2(1.0f, 0.5f);
    slime->hitbox_size       = V2S(1.0f);
    slime->view_radius       = 4.0f;
    slime->following_radius  = 6.0f;
    slime->target            = entity_get_handle(player);
    slime->walk_speed        = 4.0f;
    slime->recoil_speed      = 6.0f;
    slime->opacity           = 1.0f;
    slime->invincible_max    = 0.3f;

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
