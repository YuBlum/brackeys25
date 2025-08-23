#ifndef __GLOBAL_H__
#define __GLOBAL_H__

struct global {
  float time;
  bool show_colliders;
};

extern struct global global;

void global_init(void);
void global_update(float dt);

#endif/*__GLOBAL_H__*/
