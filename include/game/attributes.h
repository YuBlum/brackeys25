#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

struct heaviness { uint8_t value; };
static const float g_speed_by_heaviness[6] = {
    0.125f,
    0.250f,
    0.375f,
    0.500f,
    0.625f,
    0.750f
};

#endif/*__ATTRIBUTES_H__*/
