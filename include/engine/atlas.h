#ifndef __ATLAS_H__
#define __ATLAS_H__

#include "engine/math.h"
#include "engine/sprites.h"

#define ATLAS_WIDTH 512
#define ATLAS_HEIGHT 32
#define ATLAS_PIXEL_W (1.0f/512)
#define ATLAS_PIXEL_H (1.0f/32)

static const uint8_t g_atlas_data[ATLAS_WIDTH*ATLAS_HEIGHT*4] = {
#embed "imgs/atlas.bin"
};

static const struct v2 g_atlas_sprite_positions[SPRITES_AMOUNT] = {
    { 0.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
    { 1.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
    { 43.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
    { 469.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
    { 0.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_sizes[SPRITES_AMOUNT] = {
    { 1.0f * ATLAS_PIXEL_W, 1.0f * ATLAS_PIXEL_H },
    { 42.0f * ATLAS_PIXEL_W, 7.0f * ATLAS_PIXEL_H },
    { 426.0f * ATLAS_PIXEL_W, 12.0f * ATLAS_PIXEL_H },
    { 32.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
    { 16.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_half_sizes[SPRITES_AMOUNT] = {
    { 0.5f * UNIT_ONE_PIXEL, 0.5f * UNIT_ONE_PIXEL },
    { 21.0f * UNIT_ONE_PIXEL, 3.5f * UNIT_ONE_PIXEL },
    { 213.0f * UNIT_ONE_PIXEL, 6.0f * UNIT_ONE_PIXEL },
    { 16.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
    { 8.0f * UNIT_ONE_PIXEL, 8.0f * UNIT_ONE_PIXEL },
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
    float width;
} g_glyphs['~'-' '+1] = {
    { .offset = 0.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /*   */
    { .offset = 5.0f * ATLAS_PIXEL_W, .width  = 1.0f * ATLAS_PIXEL_W }, /* ! */
    { .offset = 6.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* " */
    { .offset = 10.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* # */
    { .offset = 15.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* $ */
    { .offset = 20.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* % */
    { .offset = 25.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* & */
    { .offset = 30.0f * ATLAS_PIXEL_W, .width  = 1.0f * ATLAS_PIXEL_W }, /* ' */
    { .offset = 31.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* ( */
    { .offset = 34.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* ) */
    { .offset = 37.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* * */
    { .offset = 42.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* + */
    { .offset = 47.0f * ATLAS_PIXEL_W, .width  = 2.0f * ATLAS_PIXEL_W }, /* , */
    { .offset = 49.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* - */
    { .offset = 54.0f * ATLAS_PIXEL_W, .width  = 1.0f * ATLAS_PIXEL_W }, /* . */
    { .offset = 55.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* / */
    { .offset = 60.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 0 */
    { .offset = 65.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* 1 */
    { .offset = 68.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 2 */
    { .offset = 73.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 3 */
    { .offset = 78.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 4 */
    { .offset = 83.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 5 */
    { .offset = 88.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 6 */
    { .offset = 93.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 7 */
    { .offset = 98.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 8 */
    { .offset = 103.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* 9 */
    { .offset = 108.0f * ATLAS_PIXEL_W, .width  = 1.0f * ATLAS_PIXEL_W }, /* : */
    { .offset = 109.0f * ATLAS_PIXEL_W, .width  = 2.0f * ATLAS_PIXEL_W }, /* ; */
    { .offset = 111.0f * ATLAS_PIXEL_W, .width  = 6.0f * ATLAS_PIXEL_W }, /* < */
    { .offset = 117.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* = */
    { .offset = 122.0f * ATLAS_PIXEL_W, .width  = 6.0f * ATLAS_PIXEL_W }, /* > */
    { .offset = 128.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* ? */
    { .offset = 132.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* @ */
    { .offset = 137.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* A */
    { .offset = 142.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* B */
    { .offset = 147.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* C */
    { .offset = 152.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* D */
    { .offset = 157.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* E */
    { .offset = 162.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* F */
    { .offset = 167.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* G */
    { .offset = 172.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* H */
    { .offset = 177.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* I */
    { .offset = 180.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* J */
    { .offset = 185.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* K */
    { .offset = 189.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* L */
    { .offset = 194.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* M */
    { .offset = 199.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* N */
    { .offset = 204.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* O */
    { .offset = 209.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* P */
    { .offset = 214.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* Q */
    { .offset = 219.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* R */
    { .offset = 224.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* S */
    { .offset = 229.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* T */
    { .offset = 234.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* U */
    { .offset = 239.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* V */
    { .offset = 244.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* W */
    { .offset = 249.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* X */
    { .offset = 254.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* Y */
    { .offset = 259.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* Z */
    { .offset = 264.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* [ */
    { .offset = 267.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* \ */
    { .offset = 272.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* ] */
    { .offset = 275.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* ^ */
    { .offset = 280.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* _ */
    { .offset = 285.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* ` */
    { .offset = 288.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* a */
    { .offset = 293.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* b */
    { .offset = 298.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* c */
    { .offset = 303.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* d */
    { .offset = 308.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* e */
    { .offset = 313.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* f */
    { .offset = 317.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* g */
    { .offset = 322.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* h */
    { .offset = 327.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* i */
    { .offset = 330.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* j */
    { .offset = 334.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* k */
    { .offset = 338.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* l */
    { .offset = 342.0f * ATLAS_PIXEL_W, .width  = 7.0f * ATLAS_PIXEL_W }, /* m */
    { .offset = 349.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* n */
    { .offset = 354.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* o */
    { .offset = 359.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* p */
    { .offset = 363.0f * ATLAS_PIXEL_W, .width  = 4.0f * ATLAS_PIXEL_W }, /* q */
    { .offset = 367.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* r */
    { .offset = 372.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* s */
    { .offset = 377.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* t */
    { .offset = 382.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* u */
    { .offset = 387.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* v */
    { .offset = 392.0f * ATLAS_PIXEL_W, .width  = 7.0f * ATLAS_PIXEL_W }, /* w */
    { .offset = 399.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* x */
    { .offset = 404.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* y */
    { .offset = 409.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* z */
    { .offset = 414.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* { */
    { .offset = 417.0f * ATLAS_PIXEL_W, .width  = 1.0f * ATLAS_PIXEL_W }, /* | */
    { .offset = 418.0f * ATLAS_PIXEL_W, .width  = 3.0f * ATLAS_PIXEL_W }, /* } */
    { .offset = 421.0f * ATLAS_PIXEL_W, .width  = 5.0f * ATLAS_PIXEL_W }, /* ~ */
};

#endif/*__ATLAS_H__*/
