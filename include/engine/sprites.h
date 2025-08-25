#ifndef __SPRITES_H__
#define __SPRITES_H__

enum sprite {
    SPR_PLAYER = 0,
    SPR_PIXEL,
    SPR_AIM,
    SPR_FONT,
    SPR_TEST,
    SPR_PLAYER_TEST,
    SPRITES_AMOUNT
};

enum animation {
    ANIM_AIM_PRESSED = 0,
    ANIM_AIM_IDLE,
    ANIM_TEST_TEST,
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
