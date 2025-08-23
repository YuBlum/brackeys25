#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "engine/core.h"
#include "engine/arena.h"

enum key : uint8_t {
  KEY_LEFT = 0,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_INTERACT,
  KEY_EXIT,
  KEY_DEBUG0,
  KEY_AMOUNT,
};

enum button : uint8_t {
  BTN_LEFT = 0,
  BTN_RIGHT,
  BUTTON_AMOUNT
};

bool window_make();
void window_destroy(void);
void window_close();
void window_run(struct arena *arena);
float window_frame_begin(void);
bool window_frame_end(void);
bool window_is_key_press(enum key key);
bool window_is_key_down(enum key key);
bool window_is_key_up(enum key key);
bool window_is_key_release(enum key key);
bool window_is_button_down(enum button btn);
bool window_is_button_up(enum button btn);
bool window_is_button_press(enum button btn);
bool window_is_button_release(enum button btn);
struct v2 window_get_cursor_position(void);

bool game_loop(struct arena *arena);

#endif/*__WINDOW_H__*/
