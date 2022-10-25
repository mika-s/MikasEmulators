#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H

#include <vector>
#include "chips/namco_wsg3/voice.h"
#include "chips/namco_wsg3/wsg3.h"
#include "crosscutting/typedefs.h"

namespace emu::applications::pacman {

    using emu::wsg3::Voice;
    using emu::wsg3::Waveform;
    using emu::wsg3::Wsg3;

    class Audio {
    public:

        Audio(const std::vector<u8> &sound_rom1, const std::vector<u8> &sound_rom2);

        void handle_sound(bool is_sound_enabled, const std::vector<Voice> &voices);

        void mute();

        void unmute();

        void toggle_mute();

        std::vector<Waveform> waveforms();

    private:
        static constexpr unsigned int samples_per_waveform = 32;

        bool m_is_muted;

        Wsg3 m_sound_chip;

        Wsg3 load_waveforms_from_roms(const std::vector<u8> &sound_rom1, const std::vector<u8> &sound_rom2);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_AUDIO_H
