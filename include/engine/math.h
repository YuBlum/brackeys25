#ifndef __YMATH_H__
#define __YMATH_H__

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159265358979323846
#define RAD2DEG (180.0f/PI)
#define DEG2RAD (PI/180.0f)

static inline float lerp(float a, float b, float t) { return a + t * (b - a); }
static inline float signf(float x) { return (x > 0.0f) - (x < 0.0f); }
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define randf() ((float)rand()/(float)RAND_MAX)
#define randf_from_to(X, Y) (randf() * ((Y) - (X)) + (X))
#define rand_from_to(X, Y) (rand() % ((Y) - (X)) + (X))

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
static inline struct v2 v2_lerp(struct v2 a, struct v2 b, float t) { return V2(lerp(a.x, b.x, t), lerp(a.y, b.y, t)); }
static inline float v2_dist(struct v2 v0, struct v2 v1) { return v2_mag(v2_sub(v1, v0)); }
static inline float v2_to_angle(struct v2 v) { return atan2f(-v.y, v.x); }
static inline float v2_to_angle2(struct v2 v0, struct v2 v1) { return atan2f(v0.y - v1.y, v1.x - v0.x); }
static inline struct v2 v2_from_angle(float a) { return V2(cosf(a), -sinf(a)); }

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

static inline struct v2u v2u_lerp(struct v2u a, struct v2u b, float t) { return V2U(lerp(a.x, b.x, t), lerp(a.y, b.y, t)); }
static inline float v2u_mag(struct v2u v) { return sqrt(v.x*v.x + v.y*v.y); }
static inline float v2u_dist(struct v2u v0, struct v2u v1) { return v2u_mag(v2u_sub(v1, v0)); }

#endif/*__YMATH_H__*/
