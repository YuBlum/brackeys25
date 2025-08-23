#if DEV
#  include <assert.h>
#endif
#include <stdlib.h>
#if WASM
#  include <GLES3/gl3.h>
#else
#  include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include "engine/core.h"
#include "engine/string.h"
#include "engine/shaders.h"
#include "engine/renderer.h"
#include "engine/atlas.h"

struct vertex {
  struct v2    position;
  struct v2    texcoord;
  struct v2    origin;
  struct v2    angle;
  struct color color;
  float        opacity;
};

#define QUAD_CAPACITY 100000
#define INDEX_CAPACITY (QUAD_CAPACITY*6)
#define VERTEX_CAPACITY (QUAD_CAPACITY*4)

struct quad_vertices {
  struct vertex v[4];
};

struct quad_indices {
  uint32_t i[6];
};

struct index_sort {
  uint32_t start;
  float depth;
};

struct renderer {
  struct quad_vertices vertices[QUAD_CAPACITY];
  struct quad_indices indices[QUAD_CAPACITY];
  struct index_sort indices_to_sort[QUAD_CAPACITY];
  struct quad_vertices vertices_circle[QUAD_CAPACITY];
  struct quad_indices indices_circle[QUAD_CAPACITY];
  uint32_t circles_amount;
  struct v2 offset;
  uint32_t quads_amount;
  uint32_t sh_default;
  int32_t  sh_default_proj;
  uint32_t texture_atlas;
  uint32_t sh_circle;
  int32_t  sh_circle_proj;
};

static struct renderer g_renderer;
static float g_projection[3*3] = {
  +2.0f/(GAME_RIGHT-GAME_LEFT)                  , +0.0f                                         , +0.0f,
  +0.0f                                         , +2.0f/(GAME_TOP-GAME_BOTTOM)                  , +0.0f,
  -(GAME_RIGHT+GAME_LEFT)/(GAME_RIGHT-GAME_LEFT), -(GAME_TOP+GAME_BOTTOM)/(GAME_TOP-GAME_BOTTOM), +1.0f,
};

#define SHADER_LOG_CAPACITY 512

static uint32_t
shader_make(GLenum type, const struct str_view *src) {
#if DEV
  const char *shader_str = 0;
  switch (type) {
    case GL_VERTEX_SHADER: {
      shader_str = "vertex";
    } break;
    case GL_FRAGMENT_SHADER: {
      shader_str = "fragment";
    } break;
    default: {
      log_errorlf("%s: unknown shader type", __func__);
      return 0;
    } break;
  }
#endif
  uint32_t shader = glCreateShader(type);
  if (!shader) {
    log_errorlf("%s: couldn't make shader", __func__);
    return 0;
  }
  const int len = src->length;
  glShaderSource(shader, 1, &src->data, &len);
  glCompileShader(shader);
#if DEV
  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    int log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > SHADER_LOG_CAPACITY) {
      log_warnlf("%s: shader compile error will not be fully displayed. increase SHADER_LOG_CAPACITY", __func__);
    }
    char log[SHADER_LOG_CAPACITY];
    glGetShaderInfoLog(shader, SHADER_LOG_CAPACITY, 0, log);
    log_errorf("%s shader: %s", shader_str, log);
    glDeleteShader(shader);
    return 0;
  }
#endif
  return shader;
}

static uint32_t
shader_program_make(const struct str_view *vert_src, const struct str_view *frag_src) {
  log_infolf("%s: starting shader program creation...", __func__);
  uint32_t program = glCreateProgram();
  if (!program) {
    log_errorlf("%s: couldn't make shader program", __func__);
    return 0;
  }
  uint32_t vert = shader_make(GL_VERTEX_SHADER, vert_src);
  if (!vert) return 0;
  log_infolf("%s: compiled vertex shader", __func__);
  uint32_t frag = shader_make(GL_FRAGMENT_SHADER, frag_src);
  if (!frag) return 0;
  log_infolf("%s: compiled fragment shader", __func__);
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  glDeleteShader(vert);
  glDeleteShader(frag);
#if DEV
  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    int log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > SHADER_LOG_CAPACITY) {
      log_warnlf("%s: shader linking error will not be fully displayed. increase SHADER_LOG_CAPACITY", __func__);
    }
    char log[SHADER_LOG_CAPACITY];
    glGetProgramInfoLog(program, SHADER_LOG_CAPACITY, 0, log);
    log_errorf("shader program linking: %s", log);
    glDeleteProgram(program);
    return 0;
  }
#endif
  return program;
}

