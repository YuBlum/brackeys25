#include <miniaudio.h>
#include <string.h>
#include "engine/core.h"
#include "engine/arena.h"
#include "engine/mixer.h"

struct sound {
  ma_decoder decoder;
  float volume;
  bool active;
};

struct mixer {
  struct arena *tmp_buffer_arena;
  struct sound *sounds;
  ma_mutex lock;
  ma_device device;
  float volume;
};
#define TMP_BUFFER_ARENA_CAPACITY (1ul<<20)

static struct mixer g_mixer;

static void
data_callback(ma_device *device, void *output_buffer, const void *input_buffer, uint32_t frame_count) {
  (void)device;
  (void)input_buffer;
  float *tmp_buffer = arena_push_array(g_mixer.tmp_buffer_arena, true, float, TMP_BUFFER_ARENA_CAPACITY);
  if (!tmp_buffer) {
    log_errorl("couldn't allocate temporary buffer for mixing");
    return;
  }
  float *out = output_buffer;
  ma_mutex_lock(&g_mixer.lock);
  uint32_t sounds_amount = arena_array_length(g_mixer.sounds);
  for (uint32_t i = 0; i < sounds_amount; i++) {
    if (!g_mixer.sounds[i].active) continue;
    (void)memset(tmp_buffer, 0, frame_count * sizeof (float)); 
    ma_uint64 frames_read;
    ma_result res = ma_data_source_read_pcm_frames(&g_mixer.sounds[i].decoder, tmp_buffer, frame_count, &frames_read);
    if (res != MA_SUCCESS) {
      g_mixer.sounds[i].active = false;
      continue;
    }
    if (frames_read < frame_count) g_mixer.sounds[i].active = false;
    for (uint32_t sample = 0; sample < frame_count; sample++) {
      out[sample] += g_mixer.volume * g_mixer.sounds[i].volume * tmp_buffer[sample];
      if (out[sample] > 1.0f) out[sample] = 1.0f;
      if (out[sample] < -1.0f) out[sample] = -1.0f;
    }
  }
  ma_mutex_unlock(&g_mixer.lock);
  if (!arena_clear(g_mixer.tmp_buffer_arena)) {
    log_errorl("couldn't clear mixer's temporary buffer arena");
  }
}

bool
mixer_make(void) {
  log_infol("making sound mixer...");
  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.playback.format   = ma_format_f32;
  config.playback.channels = 1;
  config.sampleRate        = 48000;
  config.dataCallback      = data_callback;
  log_warnl("for now all sounds and main audio device are loaded as monochannel. add stereo support");
  if (ma_device_init(0, &config, &g_mixer.device) != MA_SUCCESS) {
    log_errorl("couldn't obtain device for the mixer");
    return false;
  }
  log_infol("obtained device for the mixer");
  g_mixer.tmp_buffer_arena = arena_make_typed(TMP_BUFFER_ARENA_CAPACITY, float);
  if (!g_mixer.tmp_buffer_arena) {
    ma_device_uninit(&g_mixer.device);
    log_errorl("couldn't make mixer temporary buffer arena");
    return false;
  }
  log_infol("mixer temporary buffer arena created");
  g_mixer.sounds = arena_array_make(0, struct sound);
  if (!g_mixer.sounds) {
    log_errorl("couldn't make mixer sounds buffer");
    return false;
  }
  log_infol("mixer sounds buffer created");
  if (ma_mutex_init(&g_mixer.lock) != MA_SUCCESS) {
    ma_device_uninit(&g_mixer.device);
    log_infol("couldn't initialize mixer lock");
    return false;
  }
  log_infol("mixer mutex lock created");
  if (ma_device_start(&g_mixer.device) != MA_SUCCESS) {
    ma_device_uninit(&g_mixer.device);
    log_errorl("couldn't start sound mixer");
    return false;
  }
  log_infol("sound mixer started");
  g_mixer.volume = 1.0f;
  log_infol("mixer creation complete!");
  return true;
}

void
mixer_destroy(void) {
  log_warnlf("%s: mixer arena isn't destroyed here, don't call 'mixer_make' after this", __func__);
  mixer_clear_sounds();
  ma_mutex_uninit(&g_mixer.lock);
  ma_device_uninit(&g_mixer.device);
}

