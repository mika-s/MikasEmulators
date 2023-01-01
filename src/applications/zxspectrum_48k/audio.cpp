#include "audio.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_stdinc.h>
#include <cstdlib>

namespace emu::applications::zxspectrum_48k {

Audio::Audio()
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error initializing SDL audio: %s", SDL_GetError());
        exit(1);
    }

    SDL_AudioSpec audio_spec {
        .freq = s_sdl_frequency,
        .format = AUDIO_S16SYS,
        .channels = 1,
        .silence = 0,
        .samples = 1024,
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

void Audio::beep()
{
}

void Audio::toggle_mute()
{
    m_is_muted = !m_is_muted;
}

void Audio::generate_audio(u8* stream, int len)
{
    [[maybe_unused]] auto* stream16 = (i16*)stream;
    [[maybe_unused]] int samples = static_cast<int>(len / sizeof(i16));

    SDL_memset(stream, 0, len); // no sound

    //    play(ufo_sound, samples, stream16, m_ufo_x, dummy);
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
}
