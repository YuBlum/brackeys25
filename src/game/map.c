#include "engine/renderer.h"
#include "game/entity.h"
#include "game/map.h"

static char g_map[MAP_W*MAP_H] __attribute__((nonstring)) =
"#########################"
"#.......................#"
"#.......................#"
"#.......................#"
"#............#######....#"
"#............#######....#"
"#.......................#"
"#.......................#"
"#.......................#"
"#...........@...........#"
"#.......................#"
"#......#................#"
"#......#................#"
"#......#................#"
"#......#................#"
"#......#................#"
"#..................S....#"
"#.......................#"
"#.......................#"
"#########################";

void
map_init(void) {
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
        | COLLIDE_WITH_WALL
        | CAMERA_FOCUS
    );

    auto sword  = entity_make(
        0
        | RENDER_SPRITE
    );

    auto player_handle = entity_get_handle(player);

    player->sprite            = SPR_PLAYER;
    player->collider_size     = V2S(1.0f);
    player->walk_speed        = 5.0f;
    player->recoil_speed      = 4.0f;
    player->scale             = V2S(1.0f);
    player->looking_direction = 1.0f;
    player->weapon            = entity_get_handle(sword);
    player->hit_points        = 10;
    player->hitbox_size       = V2(0.5f, 0.75f);
    player->invincible_max    = 0.3f;
    player->opacity           = 1.0f;

    sword->sprite              = SPR_REGULAR_SWORD;
    sword->heaviness.value     = 0;
    sword->attack_anticipation = 3.0f;
    sword->recoil_speed        = 10.0f;
    sword->bad_damage_min      = 3;
    sword->bad_damage_max      = 4;
    sword->mid_damage_min      = 5;
    sword->mid_damage_max      = 10;
    sword->good_damage_min     = 8;
    sword->good_damage_max     = 12;

    for (uint32_t y = 0; y < MAP_H; y++) {
        for (uint32_t x = 0; x < MAP_W; x++) {
            auto tile_position = V2(x, MAP_H - y);
            switch (g_map[y*MAP_W+x]) {
                case '@': {
                    player->position = tile_position;
                    renderer_set_offset(tile_position);
                } break;
                case 'S':{
                    auto slime  = entity_make(
                        0
                        | RENDER_SPRITE
                        | RENDER_HITBOX
                        | RENDER_VIEW_RADIUS
                        | RENDER_LINE_TO_TARGET
                        | MOVABLE
                        | AI_CONTROLLED
                        | DEPTH_BY_BOTTOM
                        | CHECK_TO_FOLLOW
                        | SQUISHY
                        | COLLIDE_WITH_WALL
                    );

                    slime->sprite            = SPR_SLIME;
                    slime->position          = tile_position;
                    slime->scale             = V2S(1.0f);
                    slime->looking_direction = 1.0f;
                    slime->collider_size     = V2(1.0f, 0.5f);
                    slime->hitbox_size       = V2S(1.0f);
                    slime->hit_points        = 10;
                    slime->view_radius       = 4.0f;
                    slime->following_radius  = 6.0f;
                    slime->target            = player_handle;
                    slime->walk_speed        = 4.0f;
                    slime->recoil_speed      = 5.0f;
                    slime->opacity           = 1.0f;
                    slime->invincible_max    = 0.3f;
                } break;
                default: {
                } break;
            }
        }
    }
}

char
map_get_tile(struct v2 position) {
  if (position.x < 0.0f || position.y < 0.0f || position.x >= MAP_W || position.y >= MAP_H) return '#';
  return g_map[(MAP_H - (int)position.y) * MAP_W + (int)position.x];
}

bool
map_line_intersects_wall(struct v2 a, struct v2 b) {
    a = v2_adds(a, 0.5f);
    b = v2_adds(b, 0.5f);
    auto ab     = v2_sub(b, a);
    auto ab_mag = v2_mag(ab);
    if (ab_mag == 0.0f) return false;
    auto dir = v2_divs(ab, ab_mag);
    constexpr auto STEP = 0.05f;
    for (float cur_mag = 0.0f; cur_mag <= ab_mag; cur_mag += STEP) {
        if (map_get_tile(v2_add(a, v2_muls(dir, cur_mag))) == '#') return true;
    }
    return false;
}

void
map_render(void) {
    for (uint32_t y = 0; y < MAP_H; y++) {
        for (uint32_t x = 0; x < MAP_W; x++) {
            if (g_map[y*MAP_W+x] != '#') continue;
            auto position = V2(x, MAP_H - y);
            renderer_request_rect(position, V2S(1.0f), GRAY(0.5f), 1.0f, INFINITY);
        }
    }
}
