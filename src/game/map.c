#include "engine/renderer.h"
#include "game/entity.h"
#include "game/map.h"

static char g_map[MAP_W*MAP_H] __attribute__((nonstring)) =
"#########################"
"#########################"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##..........@..........##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.....................##"
"##.......#########.....##"
"##.....................##"
"##.....................##"
"#########################"
"#########################";

void
map_init(void) {
    for (uint32_t y = 0; y < MAP_H; y++) {
        for (uint32_t x = 0; x < MAP_W; x++) {
            auto tile_position = V2(x, MAP_H - y);
            switch (g_map[y*MAP_W+x]) {
                case '@': {
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

                    renderer_set_offset(tile_position);

                    player->sprite            = SPR_PLAYER;
                    player->position          = tile_position;
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
                } break;
                default: {
                } break;
            }
        }
    }
}

char
map_get_tile(struct v2 position) {
  if (position.x < 0.0f || position.y < 0.0f || position.x >= MAP_W || position.y >= MAP_H) return true;
  return g_map[(MAP_H - (int)position.y) * MAP_W + (int)position.x];
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
