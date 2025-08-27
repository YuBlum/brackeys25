#ifndef __YMATH_H__
#define __YMATH_H__

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159265358979323846
#define RAD2DEG (180.0f/PI)
#define DEG2RAD (PI/180.0f)

#define swap(a, b) do { \
    typeof(a) __tmp__ = a; \
    a = b; \
    b = __tmp__; \
} while(0)

static inline float
randf() {
    return (float)rand()/((float)RAND_MAX + 1.0f);
}

static inline float
randf_from_to(float from, float to) {
    return randf() * (to - from) + from;
}

static inline uint32_t
rand_from_to(uint32_t from, uint32_t to) {
    return to == from ? to : (uint32_t)(randf() * (to + 1 - from) + from);
}

struct v2 { float x, y; };
#define V2(x, y) ((struct v2) {x, y})
#define V2S(x) ((struct v2) {x, x})

static inline struct v2 v2_add(struct v2 v0, struct v2 v1) { return V2(v0.x+v1.x, v0.y+v1.y); }
static inline struct v2 v2_sub(struct v2 v0, struct v2 v1) { return V2(v0.x-v1.x, v0.y-v1.y); }
static inline struct v2 v2_mul(struct v2 v0, struct v2 v1) { return V2(v0.x*v1.x, v0.y*v1.y); }
static inline struct v2 v2_div(struct v2 v0, struct v2 v1) { return V2(v0.x/v1.x, v0.y/v1.y); }

static inline struct v2 v2_adds(struct v2 v, float s) { return V2(v.x+s, v.y+s); }
static inline struct v2 v2_subs(struct v2 v, float s) { return V2(v.x-s, v.y-s); }
static inline struct v2 v2_muls(struct v2 v, float s) { return V2(v.x*s, v.y*s); }
static inline struct v2 v2_divs(struct v2 v, float s) { return V2(v.x/s, v.y/s); }

static inline float v2_dot(struct v2 v0, struct v2 v1) { return v0.x*v1.x + v0.y*v1.y; }
static inline float v2_magsq(struct v2 v) { return v.x*v.x + v.y*v.y; }
static inline float v2_mag(struct v2 v) { return sqrt(v.x*v.x + v.y*v.y); }
static inline struct v2 v2_unit(struct v2 v) { float mag = sqrt(v.x*v.x + v.y*v.y); return mag == 0.0f ? V2S(0.0f) : V2(v.x/mag, v.y/mag); }
static inline float v2_distance(struct v2 v0, struct v2 v1) { return v2_mag(v2_sub(v1, v0)); }
static inline float v2_to_angle(struct v2 v) { return atan2f(-v.y, v.x); }
static inline float v2_to_angle2(struct v2 v0, struct v2 v1) { return atan2f(v0.y - v1.y, v1.x - v0.x); }
static inline struct v2 v2_from_angle(float a) { return V2(cosf(a), -sinf(a)); }
static inline struct v2 v2_direction(struct v2 v0, struct v2 v1) { return v2_unit(v2_sub(v1, v0)); }

struct v2u { uint32_t x, y; };
#define V2U(x, y) ((struct v2u) {x, y})
#define V2US(x) ((struct v2u) {x, x})
#define V2U_V2(v) V2(v.x, v.y)

static inline struct v2u v2u_add(struct v2u v0, struct v2u v1) { return V2U(v0.x+v1.x, v0.y+v1.y); }
static inline struct v2u v2u_sub(struct v2u v0, struct v2u v1) { return V2U(v0.x-v1.x, v0.y-v1.y); }
static inline struct v2u v2u_mul(struct v2u v0, struct v2u v1) { return V2U(v0.x*v1.x, v0.y*v1.y); }
static inline struct v2u v2u_div(struct v2u v0, struct v2u v1) { return V2U(v0.x/v1.x, v0.y/v1.y); }

static inline struct v2u v2u_adds(struct v2u v, uint32_t s) { return V2U(v.x+s, v.y+s); }
static inline struct v2u v2u_subs(struct v2u v, uint32_t s) { return V2U(v.x-s, v.y-s); }
static inline struct v2u v2u_muls(struct v2u v, uint32_t s) { return V2U(v.x*s, v.y*s); }
static inline struct v2u v2u_divs(struct v2u v, uint32_t s) { return V2U(v.x/s, v.y/s); }

static inline float v2u_mag(struct v2u v) { return sqrt(v.x*v.x + v.y*v.y); }
static inline float v2u_dist(struct v2u v0, struct v2u v1) { return v2u_mag(v2u_sub(v1, v0)); }

