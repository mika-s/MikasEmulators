#pragma once

#include "crosscutting/typedefs.h"
#include <SDL_audio.h>
#include <cstddef>
#include <vector>

namespace emu::applications::zxspectrum_48k {

class Audio {
public:
    Audio();

    ~Audio();

    void beep();

    void toggle_mute();

private:
    static constexpr int s_sdl_frequency = 44100;
    static constexpr int s_fps = 60;

    unsigned int m_volume = 5000;

    SDL_AudioDeviceID m_audio_device;

    bool m_is_muted { false };

    void generate_audio(u8* stream, int len);

    void play(std::vector<double> sound, int samples, i16* stream16, std::size_t& x, bool& is_sound_on) const;

    static void forward_callback(void* userdata, u8* stream, int len)
    {
        static_cast<Audio*>(userdata)->generate_audio(stream, len);
    }
};
}