bool
renderer_make(void) {
  log_infol("making renderer...");
#ifndef WASM
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
  log_infol("loaded opengl functions");
  g_renderer.sh_default = shader_program_make(&SH_DEFAULT_VERT, &SH_DEFAULT_FRAG);
  if (!g_renderer.sh_default) {
    log_errorl("couldn't create default shader");
    return false;
  }
  g_renderer.sh_default_proj = glGetUniformLocation(g_renderer.sh_default, "u_proj");
#if DEV
  if (g_renderer.sh_default_proj < 0) {
    log_errorl("couldn't get 'u_proj' location from default shader");
    return false;
  }
#endif
  glUseProgram(g_renderer.sh_default);
  glUniformMatrix3fv(g_renderer.sh_default_proj, 1, false, g_projection);
  log_infol("created default shader");
  g_renderer.sh_circle = shader_program_make(&SH_CIRCLE_VERT, &SH_CIRCLE_FRAG);
  if (!g_renderer.sh_circle) {
    log_errorl("couldn't create circle shader");
    return false;
  }
  g_renderer.sh_circle_proj = glGetUniformLocation(g_renderer.sh_circle, "u_proj");
#if DEV
  if (g_renderer.sh_circle_proj < 0) {
    log_errorl("couldn't get 'u_proj' location from circle shader");
    return false;
  }
#endif
  glUseProgram(g_renderer.sh_circle);
  glUniformMatrix3fv(g_renderer.sh_circle_proj, 1, false, g_projection);
  for (uint32_t i = 0; i < QUAD_CAPACITY; i++) {
    g_renderer.indices_circle[i].i[0] = (i * 4) + 0;
    g_renderer.indices_circle[i].i[1] = (i * 4) + 1;
    g_renderer.indices_circle[i].i[2] = (i * 4) + 2;
    g_renderer.indices_circle[i].i[3] = (i * 4) + 2;
    g_renderer.indices_circle[i].i[4] = (i * 4) + 3;
    g_renderer.indices_circle[i].i[5] = (i * 4) + 0;
  }
  log_infol("created circle shader");
  glGenTextures(1, &g_renderer.texture_atlas);
  glBindTexture(GL_TEXTURE_2D, g_renderer.texture_atlas);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_WIDTH, ATLAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, g_atlas_data);
  log_infol("loaded texture atlas");
  uint32_t vao, vbo, ibo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof (g_renderer.vertices), 0, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (g_renderer.indices), 0, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, texcoord));
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, origin));
  glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, angle));
  glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, color));
  glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof (struct vertex), (void *)offsetof (struct vertex, opacity));
  log_infol("vao, vbo and ibo created successfully");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  g_renderer.offset = V2S(0.0f);
  log_infol("renderer creation complete!");
  return true;
}

struct v2
renderer_get_offset(void) {
  return g_renderer.offset;
}

void
renderer_set_offset(struct v2 offset) {
  g_renderer.offset = offset;
}

static int
compare_indices(const void *r0, const void *r1) {
  float d0 = ((const struct index_sort *)r0)->depth;
  float d1 = ((const struct index_sort *)r1)->depth;
  if (d1 < d0) return -1;
  if (d1 > d0) return +1;
  return 0;
}

void
renderer_submit(void) {
  // NOTE: maybe instead of sorting and resetting the batch every frame you can have 'permanent' quads
  qsort(g_renderer.indices_to_sort, g_renderer.quads_amount, sizeof (struct index_sort), compare_indices);
  for (uint32_t i = 0; i < g_renderer.quads_amount; i++) {
    g_renderer.indices[i].i[0] = g_renderer.indices_to_sort[i].start + 0;
    g_renderer.indices[i].i[1] = g_renderer.indices_to_sort[i].start + 1;
    g_renderer.indices[i].i[2] = g_renderer.indices_to_sort[i].start + 2;
    g_renderer.indices[i].i[3] = g_renderer.indices_to_sort[i].start + 2;
    g_renderer.indices[i].i[4] = g_renderer.indices_to_sort[i].start + 3;
    g_renderer.indices[i].i[5] = g_renderer.indices_to_sort[i].start + 0;
  }
  for (uint32_t i = 0; i < g_renderer.quads_amount; i++) {
    g_renderer.vertices[i].v[0].position = v2_sub(g_renderer.vertices[i].v[0].position, g_renderer.offset);
    g_renderer.vertices[i].v[1].position = v2_sub(g_renderer.vertices[i].v[1].position, g_renderer.offset);
    g_renderer.vertices[i].v[2].position = v2_sub(g_renderer.vertices[i].v[2].position, g_renderer.offset);
    g_renderer.vertices[i].v[3].position = v2_sub(g_renderer.vertices[i].v[3].position, g_renderer.offset);
  }
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(g_renderer.sh_default);
  glBufferSubData(GL_ARRAY_BUFFER, 0, g_renderer.quads_amount * sizeof (struct quad_vertices), g_renderer.vertices);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, g_renderer.quads_amount * sizeof (struct quad_indices), g_renderer.indices);
  glDrawElements(GL_TRIANGLES, g_renderer.quads_amount * 6, GL_UNSIGNED_INT, 0);
  g_renderer.quads_amount = 0;
  glUseProgram(g_renderer.sh_circle);
  glBufferSubData(GL_ARRAY_BUFFER, 0, g_renderer.circles_amount * sizeof (struct quad_vertices), g_renderer.vertices_circle);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, g_renderer.circles_amount * sizeof (struct quad_indices), g_renderer.indices_circle);
  glDrawElements(GL_TRIANGLES, g_renderer.circles_amount * 6, GL_UNSIGNED_INT, 0);
  g_renderer.circles_amount = 0;
}

