#pragma once

#include "crosscutting/typedefs.h"

namespace emu::wsg3 {

class Voice {
public:
    Voice();

    [[nodiscard]] u8 waveform_number() const;

    void waveform_number(u8 waveform_number);

    [[nodiscard]] u32 frequency() const;

    void frequency(u32 frequency);

    [[nodiscard]] u8 volume() const;

    void volume(u8 volume);

    [[nodiscard]] u32 accumulator() const;

    void accumulator(u32 accumulator);

private:
    static constexpr u8 waveforms_supported = 8;
    static constexpr u32 max_frequency = 1 << 20;
    static constexpr u8 volume_levels_supported = 16;

    u8 m_waveform_number;
    u32 m_frequency;
    u8 m_volume;
    u32 m_accumulator;
};
}
