#include "audio.h"
#include "crosscutting/util/byte_util.h"
#include "sounds.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_stdinc.h>
#include <cstdlib>

namespace emu::applications::space_invaders {

using emu::util::byte::is_bit_set;

Audio::Audio()
    : m_is_ufo_sound_on(false)
    , m_is_shot_sound_on(false)
    , m_is_flash_sound_on(false)
    , m_is_invader_die_sound_on(false)
    , m_is_extended_play_sound_on(false)
    , m_is_fleet_movement_1_sound_on(false)
    , m_is_fleet_movement_2_sound_on(false)
    , m_is_fleet_movement_3_sound_on(false)
    , m_is_fleet_movement_4_sound_on(false)
    , m_is_ufo_hit_sound_on(false)
    , m_is_muted(false)
    , m_last_acc_reg(0)
{

    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL audio: %s", SDL_GetError());
        exit(1);
    }

    SDL_AudioSpec audio_spec {
        .freq = 11025,
        .format = AUDIO_S16SYS,
        .channels = 1,
        .silence = 0,
        .samples = 512,
        .padding = 0,
        .size = 0,
        .callback = forward_callback,
        .userdata = this
    };
    m_audio_device = SDL_OpenAudioDevice(
        nullptr,
        0,
        &audio_spec,
        nullptr,
        0);
    if (m_audio_device == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error opening audio device: %s", SDL_GetError());
        exit(1);
    }
    SDL_PauseAudioDevice(m_audio_device, 0);
}

Audio::~Audio()
{
    SDL_PauseAudioDevice(m_audio_device, 1);
    SDL_CloseAudioDevice(m_audio_device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Audio::play_sound_port_1(u8 acc_reg)
{
    if (is_rising_edge(acc_reg, s_ufo)) {
        m_is_ufo_sound_on = true;
    }
    if (is_falling_edge(acc_reg, s_ufo)) { // the UFO sound is looped, and disabled by a falling edge
        SDL_LockAudioDevice(m_audio_device);
        m_is_ufo_sound_on = false;
        SDL_UnlockAudioDevice(m_audio_device);
    }
    if (is_rising_edge(acc_reg, s_shot)) {
        m_is_shot_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_flash)) {
        m_is_flash_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_invader_die)) {
        m_is_invader_die_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_extended_play)) {
        m_is_extended_play_sound_on = true;
    }

    m_last_acc_reg = acc_reg;
}

void Audio::play_sound_port_2(u8 acc_reg)
{
    if (is_rising_edge(acc_reg, s_fleet_movement_1)) {
        m_is_fleet_movement_1_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_fleet_movement_2)) {
        m_is_fleet_movement_2_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_fleet_movement_3)) {
        m_is_fleet_movement_3_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_fleet_movement_4)) {
        m_is_fleet_movement_4_sound_on = true;
    }
    if (is_rising_edge(acc_reg, s_ufo_hit)) {
        m_is_ufo_hit_sound_on = true;
    }
}

void Audio::change_volume(unsigned int new_volume)
{
    SDL_LockAudioDevice(m_audio_device);
    m_volume = new_volume;
    SDL_UnlockAudioDevice(m_audio_device);
}

void Audio::mute()
{
    SDL_LockAudioDevice(m_audio_device);
    m_is_muted = true;
    SDL_UnlockAudioDevice(m_audio_device);
}

void Audio::unmute()
{
    SDL_LockAudioDevice(m_audio_device);
    m_is_muted = false;
    SDL_UnlockAudioDevice(m_audio_device);
}

void Audio::toggle_mute()
{
    SDL_LockAudioDevice(m_audio_device);
    m_is_muted = !m_is_muted;
    SDL_UnlockAudioDevice(m_audio_device);
}

void Audio::generate_audio(u8* stream, int len)
{
    auto* stream16 = (i16*)stream;
    int samples = static_cast<int>(len / sizeof(i16));

    SDL_memset(stream, 0, len); // no sound

    if (m_is_ufo_sound_on) {
        bool dummy = false; // m_is_ufo_sound_on will be disabled by falling edge, not when the sound is complete
        play(ufo_sound, samples, stream16, m_ufo_x, dummy);
    }
    if (m_is_shot_sound_on) {
        play(shot_sound, samples, stream16, m_shot_x, m_is_shot_sound_on);
    }
    if (m_is_flash_sound_on) {
        play(flash_sound, samples, stream16, m_flash_x, m_is_flash_sound_on);
    }
    if (m_is_invader_die_sound_on) {
        play(invader_die_sound, samples, stream16, m_invader_die_x, m_is_invader_die_sound_on);
    }
    if (m_is_extended_play_sound_on) {
        play(extended_play_sound, samples, stream16, m_extended_play_x, m_is_extended_play_sound_on);
    }
    if (m_is_fleet_movement_1_sound_on) {
        play(fleet_movement_1_sound, samples, stream16, m_fleet_movement_1_x, m_is_fleet_movement_1_sound_on);
    }
    if (m_is_fleet_movement_2_sound_on) {
        play(fleet_movement_2_sound, samples, stream16, m_fleet_movement_2_x, m_is_fleet_movement_2_sound_on);
    }
    if (m_is_fleet_movement_3_sound_on) {
        play(fleet_movement_3_sound, samples, stream16, m_fleet_movement_3_x, m_is_fleet_movement_3_sound_on);
    }
    if (m_is_fleet_movement_4_sound_on) {
        play(fleet_movement_4_sound, samples, stream16, m_fleet_movement_4_x, m_is_fleet_movement_4_sound_on);
    }
    if (m_is_ufo_hit_sound_on) {
        play(ufo_hit_sound, samples, stream16, m_ufo_hit_x, m_is_ufo_hit_sound_on);
    }
}

void Audio::play(std::vector<double> sound, int samples, i16* stream16, std::size_t& x, bool& is_sound_on) const
{
    for (int i = 0; i < samples; i++) {
        if (x < sound.size()) {
            stream16[i] += static_cast<i16>(sound[x++] * m_volume * !m_is_muted);
        } else {
            is_sound_on = false;
            x = 0;
            break;
        }
    }
}

bool Audio::is_rising_edge(u8 acc_reg, unsigned int value) const
{
    bool const is_currently_set = is_bit_set(acc_reg, value);
    bool const was_formerly_set = is_bit_set(m_last_acc_reg, value);

    return is_currently_set && !was_formerly_set;
}

bool Audio::is_falling_edge(u8 acc_reg, unsigned int value) const
{
    bool const is_currently_set = is_bit_set(acc_reg, value);
    bool const was_formerly_set = is_bit_set(m_last_acc_reg, value);

    return !is_currently_set && was_formerly_set;
}
}
