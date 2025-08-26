#ifndef __ATLAS_H__
#define __ATLAS_H__

#include "engine/math.h"
#include "engine/sprites.h"

#define ATLAS_WIDTH 1024
#define ATLAS_HEIGHT 32
#define ATLAS_PIXEL_W (1.0f/1024)
#define ATLAS_PIXEL_H (1.0f/32)

#define ATLAS_EXTRUDE_W (2.0f * ATLAS_PIXEL_W)

#define ATLAS_EXTRUDE_H (2.0f * ATLAS_PIXEL_H)

static const uint8_t g_atlas_data[ATLAS_WIDTH*ATLAS_HEIGHT*4] = {
#embed "imgs/atlas.bin"
};

static const struct v2 g_atlas_sprite_positions[SPRITES_AMOUNT] = {
    { 4.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 24.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 29.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 97.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 908.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 950.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 970.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
    { 990.0f * ATLAS_PIXEL_W, 4.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_sizes[SPRITES_AMOUNT] = {
    { 16.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 1.0f * ATLAS_PIXEL_W, 1.0f * ATLAS_PIXEL_H },
    { 42.0f * ATLAS_PIXEL_W, 7.0f * ATLAS_PIXEL_H },
    { 807.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 32.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 16.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 16.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 7.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_half_sizes[SPRITES_AMOUNT] = {
    { 8.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 0.5f * UNIT_ONE_PIXEL, 0.5f * UNIT_ONE_PIXEL },
    { 21.0f * UNIT_ONE_PIXEL, 3.5f * UNIT_ONE_PIXEL },
    { 403.5f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 16.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 8.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 8.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 3.5f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
};

static const float g_atlas_animation_durations_aim_pressed[3] = {
    0.05,
    0.05,
    0.05,
};

static const float g_atlas_animation_durations_aim_idle[3] = {
    0.05,
    0.05,
    0.05,
};

static const float g_atlas_animation_durations_test_test[2] = {
    0.1,
    0.1,
};

static const struct animation_data g_atlas_animations[ANIMATIONS_AMOUNT] = {
    {
        .durations = g_atlas_animation_durations_aim_pressed,
        .frame_width = 7.0f * ATLAS_PIXEL_W,
        .frames_amount = 3,
        .first_frame = 0,
        .sprite = SPR_AIM
    },
    {
        .durations = g_atlas_animation_durations_aim_idle,
        .frame_width = 7.0f * ATLAS_PIXEL_W,
        .frames_amount = 3,
        .first_frame = 3,
        .sprite = SPR_AIM
    },
    {
        .durations = g_atlas_animation_durations_test_test,
        .frame_width = 16.0f * ATLAS_PIXEL_W,
        .frames_amount = 2,
        .first_frame = 0,
        .sprite = SPR_TEST
    },
};

static const struct {
    float offset;
    float atlas_width;
    float game_width;
} g_glyphs['~'-' '+1] = {
    { .offset = 2.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /*   */
    { .offset = 11.0f * ATLAS_PIXEL_W, .atlas_width  = 1.0f * ATLAS_PIXEL_W, .game_width  = 1.0f * UNIT_ONE_PIXEL }, /* ! */
    { .offset = 16.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* " */
    { .offset = 24.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* # */
    { .offset = 33.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* $ */
    { .offset = 42.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* % */
    { .offset = 51.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* & */
    { .offset = 60.0f * ATLAS_PIXEL_W, .atlas_width  = 1.0f * ATLAS_PIXEL_W, .game_width  = 1.0f * UNIT_ONE_PIXEL }, /* ' */
    { .offset = 65.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* ( */
    { .offset = 72.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* ) */
    { .offset = 79.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* * */
    { .offset = 88.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* + */
    { .offset = 97.0f * ATLAS_PIXEL_W, .atlas_width  = 2.0f * ATLAS_PIXEL_W, .game_width  = 2.0f * UNIT_ONE_PIXEL }, /* , */
    { .offset = 103.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* - */
    { .offset = 112.0f * ATLAS_PIXEL_W, .atlas_width  = 1.0f * ATLAS_PIXEL_W, .game_width  = 1.0f * UNIT_ONE_PIXEL }, /* . */
    { .offset = 117.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* / */
    { .offset = 126.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 0 */
    { .offset = 135.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* 1 */
    { .offset = 142.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 2 */
    { .offset = 151.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 3 */
    { .offset = 160.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 4 */
    { .offset = 169.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 5 */
    { .offset = 178.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 6 */
    { .offset = 187.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 7 */
    { .offset = 196.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 8 */
    { .offset = 205.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* 9 */
    { .offset = 214.0f * ATLAS_PIXEL_W, .atlas_width  = 1.0f * ATLAS_PIXEL_W, .game_width  = 1.0f * UNIT_ONE_PIXEL }, /* : */
    { .offset = 219.0f * ATLAS_PIXEL_W, .atlas_width  = 2.0f * ATLAS_PIXEL_W, .game_width  = 2.0f * UNIT_ONE_PIXEL }, /* ; */
    { .offset = 225.0f * ATLAS_PIXEL_W, .atlas_width  = 6.0f * ATLAS_PIXEL_W, .game_width  = 6.0f * UNIT_ONE_PIXEL }, /* < */
    { .offset = 235.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* = */
    { .offset = 244.0f * ATLAS_PIXEL_W, .atlas_width  = 6.0f * ATLAS_PIXEL_W, .game_width  = 6.0f * UNIT_ONE_PIXEL }, /* > */
    { .offset = 254.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* ? */
    { .offset = 262.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* @ */
    { .offset = 271.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* A */
    { .offset = 280.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* B */
    { .offset = 289.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* C */
    { .offset = 298.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* D */
    { .offset = 307.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* E */
    { .offset = 316.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* F */
    { .offset = 325.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* G */
    { .offset = 334.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* H */
    { .offset = 343.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* I */
    { .offset = 350.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* J */
    { .offset = 359.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* K */
    { .offset = 367.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* L */
    { .offset = 376.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* M */
    { .offset = 385.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* N */
    { .offset = 394.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* O */
    { .offset = 403.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* P */
    { .offset = 412.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* Q */
    { .offset = 421.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* R */
    { .offset = 430.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* S */
    { .offset = 439.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* T */
    { .offset = 448.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* U */
    { .offset = 457.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* V */
    { .offset = 466.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* W */
    { .offset = 475.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* X */
    { .offset = 484.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* Y */
    { .offset = 493.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* Z */
    { .offset = 502.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* [ */
    { .offset = 510.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* \ */
    { .offset = 519.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* ] */
    { .offset = 526.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* ^ */
    { .offset = 535.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* _ */
    { .offset = 544.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* ` */
    { .offset = 551.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* a */
    { .offset = 560.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* b */
    { .offset = 569.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* c */
    { .offset = 578.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* d */
    { .offset = 587.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* e */
    { .offset = 596.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* f */
    { .offset = 604.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* g */
    { .offset = 613.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* h */
    { .offset = 622.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* i */
    { .offset = 629.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* j */
    { .offset = 637.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* k */
    { .offset = 645.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* l */
    { .offset = 653.0f * ATLAS_PIXEL_W, .atlas_width  = 7.0f * ATLAS_PIXEL_W, .game_width  = 7.0f * UNIT_ONE_PIXEL }, /* m */
    { .offset = 664.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* n */
    { .offset = 673.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* o */
    { .offset = 682.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* p */
    { .offset = 690.0f * ATLAS_PIXEL_W, .atlas_width  = 4.0f * ATLAS_PIXEL_W, .game_width  = 4.0f * UNIT_ONE_PIXEL }, /* q */
    { .offset = 698.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* r */
    { .offset = 707.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* s */
    { .offset = 716.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* t */
    { .offset = 725.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* u */
    { .offset = 734.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* v */
    { .offset = 743.0f * ATLAS_PIXEL_W, .atlas_width  = 7.0f * ATLAS_PIXEL_W, .game_width  = 7.0f * UNIT_ONE_PIXEL }, /* w */
    { .offset = 754.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* x */
    { .offset = 763.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* y */
    { .offset = 772.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* z */
    { .offset = 781.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* { */
    { .offset = 788.0f * ATLAS_PIXEL_W, .atlas_width  = 1.0f * ATLAS_PIXEL_W, .game_width  = 1.0f * UNIT_ONE_PIXEL }, /* | */
    { .offset = 793.0f * ATLAS_PIXEL_W, .atlas_width  = 3.0f * ATLAS_PIXEL_W, .game_width  = 3.0f * UNIT_ONE_PIXEL }, /* } */
    { .offset = 800.0f * ATLAS_PIXEL_W, .atlas_width  = 5.0f * ATLAS_PIXEL_W, .game_width  = 5.0f * UNIT_ONE_PIXEL }, /* ~ */
};

#endif/*__ATLAS_H__*/
