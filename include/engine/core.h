#ifndef __CORE_H__
#define __CORE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if DEV
#  include <stdio.h>
#  define log_infol(msg) (void)fprintf(stderr, "\x1b[1;32minfo: " msg "\x1b[0m\n")
#  define log_warnl(msg) (void)fprintf(stderr, "\x1b[1;33mwarn: " msg "\x1b[0m\n")
#  define log_errorl(msg) (void)fprintf(stderr, "\x1b[1;31merror: " msg "\x1b[0m\n")
#  define log_infolf(fmt, ...) (void)fprintf(stderr, "\x1b[1;32minfo: " fmt "\x1b[0m\n", __VA_ARGS__)
#  define log_warnlf(fmt, ...) (void)fprintf(stderr, "\x1b[1;33mwarn: " fmt "\x1b[0m\n", __VA_ARGS__)
#  define log_errorlf(fmt, ...) (void)fprintf(stderr, "\x1b[1;31merror: " fmt "\x1b[0m\n", __VA_ARGS__)
#  define log_info(msg) (void)fprintf(stderr, "\x1b[1;32minfo: " msg "\x1b[0m")
#  define log_warn(msg) (void)fprintf(stderr, "\x1b[1;33mwarn: " msg "\x1b[0m")
#  define log_error(msg) (void)fprintf(stderr, "\x1b[1;31merror: " msg "\x1b[0m")
#  define log_infof(fmt, ...) (void)fprintf(stderr, "\x1b[1;32minfo: " fmt "\x1b[0m", __VA_ARGS__)
#  define log_warnf(fmt, ...) (void)fprintf(stderr, "\x1b[1;33mwarn: " fmt "\x1b[0m", __VA_ARGS__)
#  define log_errorf(fmt, ...) (void)fprintf(stderr, "\x1b[1;31merror: " fmt "\x1b[0m", __VA_ARGS__)
#else
#  define log_infol(msg)
#  define log_warnl(msg)
#  define log_errorl(msg)
#  define log_infolf(fmt, ...)
#  define log_warnlf(fmt, ...)
#  define log_errorlf(fmt, ...)
#  define log_info(msg)
#  define log_warn(msg)
#  define log_error(msg)
#  define log_infof(fmt, ...)
#  define log_warnf(fmt, ...)
#  define log_errorf(fmt, ...)
#endif

#define UNIT_PER_PIXEL 16
#define UNIT_ONE_PIXEL (1.0f/(float)UNIT_PER_PIXEL)
#define GAME_W_PIXEL 320
#define GAME_H_PIXEL 240
#define GAME_S 3
#define GAME_W (GAME_W_PIXEL/(float)UNIT_PER_PIXEL)
#define GAME_H (GAME_H_PIXEL/(float)UNIT_PER_PIXEL)
#define GAME_LEFT   (-GAME_W * 0.5f)
#define GAME_RIGHT  (+GAME_W * 0.5f)
#define GAME_BOTTOM (-GAME_H * 0.5f)
#define GAME_TOP    (+GAME_H * 0.5f)

#define TILES_AMOUNT (((int)GAME_W)*((int)GAME_H))

static_assert((GAME_W_PIXEL % UNIT_PER_PIXEL) == 0, "GAME_W_PIXEL has to be a multiple of UNIT_PER_PIXEL");
static_assert((GAME_H_PIXEL % UNIT_PER_PIXEL) == 0, "GAME_H_PIXEL has to be a multiple of UNIT_PER_PIXEL");

#define WINDOW_W (GAME_S*GAME_W_PIXEL)
#define WINDOW_H (GAME_S*GAME_H_PIXEL)
#define WINDOW_TITLE "game"

#endif/*__CORE_H__*/
