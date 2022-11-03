#ifndef MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WAVEFORM_H
#define MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WAVEFORM_H

#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::wsg3 {

    class Waveform {
    public:
        explicit Waveform(std::vector<u8> samples);

        [[nodiscard]] std::vector<u8> samples() const;

    private:
        static constexpr u8 max_value_for_sample = 1 << 4;

        std::vector<u8> m_samples;
    };
}

#endif //MIKA_EMULATORS_CHIPS_NAMCO_WSG3_WAVEFORM_H