static struct animation_data g_animation_stub;

const struct animation_data *
renderer_animation_get_data(enum animation animation) {
  if (animation >= ANIMATIONS_AMOUNT) {
    g_animation_stub = (struct animation_data) { 0 };
    return &g_animation_stub;
  }
  return &g_atlas_animations[animation];
}

#define TEXEL_HALF_W (ATLAS_PIXEL_W*0.5f)
#define TEXEL_HALF_H (ATLAS_PIXEL_H*0.5f)

static void
renderer_request_sprite_internal(enum sprite sprite, struct v2 position, struct v2 hsiz, struct v2 tpos, struct v2 tsiz, struct renderer_params p) {
#if DEV
  if (g_renderer.quads_amount + 1 >= QUAD_CAPACITY) {
    log_warnlf("%s: trying to request to much quads for rendering. increase QUAD_CAPACITY", __func__);
    return;
  }
  if (sprite >= SPRITES_AMOUNT) {
    log_warnlf("%s: sprite with number '%d' doesn't exists", __func__, sprite);
    return;
  }
#endif
  static_assert(sizeof (struct vertex) == sizeof (float) * 12);
  struct v2 cos_sin = { cosf(p.angle), sinf(p.angle) };
  struct vertex *vertices = g_renderer.vertices[g_renderer.quads_amount].v;
  vertices[0].position = position;
  vertices[1].position = position;
  vertices[2].position = position;
  vertices[3].position = position;
  /* Note for the future: the texcoords needs to be offseted by half a texel, this is to prevent random sprite leakage into the current sprite.
   *                      Its also possible to add some padding into the sprite to also prevent this problem, the padding isn't just some blank
   *                      padding but a copy of the edges of the sprite. usually 2 pixels of padding is preferable. the padding is also required
   *                      if using mipmaps or linear filtering, that's not the case here though so this is fine.
   */
  vertices[0].texcoord = v2_add(tpos, V2(       TEXEL_HALF_W, tsiz.y-TEXEL_HALF_H));
  vertices[1].texcoord = v2_add(tpos, V2(tsiz.x-TEXEL_HALF_W, tsiz.y-TEXEL_HALF_H));
  vertices[2].texcoord = v2_add(tpos, V2(tsiz.x-TEXEL_HALF_W,        TEXEL_HALF_H));
  vertices[3].texcoord = v2_add(tpos, V2(       TEXEL_HALF_W,        TEXEL_HALF_H));
  vertices[0].origin = v2_mul(v2_sub(V2(-hsiz.x, -hsiz.y), p.origin), p.scale);
  vertices[1].origin = v2_mul(v2_sub(V2(+hsiz.x, -hsiz.y), p.origin), p.scale);
  vertices[2].origin = v2_mul(v2_sub(V2(+hsiz.x, +hsiz.y), p.origin), p.scale);
  vertices[3].origin = v2_mul(v2_sub(V2(-hsiz.x, +hsiz.y), p.origin), p.scale);
  vertices[0].angle = cos_sin;
  vertices[1].angle = cos_sin;
  vertices[2].angle = cos_sin;
  vertices[3].angle = cos_sin;
  vertices[0].color = p.color;
  vertices[1].color = p.color;
  vertices[2].color = p.color;
  vertices[3].color = p.color;
  vertices[0].opacity = p.opacity;
  vertices[1].opacity = p.opacity;
  vertices[2].opacity = p.opacity;
  vertices[3].opacity = p.opacity;
  g_renderer.indices_to_sort[g_renderer.quads_amount].depth = p.depth;
  g_renderer.indices_to_sort[g_renderer.quads_amount].start = g_renderer.quads_amount * 4;
  g_renderer.quads_amount++;
}

