#ifndef __GLOBAL_H__
#define __GLOBAL_H__

struct global {
    bool show_debug;
};

extern struct global global;

void global_init(void);
void global_update(float dt);

#endif/*__GLOBAL_H__*/
