#ifndef __SHADERS_H__
#define __SHADERS_H__

#if WASM
#define GLSL_VERSION "#version 300 es\n"
#else
#define GLSL_VERSION "#version 330 core\n"
#endif

#define SH_DEFAULT_VERT str_view_make_from_lit( \
GLSL_VERSION \
"layout (location=0) in vec2  a_position;\n" \
"layout (location=1) in vec2  a_texcoord;\n" \
"layout (location=2) in vec2  a_origin;\n" \
"layout (location=3) in vec2  a_angle;\n" \
"layout (location=4) in vec3  a_color;\n" \
"layout (location=5) in float a_opacity;\n" \
"\n" \
"out vec2 v_texcoord;\n" \
"out vec4 v_blend;\n" \
"\n" \
"uniform mat3 u_proj;\n" \
"\n" \
"void\n" \
"main() {\n" \
"  mat2 transform = mat2(\n" \
"    a_angle.x, -a_angle.y,\n" \
"    a_angle.y,  a_angle.x\n" \
"  );\n" \
"  gl_Position = vec4(u_proj * vec3((transform * a_origin) + a_position, 0.0), 1.0);\n" \
"  v_texcoord = a_texcoord;\n" \
"  v_blend = vec4(a_color, a_opacity);\n" \
"}\n")

#define SH_DEFAULT_FRAG str_view_make_from_lit( \
GLSL_VERSION \
"precision mediump float;\n" \
"in vec2 v_texcoord;\n" \
"in vec4 v_blend;\n" \
"\n" \
"out vec4 f_color;\n" \
"\n" \
"uniform sampler2D texture_atlas;\n" \
"\n" \
"void\n" \
"main() {\n" \
"  f_color = texture(texture_atlas, v_texcoord) * v_blend;\n" \
"}\n")

#if DEV
#define SH_CIRCLE_VERT str_view_make_from_lit( \
GLSL_VERSION \
"layout (location=0) in vec2  a_position;\n" \
"layout (location=2) in vec2  a_coord;\n" \
"layout (location=4) in vec3  a_color;\n" \
"layout (location=5) in float a_opacity;\n" \
"\n" \
"out vec4 v_blend;\n" \
"out vec2 v_coord;\n" \
"\n" \
"uniform mat3 u_proj;\n" \
"\n" \
"void\n" \
"main() {\n" \
"  gl_Position = vec4(u_proj * vec3(a_position, 0.0), 1.0);\n" \
"  v_blend = vec4(a_color, a_opacity);\n" \
"  v_coord = a_coord;\n" \
"}\n")

#define SH_CIRCLE_FRAG str_view_make_from_lit( \
GLSL_VERSION \
"precision mediump float;\n" \
"in vec4 v_blend;\n" \
"in vec2 v_coord;\n" \
"\n" \
"out vec4 f_color;\n" \
"\n" \
"void\n" \
"main() {\n" \
"  f_color = v_blend;\n" \
"  f_color *= float(1.0 - distance(v_coord, vec2(0.0, 0.0)) > 0.0);\n" \
"}\n")
#endif

#endif/*__SHADERS_H__*/
