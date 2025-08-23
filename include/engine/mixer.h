#ifndef __MIXER_H__
#define __MIXER_H__

struct sound_result {
  uint32_t sound_handle;
  bool ok;
};

bool mixer_make(void);
void mixer_destroy(void);

struct sound_result mixer_sound_reserve(const char *sound_file_path, bool active_on_start, bool loop);
bool mixer_sound_play(uint32_t sound_handle);
bool mixer_sound_pause(uint32_t sound_handle);
bool mixer_sound_resume(uint32_t sound_handle);
bool mixer_sound_toggle(uint32_t sound_handle);
bool mixer_sound_set_volume(uint32_t sound_handle, float new_volume);
float mixer_sound_get_volume(uint32_t sound_handle);
bool mixer_sound_inc_volume(uint32_t sound_handle, float amount);
bool mixer_sound_dec_volume(uint32_t sound_handle, float amount);
void mixer_clear_sounds(void);

void mixer_set_volume(float new_volume);
float mixer_get_volume(void);
void mixer_inc_volume(float amount);
void mixer_dec_volume(float amount);

#endif/*__MIXER_H__*/
