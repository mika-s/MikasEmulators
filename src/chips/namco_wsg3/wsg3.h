#ifndef MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H
#define MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H

#include "voice.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/audio/waveform.h"

namespace emu::wsg3 {

    class Wsg3 {
    public:
        static constexpr int frequency = 96000;

        explicit Wsg3(std::vector<Waveform> waveforms);

        std::vector<Waveform> waveforms();

        std::vector<i16> next_tick(std::vector<Voice> &voices);

    private:
        static constexpr unsigned int expected_number_of_waveforms = 16;
        static constexpr unsigned int expected_number_of_voices = 3;
        static constexpr int fps = 60;
        static constexpr unsigned int buffer_size = frequency / fps;
        static constexpr u32 mask_for_20_bit = 0xfffff;

        std::vector<Waveform> m_waveforms;
    };
}

#endif //MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H