void
_renderer_request_sprite_slice(enum sprite sprite, struct v2 position, struct v2 top_left, struct v2 size, struct renderer_params p) {
  struct v2 hsiz = V2(size.x * ATLAS_WIDTH * 0.5f * UNIT_ONE_PIXEL, size.y * ATLAS_HEIGHT * 0.5f * UNIT_ONE_PIXEL),
            tpos = v2_add(g_atlas_sprite_positions[sprite], top_left),
            tsiz = size;
  renderer_request_sprite_internal(sprite, position, hsiz, tpos, tsiz, p);
}

void
_renderer_request_sprite(enum sprite sprite, struct v2 position, struct renderer_params p) {
  struct v2 hsiz = g_atlas_sprite_half_sizes[sprite],
            tpos = g_atlas_sprite_positions[sprite],
            tsiz = g_atlas_sprite_sizes[sprite];
  renderer_request_sprite_internal(sprite, position, hsiz, tpos, tsiz, p);
}

void
_renderer_request_animation(enum animation animation, uint32_t frame, struct v2 position, struct renderer_params p) {
  #if DEV
  if (animation >= ANIMATIONS_AMOUNT) {
    log_warnlf("%s: animation with index '%u' doesn't exists", __func__, animation);
    return;
  }
  if (frame >= g_atlas_animations[animation].frames_amount) {
    log_warnlf("%s: maximum frame of animation is '%u', but requested frame '%u'", __func__, g_atlas_animations[animation].frames_amount, frame);
    return;
  }
  #endif
  auto sprite = g_atlas_animations[animation].sprite;
  struct v2 size     = { g_atlas_animations[animation].frame_width, g_atlas_sprite_sizes[sprite].y },
            top_left = { size.x * (frame + g_atlas_animations[animation].first_frame), 0.0f };
  _renderer_request_sprite_slice(sprite, position, top_left, size, p);
}

void
renderer_request_circle(struct v2 position, float radius, struct color color, float opacity) {
  if (g_renderer.circles_amount + 1 >= QUAD_CAPACITY) {
    log_warnlf("%s: trying to request to much circles for rendering. increase QUAD_CAPACITY", __func__);
    return;
  }
  static_assert(sizeof (struct vertex) == sizeof (float) * 12);
  struct vertex *vertices = g_renderer.vertices_circle[g_renderer.circles_amount].v;
  vertices[0].position = v2_add(position, V2(-radius, -radius));
  vertices[1].position = v2_add(position, V2(+radius, -radius));
  vertices[2].position = v2_add(position, V2(+radius, +radius));
  vertices[3].position = v2_add(position, V2(-radius, +radius));
  vertices[0].origin = V2(-1.0f, -1.0f); /* treating the 'origin' like normalized coordinates */
  vertices[1].origin = V2(+1.0f, -1.0f); /* treating the 'origin' like normalized coordinates */
  vertices[2].origin = V2(+1.0f, +1.0f); /* treating the 'origin' like normalized coordinates */
  vertices[3].origin = V2(-1.0f, +1.0f); /* treating the 'origin' like normalized coordinates */
  vertices[0].color = color;
  vertices[1].color = color;
  vertices[2].color = color;
  vertices[3].color = color;
  vertices[0].opacity = opacity;
  vertices[1].opacity = opacity;
  vertices[2].opacity = opacity;
  vertices[3].opacity = opacity;
  g_renderer.circles_amount++;
}

void
renderer_request_rect(struct v2 position, struct v2 size, struct color color, float opacity, float depth) {
  renderer_request_sprite(
    SPR_PIXEL,
    position,
    .scale   = v2_mul(size, V2(UNIT_PER_PIXEL, UNIT_PER_PIXEL)),
    .color   = color,
    .opacity = opacity,
    .depth   = depth
  );
}

void
renderer_request_line(struct v2 p0, struct v2 p1, float thickness, struct color color, float opacity, float depth) {
  auto position = v2_lerp(p0, p1, 0.5f);
  renderer_request_sprite(
    SPR_PIXEL,
    position,
    .scale   = { v2_dist(p0, p1) * UNIT_PER_PIXEL, thickness * UNIT_PER_PIXEL },
    .angle   = v2_to_angle2(p0, p1),
    .color   = color,
    .opacity = opacity,
    .depth   = depth
  );
}

void
renderer_request_point(struct v2 position, struct color color, float opacity, float depth) {
  renderer_request_sprite(
    SPR_PIXEL,
    position,
    .color   = color,
    .opacity = opacity,
    .depth   = depth
  );
}