static inline float      f32_max(float      x, float      y) { return x > y ? x : y; }
static inline int32_t    i32_max(int32_t    x, int32_t    y) { return x > y ? x : y; }
static inline uint32_t   u32_max(uint32_t   x, uint32_t   y) { return x > y ? x : y; }
static inline struct v2   v2_max(struct v2  x, struct v2  y) { return V2(f32_max(x.x, y.x), f32_max(x.y, y.y)); }
static inline struct v2u v2u_max(struct v2u x, struct v2u y) { return V2U(u32_max(x.x, y.x), u32_max(x.y, y.y)); }
static inline float      f32_min(float      x, float      y) { return x < y ? x : y; }
static inline int32_t    i32_min(int32_t    x, int32_t    y) { return x < y ? x : y; }
static inline uint32_t   u32_min(uint32_t   x, uint32_t   y) { return x < y ? x : y; }
static inline struct v2   v2_min(struct v2  x, struct v2  y) { return V2(f32_min(x.x, y.x), f32_min(x.y, y.y)); }
static inline struct v2u v2u_min(struct v2u x, struct v2u y) { return V2U(u32_min(x.x, y.x), u32_min(x.y, y.y)); }
static inline float      f32_clamp(float      x, float      mn, float      mx) { return f32_max(f32_min(x, mx), mn); }
static inline int32_t    i32_clamp(int32_t    x, int32_t    mn, int32_t    mx) { return i32_max(i32_min(x, mx), mn); }
static inline uint32_t   u32_clamp(uint32_t   x, uint32_t   mn, uint32_t   mx) { return u32_max(u32_min(x, mx), mn); }
static inline struct v2   v2_clamp(struct v2  x, struct v2  mn, struct v2  mx) { return v2_max(v2_min(x, mx), mn); }
static inline struct v2u v2u_clamp(struct v2u x, struct v2u mn, struct v2u mx) { return v2u_max(v2u_min(x, mx), mn); }
#define max(x, y) _Generic((x), \
    float:      f32_max, \
    int32_t:    i32_max, \
    uint32_t:   u32_max, \
    struct v2:   v2_max, \
    struct v2u: v2u_max  \
)(x, y)
#define min(x, y) _Generic((x), \
    float:      f32_min, \
    int32_t:    i32_min, \
    uint32_t:   u32_min, \
    struct v2:   v2_min, \
    struct v2u: v2u_min  \
)(x, y)
#define clamp(x, mn, mx) _Generic((x), \
    float:      f32_clamp, \
    int32_t:    i32_clamp, \
    uint32_t:   u32_clamp, \
    struct v2:   v2_clamp, \
    struct v2u: v2u_clamp  \
)(x, mn, mx)

static inline float f32_lerp(float a, float b, float t) { return a + t * (b - a); }
static inline float f32_lerp_smooth(float a, float b, float speed, float dt) { float t = 1.0f - pow(1.0f - speed, dt); return a + t * (b - a); }
static inline struct v2u v2u_lerp(struct v2u a, struct v2u b, float t) { return V2U((uint32_t)f32_lerp(a.x, b.x, t), (uint32_t)f32_lerp(a.y, b.y, t)); }
static inline struct v2 v2_lerp(struct v2 a, struct v2 b, float t) { return V2(f32_lerp(a.x, b.x, t), f32_lerp(a.y, b.y, t)); }
static inline struct v2u v2u_lerp_smooth(struct v2u a, struct v2u b, float speed, float dt) { return V2U((uint32_t)f32_lerp_smooth(a.x, b.x, speed, dt), (uint32_t)f32_lerp_smooth(a.y, b.y, speed, dt)); }
static inline struct v2 v2_lerp_smooth(struct v2 a, struct v2 b, float speed, float dt) { return V2(f32_lerp_smooth(a.x, b.x, speed, dt), f32_lerp_smooth(a.y, b.y, speed, dt)); }

#define lerp(a, b, t) _Generic((a), \
    struct v2:   v2_lerp, \
    struct v2u: v2u_lerp, \
    default:    f32_lerp  \
)(a, b, t)

#define lerp_smooth(a, b, speed, t) _Generic((a), \
    struct v2:   v2_lerp_smooth, \
    struct v2u: v2u_lerp_smooth, \
    default:    f32_lerp_smooth  \
)(a, b, speed, t)

static inline float     f32_sign(float     v) { return (v > 0.0f) - (v < 0.0f); }
static inline struct v2  v2_sign(struct v2 v) { return V2(f32_sign(v.x), f32_sign(v.y)); }
static inline float     f32_sign_nonzero(float     v) { return v > 0.0f ? 1.0f : -1.0f; }
static inline struct v2  v2_sign_nonzero(struct v2 v) { return V2(f32_sign_nonzero(v.x), f32_sign_nonzero(v.y)); }
#define sign(v) _Generic((v), \
    struct v2: v2_sign, \
    default:   f32_sign \
)(v)
#define sign_nonzero(v) _Generic((v), \
    struct v2: v2_sign_nonzero, \
    default:   f32_sign_nonzero \
)(v)

struct smoothstep_params {
    float a;
    float b;
};
static inline float _smoothstep(float x, struct smoothstep_params p) {
    x = clamp(p.a == p.b ? 0.0f : (x-p.a)/(p.b-p.a), 0.0f, 1.0f);
    return x * x * (3.0f-2.0f * x);
}
#define smoothstep(x, ...) _smoothstep(x, (struct smoothstep_params) { .a = 0.0f, .b = 1.0f })
static inline float ease_in_quad(float t) { t = clamp(t, 0.0f, 1.0f); return t * t; }
static inline float ease_in_cubic(float t) { t = clamp(t, 0.0f, 1.0f); return t * t * t; }
static inline float ease_in_power(float t, float power) { t = clamp(t, 0.0f, 1.0f); return powf(t, power); }
static inline float
ease_in_out_cubic(float t) {
    t = clamp(t, 0.0f, 1.0f);
    return t < 0.5f
        ? 4.0f * t * t * t
        : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}
static inline float
ease_out_back(float t) {
    t = clamp(t, 0.0f, 1.0f);
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return 1.0f + c3 * powf(t - 1.0f, 3) + c1 * powf(t - 1.0f, 2);
}

#endif/*__YMATH_H__*/
