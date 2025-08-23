#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "engine/math.h"

static inline bool
check_rect_rect(struct v2 p0, struct v2 s0, struct v2 p1, struct v2 s1) {
  return p0.x - s0.x * 0.5f < p1.x + s1.x * 0.5f &&
         p0.x + s0.x * 0.5f > p1.x - s1.x * 0.5f &&
         p0.y - s0.y * 0.5f < p1.y + s1.y * 0.5f &&
         p0.y + s0.y * 0.5f > p1.y - s1.y * 0.5f;
}


static inline bool
check_rect_circle(struct v2 rp, struct v2 s, struct v2 cp, float r) {
  struct v2 d = v2_sub(cp, V2(MAX(rp.x - s.x * 0.5f, MIN(cp.x, rp.x + s.x * 0.5f)),
                              MAX(rp.y - s.y * 0.5f, MIN(cp.y, rp.y + s.y * 0.5f))));
  return (d.x*d.x + d.y*d.y) <= r*r;
}

static inline float
resolve_rect_rect_axis(float p0, float s0, float p1, float s1) {
  float half   = p0 < p1 ? -0.5f : +0.5f;
  float offset = half * (s0 + s1);
  return p1 + offset;
}

#endif/*__COLLISION_H__*/
