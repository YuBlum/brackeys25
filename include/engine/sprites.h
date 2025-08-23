#ifndef __SPRITES_H__
#define __SPRITES_H__

enum sprite {
  SPR_PIXEL = 0,
  SPR_TEST,
  SPRITES_AMOUNT
};

enum animation {
  ANIM_TEST_TEST = 0,
  ANIMATIONS_AMOUNT
};

struct animation_data {
  const float *durations;
  float frame_width;
  uint32_t frames_amount;
  uint32_t first_frame;
  enum sprite sprite;
};

#endif/*__SPRITES_H__*/
