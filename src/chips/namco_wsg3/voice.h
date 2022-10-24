#ifndef MIKA_EMULATORS_CHIPS_NAMCO_WSG3_VOICE_H
#define MIKA_EMULATORS_CHIPS_NAMCO_WSG3_VOICE_H

#include "crosscutting/typedefs.h"

namespace emu::wsg3 {

    class Voice {
    public:
        Voice();

        u32 waveform_number();

        void waveform_number(u32 waveform_number);

        u32 frequency();

        void frequency(u32 frequency);

        u8 volume();

        void volume(u8 volume);

        u8 accumulator();

        void accumulator(u8 accumulator);

    private:
        static constexpr u32 waveforms_supported = 8;
        static constexpr u32 max_frequency = 1 << 20;
        static constexpr u8 volume_levels_supported = 16;

        u32 m_waveform_number;
        u32 m_frequency;
        u8 m_volume;
        u8 m_accumulator;
    };
}

#endif //MIKA_EMULATORS_CHIPS_NAMCO_WSG3_VOICE_H