struct sound_result
mixer_sound_reserve(const char *sound_file_path, bool active_on_start, bool loop) {
  ma_mutex_lock(&g_mixer.lock);
  struct sound *sound = arena_array_grow(g_mixer.sounds, true, 1);
  if (!sound) {
    log_errorlf("%s: couldn't allocate sound", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return (struct sound_result) { 0, false };
  }
  ma_decoder_config decoder_config = ma_decoder_config_init(
    g_mixer.device.playback.format,
    g_mixer.device.playback.channels,
    g_mixer.device.sampleRate
  );
  if (ma_decoder_init_file(sound_file_path, &decoder_config, &sound->decoder) != MA_SUCCESS) {
    log_errorlf("%s: couldn't initiate sound decoder", __func__);
    arena_array_pop(g_mixer.sounds);
    ma_mutex_unlock(&g_mixer.lock);
    return (struct sound_result) { 0, false };
  }
  if (loop) {
    if (ma_data_source_set_looping(&sound->decoder, true) != MA_SUCCESS) {
      log_warnlf("%s: couldn't set sound to loop", __func__);
    }
  }
  sound->volume = 1.0f;
  sound->active = active_on_start;
  struct sound_result res = { arena_array_length(g_mixer.sounds) - 1, true };
  ma_mutex_unlock(&g_mixer.lock);
  return res;
}

bool
mixer_sound_play(uint32_t sound_handle) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  struct sound *sound = &g_mixer.sounds[sound_handle];
  sound->active = true;
  if (ma_data_source_seek_to_pcm_frame(&sound->decoder, 0) != MA_SUCCESS) {
    log_errorlf("%s: couldn't reset sound", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

bool
mixer_sound_pause(uint32_t sound_handle) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  g_mixer.sounds[sound_handle].active = false;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

bool
mixer_sound_resume(uint32_t sound_handle) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  g_mixer.sounds[sound_handle].active = true;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

bool
mixer_sound_toggle(uint32_t sound_handle) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  g_mixer.sounds[sound_handle].active = !g_mixer.sounds[sound_handle].active;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

bool
mixer_sound_set_volume(uint32_t sound_handle, float new_volume) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  if (new_volume > 1.0f) new_volume = 1.0f;
  if (new_volume < 0.0f) new_volume = 0.0f;
  g_mixer.sounds[sound_handle].volume = new_volume;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

float
mixer_sound_get_volume(uint32_t sound_handle) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return 0.0f;
  }
  float volume = g_mixer.sounds[sound_handle].volume;
  ma_mutex_unlock(&g_mixer.lock);
  return volume;
}

bool
mixer_sound_inc_volume(uint32_t sound_handle, float amount) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  g_mixer.sounds[sound_handle].volume += amount;
  if (g_mixer.sounds[sound_handle].volume > 1.0f) g_mixer.sounds[sound_handle].volume = 1.0f;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

bool
mixer_sound_dec_volume(uint32_t sound_handle, float amount) {
  ma_mutex_lock(&g_mixer.lock);
  if (sound_handle >= arena_array_length(g_mixer.sounds)) {
    log_errorlf("%s: invalid sound handle", __func__);
    ma_mutex_unlock(&g_mixer.lock);
    return false;
  }
  g_mixer.sounds[sound_handle].volume -= amount;
  if (g_mixer.sounds[sound_handle].volume < 0.0f) g_mixer.sounds[sound_handle].volume = 0.0f;
  ma_mutex_unlock(&g_mixer.lock);
  return true;
}

void
mixer_clear_sounds(void) {
  ma_mutex_lock(&g_mixer.lock);
  uint32_t sounds_length = arena_array_length(g_mixer.sounds);
  for (uint32_t i = 0; i < sounds_length; i++) {
    (void)ma_decoder_uninit(&g_mixer.sounds[i].decoder);
  }
  arena_array_clear(g_mixer.sounds);
  ma_mutex_unlock(&g_mixer.lock);
}

void
mixer_set_volume(float new_volume) {
  ma_mutex_lock(&g_mixer.lock);
  if (new_volume > 1.0f) new_volume = 1.0f;
  if (new_volume < 0.0f) new_volume = 0.0f;
  g_mixer.volume = new_volume;
  ma_mutex_unlock(&g_mixer.lock);
}

float
mixer_get_volume(void) {
  ma_mutex_lock(&g_mixer.lock);
  float volume = g_mixer.volume;
  ma_mutex_unlock(&g_mixer.lock);
  return volume;
}

void
mixer_inc_volume(float amount) {
  ma_mutex_lock(&g_mixer.lock);
  g_mixer.volume += amount;
  if (g_mixer.volume > 1.0f) g_mixer.volume = 1.0f;
  ma_mutex_unlock(&g_mixer.lock);
}

void
mixer_dec_volume(float amount) {
  ma_mutex_lock(&g_mixer.lock);
  g_mixer.volume -= amount;
  if (g_mixer.volume < 0.0f) g_mixer.volume = 0.0f;
  ma_mutex_unlock(&g_mixer.lock);
}
