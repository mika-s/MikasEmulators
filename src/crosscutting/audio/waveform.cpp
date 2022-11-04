#include "waveform.h"
#include "typedefs.h"
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace emu::wsg3 {

    Waveform::Waveform(std::vector<u8> samples)
        : m_samples(std::move(samples)) {

        for (u8 sample: m_samples) {
            if (sample > max_value_for_sample) {
                throw std::invalid_argument(
                        fmt::format(
                                "Sample value is too large. Max value is {}, but the sample was {}",
                                +max_value_for_sample,
                                +sample
                        )
                );
            }
        }
    }

    std::vector<u8> Waveform::samples() const {
        return m_samples;
    }
}
