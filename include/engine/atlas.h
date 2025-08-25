#ifndef __ATLAS_H__
#define __ATLAS_H__

#include "engine/math.h"
#include "engine/sprites.h"

#define ATLAS_WIDTH 64
#define ATLAS_HEIGHT 32
#define ATLAS_PIXEL_W (1.0f/64)
#define ATLAS_PIXEL_H (1.0f/32)

static const uint8_t g_atlas_data[ATLAS_WIDTH*ATLAS_HEIGHT*4] = {
#embed "imgs/atlas.bin"
};

static const struct v2 g_atlas_sprite_positions[SPRITES_AMOUNT] = {
  { 0.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
  { 1.0f * ATLAS_PIXEL_W, 0.0f * ATLAS_PIXEL_H },
  { 0.0f * ATLAS_PIXEL_W, 7.0f * ATLAS_PIXEL_H },
  { 32.0f * ATLAS_PIXEL_W, 7.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_sizes[SPRITES_AMOUNT] = {
  { 1.0f * ATLAS_PIXEL_W, 1.0f * ATLAS_PIXEL_H },
  { 42.0f * ATLAS_PIXEL_W, 7.0f * ATLAS_PIXEL_H },
  { 32.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
  { 16.0f * ATLAS_PIXEL_W, 16.0f * ATLAS_PIXEL_H },
};

static const struct v2 g_atlas_sprite_half_sizes[SPRITES_AMOUNT] = {
  { 0.5f * UNIT_ONE_PIXEL, 0.5f * UNIT_ONE_PIXEL },
  { 21.0f * UNIT_ONE_PIXEL, 3.5f * UNIT_ONE_PIXEL },
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

#endif/*__ATLAS_H__*/
