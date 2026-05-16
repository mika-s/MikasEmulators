#include "waveform.h"
#include "crosscutting/typedefs.h"
#include <format>
#include <stdexcept>
#include <utility>

namespace emu::wsg3 {

Waveform::Waveform(std::vector<u8> samples)
    : m_samples(std::move(samples))
{
    for (u8 const sample : m_samples) {
        if (sample > max_value_for_sample) {
            throw std::invalid_argument(
                std::format(
                    "Sample value is too large. Max value is {}, but the sample was {}",
                    +max_value_for_sample,
                    +sample));
        }
    }
}

auto Waveform::samples() const -> std::vector<u8>
{
    return m_samples;
}
}
