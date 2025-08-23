#include "game/sound.h"
#include "engine/mixer.h"

static uint32_t sounds[SOUNDS_AMOUNT];

#define RESERVE_SOUND(sound, path, loop) do { \
  res = mixer_sound_reserve(path, false, loop); \
  if (!res.ok) return false; \
  sounds[sound] = res.sound_handle; \
} while (0)

bool
sound_init(void) {
  static_assert(SOUNDS_AMOUNT == 1, "not all sounds are initialized");
  return true;
}

uint32_t
sound_get(enum sound sound) {
  #if DEV
  if (sound >= SOUNDS_AMOUNT) {
    log_errorlf("%s: invalid sound number '%u'", __func__, sound);
    return INVALID_SOUND;
  }
  #endif
  return sounds[sound];
}
