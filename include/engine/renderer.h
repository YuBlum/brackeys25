#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "engine/core.h"
#include "engine/arena.h"
#include "engine/math.h"
#include "engine/sprites.h"

struct color { float r, g, b; };
#define RGB(r, g, b) ((struct color) { r, g, b })
#define WHITE RGB(1.0f, 1.0f, 1.0f)
#define RED   RGB(1.0f, 0.0f, 0.0f)
#define GREEN RGB(0.0f, 1.0f, 0.0f)
#define BLUE  RGB(0.0f, 0.0f, 1.0f)
#define DARK_RED   RGB(0.5f, 0.0f, 0.0f)
#define DARK_GREEN RGB(0.0f, 0.5f, 0.0f)
#define DARK_BLUE  RGB(0.0f, 0.0f, 0.5f)
#define BLACK RGB(0.0f, 0.0f, 0.0f)
#define CORNFLOWER_BLUE RGB(0.392f, 0.584f, 0.929f)

bool renderer_make(void);
void renderer_submit(void);

struct v2 renderer_get_offset(void);
void renderer_set_offset(struct v2 offset);

const struct animation_data *renderer_animation_get_data(enum animation animation);

struct renderer_params {
  struct v2 origin;
  float angle;
  struct v2 scale;
  struct color color;
  float opacity;
  float depth;
};

void _renderer_request_sprite(enum sprite sprite, struct v2 position, struct renderer_params params);
#define renderer_request_sprite(sprite, position, ...) \
  _renderer_request_sprite(sprite, position, (struct renderer_params) { .scale = V2S(1.0f), .color = WHITE, .opacity = 1.0f, __VA_ARGS__ })

void _renderer_request_sprite_slice(enum sprite sprite, struct v2 position, struct v2 top_left, struct v2 size, struct renderer_params params);
#define renderer_request_sprite_slice(sprite, position, top_left, size, ...) \
  _renderer_request_sprite_slice(sprite, position, top_left, size, (struct renderer_params) { .scale = V2S(1.0f), .color = WHITE, .opacity = 1.0f, __VA_ARGS__ })

void _renderer_request_animation(enum animation animation, uint32_t frame, struct v2 position, struct renderer_params params);
#define renderer_request_animation(animation, frame, position, ...) \
  _renderer_request_animation(animation, frame, position, (struct renderer_params) { .scale = V2S(1.0f), .color = WHITE, .opacity = 1.0f, __VA_ARGS__ })

void renderer_request_circle(struct v2 position, float radius, struct color color, float opacity);
void renderer_request_rect(struct v2 position, struct v2 size, struct color color, float opacity, float depth);
void renderer_request_line(struct v2 p0, struct v2 p1, float thickness, struct color color, float opacity, float depth);
void renderer_request_point(struct v2 position, struct color color, float opacity, float depth);

#endif/*__RENDERER_H__*/
