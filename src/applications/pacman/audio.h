#pragma once

#include "chips/namco_wsg3/wsg3.h"
#include "crosscutting/typedefs.h"
#include <SDL_audio.h>
#include <vector>

namespace emu::wsg3 {
class Voice;
}
namespace emu::wsg3 {
class Waveform;
}

namespace emu::applications::pacman {

using emu::wsg3::Voice;
using emu::wsg3::Waveform;
using emu::wsg3::Wsg3;

class Audio {
public:
    Audio(std::vector<u8> const& sound_rom1, std::vector<u8> const& sound_rom2);

    ~Audio();

    void handle_sound(bool is_sound_enabled, std::vector<Voice>& voices);

    void toggle_mute();

    std::vector<Waveform> waveforms();

private:
    static constexpr unsigned int s_samples_per_waveform = 32;
    static constexpr int s_sdl_frequency = 44100;
    static constexpr int s_fps = 60;
    static constexpr float s_resampling_ratio = static_cast<float>(Wsg3::s_frequency) / static_cast<float>(s_sdl_frequency);
    static constexpr int s_samples_per_frame = s_sdl_frequency / s_fps;

    unsigned int m_volume = 20;

    SDL_AudioDeviceID m_audio_device;
    Wsg3 m_sound_chip;

    bool m_is_muted { false };

    std::vector<Waveform> load_waveforms_from_roms(
        std::vector<u8> const& sound_rom1,
        std::vector<u8> const& sound_rom2);
};
}
