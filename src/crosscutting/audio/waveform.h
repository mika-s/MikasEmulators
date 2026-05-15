#pragma once

#include "crosscutting/typedefs.h"
#include <vector>

namespace emu::wsg3 {

class Waveform {
public:
    explicit Waveform(std::vector<u8> samples);

    [[nodiscard]] auto samples() const -> std::vector<u8>;

private:
    static constexpr u8 max_value_for_sample = 1 << 4;

    std::vector<u8> m_samples;
};
}
