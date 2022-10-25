#ifndef MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H
#define MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H

#include "voice.h"
#include "waveform.h"
#include "crosscutting/typedefs.h"

namespace emu::wsg3 {

    class Wsg3 {
    public:
        explicit Wsg3(std::vector<Waveform> waveforms);

        std::vector<Waveform> waveforms();

        void next_tick(const std::vector<Voice> &voices);

    private:
        static constexpr unsigned int expected_number_of_waveforms = 16;
        static constexpr unsigned int expected_number_of_voices = 3;

        std::vector<Waveform> m_waveforms;
    };
}

#endif //MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WSG3_H
