#ifndef __GSOUND_H__
#define __GSOUND_H__

#include "engine/core.h"
#include "engine/mixer.h"

#define INVALID_SOUND ((uint32_t)-1)

enum sound {
  SOUNDS_AMOUNT = 1
};

bool sound_init(void);
uint32_t sound_get(enum sound sound);

#endif/*__GSOUND_H__*/
